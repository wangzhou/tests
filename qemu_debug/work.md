任务描述
=========

总体目标
---------

根据现有的dma engine linux内核驱动，反向生成对应的芯片手册。根据对应的芯片手册编
写对应的qemu模型，编写好qemu模型后，可以使用现有的dma linux内核驱动迭代调试这个
模型，直到这个qemu模型可以正常工作。

相关代码和工具路径
-------------------

linux代码路径：/home/wz/linux
dma engine内核驱动路径：/home/wz/linux/drivers/dma/hisi_dma.c
qemu代码路径：/home/wz/qemu

编译/home/wz/qemu, 编译好的qemu在/home/wz/qemu/build/qemu-system-aarch64。
启动qemu的命令可以参考：/home/wz/tests/qemu_debug/kvm_s2_cont.sh
启动qemu的小文件系统可以使用：/home/wz/tests/qemu_debug/rootfs_sshd_new.cpio.gz

工作步骤
----------
1. 先根据dma engine内核驱动生成芯片手册。输出: DMA芯片手册.md。
2. 根据如上DMA芯片手册.md编写qemu模型。checkout一个开发分支出来，保留每次git提交。
3. 编译带这个dma engine驱动的linux内核Image。(只编译一次就好)
4. 编译带这个dma engine模型的qemu。
5. 调试这个dma engine模型，测试代码可以用内核自带的dma engine自测试程序。
6. 反复自动迭代如上步骤，直到通过内核的dma engine自测试程序。
