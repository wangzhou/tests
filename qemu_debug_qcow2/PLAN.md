# ARM SMMU HDBSS 嵌套虚拟化项目 — 整体架构与分步计划

## 一、关键概念澄清

本项目涉及 ARM SMMU 的两个**独立但有依赖关系**的特性：

### SMMU HTTU (Hardware Translation Table Update)
- **本质**：SMMU 页表遍历时，通过 DBM（Dirty Bit Modifier）机制自动更新 PTE 中的 Access/Dirty 位
- **角色**：**脏页"检测"能力** — SMMU 知道自己翻译了哪些页、写入了哪些页
- **效果**：消除了软件处理 access/dirty fault 的开销，但软件仍需扫描全部 PTE 才能找出脏页

### SMMU HDBSS (Hardware Dirty Bit State Structure)
- **本质**：SMMU 内部的硬件环形 buffer，自动记录被脏化的页地址
- **角色**：**脏页"上报"能力** — 脏页信息从 PTE 中"推送"到 ring buffer，软件直接读取即可
- **效果**：O(1) 获取脏页列表，不用再扫描全部 PTE

### 依赖关系

```
SMMU HTTU ──(前置依赖)──→ SMMU HDBSS
   ↑                          ↑
 "检测脏页"               "上报脏页"
 (往 PTE 写 DBM 位)      (记录脏页地址到 ring buffer)
```

> HDBSS 依赖 HTTU：SMMU 必须先能检测到脏页（HTTU），才能把脏页地址写入 ring buffer（HDBSS）。
> 只开 HTTU 不开 HDBSS 是合法的：脏位标记在 PTE 里，软件自己扫描，效率低但不影响正确性。

### 与 CPU FEAT_HDBSS 的区别

| | SMMU HDBSS | CPU FEAT_HDBSS (Armv9.5) |
|------|------------|--------------------------|
| 跟踪对象 | **设备 DMA** 产生的脏页 | **CPU** 访问产生的脏页 |
| 硬件 | SMMU 内部 ring buffer | CPU 侧 HDBSSBR_EL2/HDBSSPROD_EL2 寄存器 |
| 使用者 | IOMMUFD / VFIO 设备迁移 | KVM guest 内存迁移 |
| 本文档 | **核心目标** | 可选补充（Phase 6） |

两者互不依赖，协同工作时实现完整覆盖：CPU 跟踪 vCPU 脏页，SMMU 跟踪设备 DMA 脏页。

---

## 二、项目目标

在无专用硬件平台的情况下，利用 QEMU 模拟构建一套完整环境，开发和验证 **ARM SMMU HDBSS**：

1. QEMU L1 中模拟 SMMUv3 **HTTU**（脏页检测）
2. QEMU L1 中模拟 SMMUv3 **HDBSS**（脏页上报 ring buffer），**依赖步骤 1**
3. Guest 内核中增加 SMMUv3 HTTU + HDBSS 驱动，通过 IOMMUFD 暴露 dirty tracking
4. QEMU L2 中基于 VFIO/IOMMUFD 实现设备热迁移的脏页跟踪
5. 配套：L1 QEMU 中模拟 NVMe SR-IOV（VF 直通用）

---

## 三、项目全景架构

```
                src                                    dst

            Inner VM                              Inner VM
                |                                     |
            QEMU L2       <--- hot migration --->  QEMU L2
            (VFIO user)                            (VFIO user)
                |                                     |
            Outer VM                              Outer VM
            kernel                                kernel
                |                                     |
            QEMU L1                               QEMU L1
            (hw sim)                              (hw sim)
                |                                     |
               tap0                                  tap1
                \--------------- br0 ----------------/
                                  |
                             192.168.100.1
```

各层详情：

| 层 | 组件 | 关键内容 |
|---|------|---------|
| L2 QEMU | VFIO 用户态 | passthrough NVMe VF; query IOMMUFD dirty bitmap; hot migration |
| Outer VM | Linux Kernel | SMMUv3 driver (HTTU + HDBSS); IOMMUFD; VFIO; KVM (nested) |
| L1 QEMU | 硬件模拟 | SMMUv3 HTTU + HDBSS simulation; NVMe SR-IOV simulation |
| Host | 网络 | bridge br0 + tap0/tap1, 192.168.100.0/24 |

### 数据流: 一次 DMA 如何触发脏页跟踪

