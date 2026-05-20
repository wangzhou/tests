# ARM KVM Stage-2 Contig Bit 调试环境使用指南

## 架构

```
物理机 (ARM64 aarch64)
  └── L1 QEMU (TCG 模拟, run_qemu_qcow2_src.sh)
        ├── openEuler 24.03 LTS SP2, 6G RAM, 4 SMP
        ├── default_hugepagesz=64K
        └── L2 QEMU (KVM 加速, src.sh)
              └── 128MB, -mem-path /dev/hugepages
```

## 文件布局

| 位置 | 内容 |
|------|------|
| `./run_qemu_qcow2_src.sh` | L1 启动脚本 |
| `./share/` | 9p 共享，L1 内自动挂载到 `/mnt` |
| `/home/wz/linux/arch/arm64/boot/Image` | L1 内核 |

L1 内部 (`/root/`)：`qemu-system-aarch64`, `Image`, `rootfs_mini.cpio.gz`, `src.sh`, `src_prepare.sh`

## 调试步骤

### 1. 构建内核

```bash
cd /home/wz/linux
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- Image -j$(nproc)
```

### 2. 启动 L1 (tmux 后台)

```bash
tmux kill-session -t qemu_l1 2>/dev/null
tmux new-session -d -s qemu_l1 \
  "cd /home/wz/tests/qemu_debug_qcow2 && bash run_qemu_qcow2_src.sh 2>&1 | tee /tmp/qemu_l1.log"
```

等待约 30s 到登录提示符，查看进度：

```bash
tmux capture-pane -t qemu_l1 -p | tail -20
```

### 3. 登录 L1

```bash
tmux send-keys -t qemu_l1 "root" Enter
sleep 1
tmux send-keys -t qemu_l1 "wangroot" Enter
```

### 4. 检查/运行 prepare

```bash
tmux send-keys -t qemu_l1 "cat /proc/meminfo | grep Huge" Enter
tmux send-keys -t qemu_l1 "cd /mnt && ./src_prepare.sh" Enter
```

### 5. 启动 L2

```bash
# 必须 < /dev/null 避免后台 SIGTTIN
tmux send-keys -t qemu_l1 "cd /root && ./src.sh < /dev/null &" Enter
```

### 6. 检查 WARN_ON

```bash
tmux send-keys -t qemu_l1 "dmesg | grep -i 'warn\|Call trace\|pgtable.c'" Enter
sleep 1
tmux capture-pane -t qemu_l1 -p | tail -30
```

### 7. 清理

```bash
tmux kill-session -t qemu_l1
```

## 注意事项

- L2 内存由 64K 大页支持，触发 stage-2 contig 路径
- L2 启动必须 `< /dev/null` 重定向 stdin，否则后台运行收到 SIGTTIN 停止
- 查看 dmesg 需要在 L1 shell 内执行，不会自动出现在 tmux 面板
- L1/L2 共享 tmux 面板显示，L2 崩溃输出也会出现在 L1 串口日志中
