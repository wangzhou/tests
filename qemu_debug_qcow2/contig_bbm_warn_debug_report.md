# ARM64 KVM Stage-2 Contig BBM WARN_ON 调试报告

## 问题现象

启动使用 64K hugetlb 的 L2 虚机时，`arch/arm64/kvm/hyp/pgtable.c` 中 `stage2_map_contig_leaf()` 的 WARN_ON_ONCE 稳定触发：

```
WARNING: arch/arm64/kvm/hyp/pgtable.c:1098 at stage2_map_walker_try_leaf
CPU: CPU 0/KVM
Call trace:
  stage2_map_walker_try_leaf
  stage2_map_walker
  __kvm_pgtable_walk
  kvm_pgtable_walk
  kvm_pgtable_stage2_map
  kvm_handle_guest_abort
```

WARN 处代码：
```c
for (i = 1; i < CONT_PTES; i++, ptep++) {
    WARN_ON_ONCE(stage2_pte_is_locked(*ptep));  // line 1098
```

触发后 L2 QEMU 崩溃（`regime_is_user: code should not be reached`），进而 L1 QEMU TCG 模拟器也崩溃。

## 调试过程

### 1. 排除 L2 多 vCPU 竞态

用 `-smp 1` 启动 L2（单核），WARN 仍然触发 → 不是 L2 多 vCPU 之间的竞态。

### 2. 排除内核抢占

编译 `CONFIG_PREEMPT_NONE` 内核，WARN 仍然触发 → 抢占不是根因。

### 3. Debug 追踪锁来源

在 `stage2_try_break_pte` 中加 per-CPU ring buffer 记录每次锁 PTE 的地址和 CPU，WARN 处搜索匹配。ring buffer 匹配到 CPU 0/1/2 的锁记录，但不能确定具体调用路径。

### 4. 错误假设：并发竞态

最初认为是 contig BBM 和单页 BBM 之间的真正竞态：
- contig BBM 锁 PTE[0]，发现 PTE[k] (k>0) 已被其他线程锁
- 尝试了 lock-then-check 方案（双方先锁自己的再检查对方的）
- WARN 仍然触发

### 5. 根因发现

仔细检查 for 循环的 `ptep++` 执行时机：

```c
kvm_pte_t *ptep = (kvm_pte_t *)ctx->ptep;  // ptep = PTE[0]

// lock PTE[0] → PTE[0] = KVM_INVALID_PTE_LOCKED
if (!stage2_try_set_pte(ctx, KVM_INVALID_PTE_LOCKED))
    return -EAGAIN;

// for 循环：ptep 的 ++ 在循环体之后执行
for (i = 1; i < CONT_PTES; i++, ptep++) {
    WARN_ON_ONCE(stage2_pte_is_locked(*ptep));
    // 第一次迭代: ptep 仍是 PTE[0]，刚被自己锁了！
    // 第二次迭代: ptep = PTE[1] (正确)
    // ...
    // 第15次迭代: ptep = PTE[14] (正确)
    // 循环结束: i=16, PTE[15] 从未被检查/清除！
```

**这不是并发竞态——是 off-by-one 索引错误。**

- `ptep++` 在 for 循环末尾执行，第一次进入循环体时 `ptep` 仍是初始值 PTE[0]
- PTE[0] 已被本函数在第 1094 行锁定，`stage2_pte_is_locked(PTE[0])` 必然为真
- 每次 contig map 调用都会触发此条件，只是 WARN_ON_ONCE 仅报一次
- 同时 PTE[15] 从未被清除循环处理，存在打破 BBM（Break-Before-Make）协议的风险

## 修复

一行修改：将 `ptep++` 提前到循环体之前。

```diff
- for (i = 1; i < CONT_PTES; i++, ptep++) {
+ for (ptep++, i = 1; i < CONT_PTES; i++, ptep++) {
      WARN_ON_ONCE(stage2_pte_is_locked(*ptep));
```

修复后循环从 PTE[1] 开始，正确检查 PTE[1..15]，不再检测自己的 PTE[0] 锁。

## 错误的分析路径

调试过程中走了弯路的错误假设：

1. **"PTE[0]-as-block-lock 与单页 BBM 存在竞态"** — 基于 WARN 触发，假设锁来自于其他线程，设计了 lock-then-check 修复方案
2. **"spin 等待可解决竞态"** — 试图在清除循环中 spin 等待 LOCKED PTE 解锁，但 TCG 模拟下 spin 不生效
3. **"抢占导致并发"** — 怀疑 CONFIG_PREEMPT 导致读锁持有者被抢占从而产生竞态窗口

最终发现 WARN 检查的 PTE[0] 是被自己锁的，所有这些分析方向都是多余的。

## 教训

1. **先检查循环边界条件** — 怀疑并发之前，先确认代码本身逻辑是否正确
2. **WARN_ON_ONCE 掩盖了问题频率** — 如果用的是 WARN_ON（非 _ONCE），第一次触发就能看到每次 contig map 都会触发，更容易发现是自锁问题
3. **attr contig 路径没有此 bug** — `stage2_attr_contig_unaligned/aligned` 用的是 `ptep = first_ptep + 1`，起始位置正确

## 提交

```
c69b9578f4b1 KVM: arm64: Fix off-by-one in contig BBM clear loop
```

一行修复：将 `ptep++` 从 for 增量表达式提前到初始化表达式。