```
NVMe VF DMA write
        |
        v
SMMU HTTU (DBM mark dirty in PTE)
        |
        v
SMMU HDBSS (record GPA to ring buffer)
        |
        v
IOMMUFD (IOMMU_HWPT_GET_DIRTY_BITMAP ioctl)
        |
        v
VFIO migration (send dirty pages to dst)
```

### 三个层面各自的工作

| 层面 | 做什么 | HTTU 相关 | HDBSS 相关 |
|------|--------|----------|-----------|
| **QEMU L1** | **硬件模拟** — 构造 SMMUv3 寄存器、PTE 遍历、DBM 更新、ring buffer 行为 | 声明 IDR1.HTTU 能力；在 PTE 遍历中实现 DBM 脏位自动更新 | 声明 HDBSS 能力；模拟 ring buffer 写入 + 满时中断 |
| **Guest 内核** | **驱动** — 驱动模拟硬件，对上层 IOMMUFD 暴露 dirty tracking | 使能 HTTU（写 CR0/CD.TCR）；通过 DBM 标脏 | 分配/管理 HDBSS ring buffer；消费 HDBSS 上报的脏页记录；通过 IOMMUFD 传递 dirty bitmap |
| **QEMU L2** | **用户态消费** — 通过 IOMMUFD 查询脏页位图，驱动热迁移 | 无直接编码 | 调用 `IOMMU_HWPT_GET_DIRTY_BITMAP` 获取脏页；集成到 VFIO migration 流程 |

---

## 四、当前环境

| 项目 | 版本/状态 |
|------|----------|
| Host OS | openEuler 24.03 LTS SP2 (aarch64) |
| Host kernel | 6.6.0-98 |
| QEMU | 8.2.0 (系统包，太老，需从 git 编译) |
| Guest 内核 | 自编译 (`/home/wz/linux`)，ARM_SMMU_V3=y |
| Guest 镜像 | openEuler aarch64 qcow2 (src + dst) |
| 网络 | bridge br0 + tap0/tap1，src/dst 互通已就绪 |
| IOMMUFD | 当前 **未使能** (`# CONFIG_IOMMUFD is not set`) |
| ARM_SMMU_V3_SVA | 当前 **未使能** (`# CONFIG_ARM_SMMU_V3_SVA is not set`) |

---

## 五、分步实施计划

### Phase 0: 基础环境升级（预估 1-2 天）

**目标**：升级 QEMU 和内核配置，满足后续开发的前置条件。

| 步骤 | 内容 | 验证方式 |
|------|------|---------|
| 0.1 | 从 git 编译最新 QEMU（≥ 10.x，建议直接用 master） | `qemu-system-aarch64 --version` ≥ 10.0 |
| 0.2 | Guest 内核：打开 `CONFIG_IOMMUFD=y`, `CONFIG_IOMMUFD_VFIO_CONTAINER=y`, `CONFIG_ARM_SMMU_V3_SVA=y` | grep 对应 config |
| 0.3 | 重新编译 guest 内核并验证启动 | 虚机正常启动 |
| 0.4 | 验证嵌套 KVM：Outer VM 内确认 `/dev/kvm` 存在 | `ls -la /dev/kvm` |

---

### Phase 1: NVMe SR-IOV 模拟 + VF 直通（预估 2-3 周）

**目标**：在 L1 QEMU 中模拟带 VF 的 NVMe，打通 "L2 QEMU → VFIO → NVMe VF → Inner VM" 链路。

