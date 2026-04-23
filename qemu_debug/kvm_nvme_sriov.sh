#!/bin/sh
#
# NVMe SR-IOV 测试脚本
#
# 关键参数说明:
#   - nvme-subsys: NVMe 子系统,SR-IOV 必须配合此设备使用
#   - sriov_max_vfs: PF 支持的最大 VF 数量
#   - sriov_vq_flexible: VF 可用的灵活 Queue Pair 数量 (SQ/CQ)
#   - sriov_vi_flexible: VF 可用的灵活 Interrupt Vector 数量
#   - subsys: 关联的 NVMe 子系统 ID
#
# 启动后可通过 lspci 查看 PF/VF 配置,通过修改 NUM_VF 启用 VF

/home/wz/qemu/build/qemu-system-aarch64 \
	-m 4G \
	-smp 4 \
	-cpu max \
	-machine virt,gic-version=3,acpi=on,virtualization=true \
 	-append "console=ttyAMA0 root=/dev/ram init=/init default_hugepagesz=64K" \
	-nographic \
	-kernel /home/wz/linux/arch/arm64/boot/Image \
	-initrd /home/wz/tests/qemu_debug/rootfs_sshd_new.cpio.gz \
	-device virtio-9p-pci,fsdev=p9fs,mount_tag=p9,bus=pcie.0 \
	-fsdev local,id=p9fs,path=./kvm_s2_test,security_model=mapped \
	-device nvme-subsys,id=subsys0,nqn=test-nqn \
	-device nvme,id=nvme0,serial=1234,sriov_max_vfs=4,sriov_vq_flexible=8,sriov_vi_flexible=4,subsys=subsys0,bus=pcie.0