| 步骤 | 内容 | 关键文件/补丁 |
|------|------|--------------|
| 1.1 | L1 QEMU 启动命令中添加 NVMe SR-IOV：`-device nvme,id=nvme0,serial=deadbeef,sriov_max_vfs=8,sriov_vq_flexible=2,num_queues=64` | 启动脚本 |
| 1.2 | Outer VM 内验证 NVMe PF + 8 VF 可见（`lspci \| grep -i nvme`） | Guest NVMe 驱动 |
| 1.3 | 拉取 [NVMe live migration v2](https://patchew.org/QEMU/20260304091229.80725-1-alexander@mihalicyn.com/)（4 patches, 2026.3），应用到 QEMU | `hw/nvme/*` |
| 1.4 | 在 patch 基础上解除 SR-IOV 迁移限制（当前明确禁用） | `hw/nvme/ctrls/sriov.c` |
| 1.5 | Outer VM 内 bind NVMe VF → vfio-pci；L2 QEMU `-device vfio-pci,host=0000:00:01.1` 直通 | L2 启动命令 |
| 1.6 | Inner VM 验证 NVMe VF 可用（`lsblk` + `fio` 读写） | Inner VM |

---

### Phase 2: QEMU L1 — SMMUv3 HTTU 硬件模拟（预估 3-4 周）

**目标**：在 L1 QEMU 的 SMMUv3 模型中模拟 HTTU —— 即 SMMU 页表遍历时通过 DBM 自动更新 PTE 脏位。**这是 HDBSS 的前置依赖。**

| 步骤 | 内容 | 关键文件 |
|------|------|---------|
| 2.1 | 研究 QEMU SMMUv3 模型架构（`hw/arm/smmuv3.c`、`smmu-common.c`、`smmu-internal.h`）和 accel 路径（`hw/arm/smmu-v3-accel.c`） | `hw/arm/smmuv3*` |
| 2.2 | 在 `SMMU_IDR1` 中声明 HTTU 能力：设置 `IDR1.HTTU` 为非零值，表示支持 HA（Hardware Access）和 HD（Hardware Dirty） | `smmuv3_reset()` |
| 2.3 | 实现 `SMMU_CR0` 中 HTTU 控制位：接受 guest 写 `CR0.HA=1` / `CR0.HD=1` | `smmuv3_write_cr0()` |
| 2.4 | 在 CD (Context Descriptor) TCR 中解析 `HA`/`HD` 位，per-stream 控制使能 | CD decode 路径 |
| 2.5 | **核心**：在 PTE 遍历逻辑中实现 DBM —— 写访问命中 `DBM=1` 的 PTE 时，自动置上 AP[2]（dirty bit），模拟 SMMU 硬件的脏位更新行为 | `smmu-common.c` PTE walk |
| 2.6 | 添加 trace events 便于调试（`-trace smmuv3_dirty*`） | `trace-events` |
| 2.7 | Guest 内 `dmesg \| grep -i smmu` 确认 HTTU 能力检测成功 | Guest 日志 |

> **此阶段完成后，SMMU 的脏页"检测"能力就绪。脏位写在 PTE 里，软件可以扫描 PTE 获取脏页（慢但可用）。**

---

### Phase 3: QEMU L1 — SMMU HDBSS 硬件模拟（预估 3-4 周）

**目标**：在 HTTU 基础上，模拟 SMMU HDBSS ring buffer 硬件行为 —— 脏页地址自动写入环 buffer，buffer 满触发中断。**依赖 Phase 2。**

| 步骤 | 内容 | 关键文件 |
|------|------|---------|
| 3.1 | 研究 SMMU HDBSS 硬件 spec：ring buffer 基址/大小/生产者消费者指针寄存器定义 | SMMUv3 arch spec |
| 3.2 | 在 SMMU ID 寄存器中声明 HDBSS 能力（如 `IDR3.HDBSS` 或等效标志位） | `smmuv3_reset()` |
| 3.3 | 实现 HDBSS 控制寄存器：ring buffer 基址寄存器、大小配置、生产者/消费者指针 | `hw/arm/smmuv3.c` |
| 3.4 | **核心**：在 HTTU 脏位更新路径上 hook HDBSS 写入 —— 当 DBM 置脏一个 PTE 时，同时将该页的 GPA 写入 HDBSS ring buffer 的当前生产者位置，推进生产者指针 | `smmu-common.c` |
| 3.5 | 实现 ring buffer 满检测：当生产者指针 + 1 == 消费者指针（或等效条件），触发 HDBSS 中断（event queue 注入） | `smmuv3_trigger_irq()` |
| 3.6 | Guest 驱动侧：通过读取消费者指针 + ring buffer 内容来消费脏页记录，更新消费者指针让硬件继续写入 | Guest 内核 |
| 3.7 | 添加 HDBSS trace events | `trace-events` |

> **此阶段完成后，SMMU 的脏页"上报"能力就绪。驱动可直接从 ring buffer 读取脏页地址列表，O(1) 效率。**

---

### Phase 4: Guest 内核 — SMMUv3 HTTU + HDBSS 驱动 + IOMMUFD（预估 3-4 周）

**目标**：让 Outer VM 内核驱动模拟的 SMMU HTTU + HDBSS 硬件，通过 IOMMUFD 向上暴露 dirty tracking。

| 步骤 | 内容 | 关键补丁/文件 |
|------|------|--------------|
| 4.1 | Backport SMMUv3 HTTU patches（mainline v6.11 已合入）到 6.6 内核 | `drivers/iommu/arm/arm-smmu-v3/*` |
| 4.2 | 实现 SMMUv3 HDBSS 驱动：ring buffer 分配、初始化、中断处理、脏页消费、IOMMUFD 对接 | `arm-smmu-v3-hdbss.c`（新增） |
| 4.3 | 在 IOMMUFD 层增加对 SMMU HDBSS dirty tracking 的支持：`IOMMU_HWPT_ALLOC_DIRTY_TRACKING` 能够利用 HDBSS ring buffer 获取脏页 | `drivers/iommu/iommufd/*` |
| 4.4 | 确保内核配置就绪：`IOMMUFD=y`, `IOMMUFD_VFIO_CONTAINER=y`, `ARM_SMMU_V3_SVA=y` | `.config` |
| 4.5 | 打上 [SVA HTTU 补丁](https://patchew.org/linux/20260503135413.1108138-1-nicolinc@nvidia.com/)（Nicolin Chen, 2026.5），使能 SVA 场景 HA/HD | `arm-smmu-v3-sva.c` |
| 4.6 | 验证：`dmesg` 确认 SMMUv3 HTTU + HDBSS 能力检测成功；编写 selftest 验证 IOMMUFD dirty bitmap 查询 | `tools/testing/selftests/iommu/` |

---

### Phase 5: QEMU L2 — VFIO 脏页跟踪 + 热迁移（预估 3-4 周）

**目标**：在 L2 QEMU 中通过 IOMMUFD 利用 SMMU HDBSS 进行 VFIO 设备热迁移。

| 步骤 | 内容 | 关键文件 |
|------|------|---------|
| 5.1 | 研究 QEMU 10.x/11.x IOMMU dirty tracking 框架（`hw/vfio/migration.c` + `backends/iommufd.c` + `hw/vfio/iommufd.c`） | `hw/vfio/*` |
| 5.2 | L2 QEMU 通过 IOMMUFD 创建带 `IOMMU_HWPT_ALLOC_DIRTY_TRACKING` 的 HWPT | `backends/iommufd.c` |
| 5.3 | 集成 dirty bitmap sync：QEMU migration 框架调用 `IOMMU_HWPT_GET_DIRTY_BITMAP` ioctl（背靠 SMMU HDBSS ring buffer） | `hw/vfio/container.c` |
| 5.4 | 补全 SMMUv3 vIOMMU 下的迁移支持（QEMU 11.0 `accel=on` 已合入，迁移未完整） | `hw/arm/smmuv3.c` |
| 5.5 | 端到端测试：src L2 QEMU（Inner VM + NVMe VF）→ 热迁移 → dst L2 QEMU，验证 VM 状态 + NVMe VF 状态正确恢复、脏页跟踪生效 | 完整流程 |

---

### Phase 6: （可选）CPU FEAT_HDBSS（预估 3-4 周）

**目标**：额外增加 Armv9.5 CPU FEAT_HDBSS（HDBSSBR_EL2/HDBSSPROD_EL2），让 KVM 也具备硬件脏页跟踪。与 SMMU HDBSS 互补，前者跟踪 vCPU 脏页，后者跟踪设备 DMA 脏页。

| 步骤 | 内容 | 关键补丁 |
|------|------|---------|
| 6.1 | 应用 [FEAT_HDBSS v3](https://lore.kernel.org/lkml/20260225040421.2683931-1-zhengtian10@huawei.com/)（5 patches, 2026.2）到 guest 内核 | `arch/arm64/kvm/*` |
| 6.2 | L1 QEMU CPU 模型中模拟 `HDBSSBR_EL2`、`HDBSSPROD_EL2` 系统寄存器 + ring buffer 行为 + HDBSSF 异常 | `target/arm/helper.c` |
| 6.3 | 联动测试：CPU FEAT_HDBSS（KVM dirty）+ SMMU HDBSS（DMA dirty）双重覆盖 | 完整迁移测试 |

---

## 六、依赖关系总图

```
Phase 0 (基础升级)
  │
  ├──→ Phase 1 (NVMe SR-IOV)
  │       └──→ VF 直通链路就绪 (可与 Phase 2+3 并行)
  │
  └──→ Phase 2 (QEMU L1: SMMU HTTU 模拟)
            │
            │  [严格依赖]
            ▼
          Phase 3 (QEMU L1: SMMU HDBSS 模拟)
            │
            │  [严格依赖]
            ▼
          Phase 4 (Guest 内核: HTTU + HDBSS 驱动 + IOMMUFD)
            │
            │  [严格依赖]
            ▼
          Phase 5 (QEMU L2: VFIO dirty track + 热迁移)
            │
            └──→ 端到端嵌套迁移验证

          Phase 6 (CPU FEAT_HDBSS, 可选, 与 Phase 2-5 解耦)
```

- **Phase 0 是所有工作的前置**
- **Phase 1 与 Phase 2-3 可并行**（NVMe 模块 vs SMMU 模块，互不依赖）
- **Phase 2 → 3 → 4 → 5 是严格串行链**，项目核心路径
- **Phase 6 与核心路径解耦**，可与 Phase 3 之后任意阶段并行

---

## 七、关键补丁清单

| 补丁 | 作者 | 版本 | 状态 | 用途 | Phase |
|------|------|------|------|------|-------|
| [QEMU NVMe live migration](https://patchew.org/QEMU/20260304091229.80725-1-alexander@mihalicyn.com/) | Mikhalitsyn | v2 (2026.3) | 审核中 | L1 NVMe 迁移（需解 SR-IOV 限制） | 1 |
| [SMMUv3 HTTU mainline](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/log/?qt=grep&q=HTTU) | Jiang/Kolothum | v6.11 | **已合入** | Guest 内核 HTTU 基础 | 4 |
| [SMMUv3 SVA HTTU](https://patchew.org/linux/20260503135413.1108138-1-nicolinc@nvidia.com/) | Nicolin Chen | v1 (2026.5) | 审核中 | SVA 场景 HA/HD | 4 |
| [QEMU SMMUv3 user-creatable](https://patchew.org/QEMU/20250703084643.85740-1-shameerali.kolothum.thodi@huawei.com/) | Kolothum | v6 (2025.7) | 审核中 | SMMUv3 device model | 2 |
| [vIOMMU migration relax](https://qemu.googlesource.com/qemu/+/0e3c1e2b) | Duan/Martins | 已合入 | **已合入 QEMU** | vIOMMU 迁移解禁 | 5 |
| [SMMUv3 accel dirty tracking](https://qemu.googlesource.com/qemu/+/659275f8) | Kolothum | 已合入 | **已合入 QEMU** | 嵌套父 HWPT 脏位 flag | 5 |
| [FEAT_HDBSS v3](https://lore.kernel.org/lkml/20260225040421.2683931-1-zhengtian10@huawei.com/) | Tian Zheng | v3 (2026.2) | 审核中 | CPU HDBSS（可选） | 6 |
| [pKVM SMMUv3 v4](https://lore.kernel.org/20250819215156.2494305-21-smostafa@google.com/) | Mostafa Saleh | v4 (2025.8) | 审核中 | 嵌套 SMMUv3 参考 | 参考 |

---

## 八、风险与挑战

| 风险 | 影响 | 缓解策略 |
|------|------|---------|
| QEMU SMMUv3 HTTU 模拟工作量大（PTE 遍历逻辑复杂） | Phase 2 延期 | 先做最小版本：声明 HTTU + 基础 DBM 置位，后续迭代 |
| SMMU HDBSS 硬件 spec 不公开或文档不全 | Phase 3 阻塞 | 从内核 FEAT_HDBSS patch 逆推行为；参考 Intel VT-d PML 的模拟实现 |
| NVMe SR-IOV 迁移 patch 不成熟 | Phase 1 延期 | 先做 VF 直通（1.1-1.2, 1.5-1.6），迁移部分等 upstream |
| QEMU 8.2 → master 升级兼容性 | Phase 0 阻塞 | 保留 8.2 备份 |
| 内核 backport 冲突（6.6 → 6.11 HTTU） | Phase 4 延期 | 评估直接升级到 6.12+ |
| HDBSS ring buffer 模拟 + guest 中断联动调试极难 | Phase 3+4 | 充分打 trace events；先单步验证 ring buffer 读写 |
