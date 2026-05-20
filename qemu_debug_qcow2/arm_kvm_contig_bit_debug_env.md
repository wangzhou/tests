
介绍下调试arm kvm stage2 contig bit的环境

1. 没有实体的单板，我们用qemu tcg的模拟环境，这个我们叫L1系统，L1系统模拟host系统。
   L1的启动脚本是: run_qemu_qcow2_src.sh, run_qemu_qcow2_dst.sh，做热迁移模拟测试
   的时候需要起两个host，但是普通起虚机就启动run_qemu_qcow2_src.sh就可以。

   L1系统的用户名/密码是：root/wangroot

2. 启动L1系统后，在root的home目录有，启动内层虚机的脚本、内核Image和小内存文件系统。
   我们叫内存虚机为L2。

   run_qemu_qcow2_src.sh系统的L2启动脚本是src.sh
   run_qemu_qcow2_dst.sh系统的L2启动脚本是dst.sh

3. 测试这个特性，我们需要在配置L1系统的default大页是64K hugetlb，这个在L1启动脚本
   里已经有了。需要在L1系统中写sysfs文件预留大页，这个已经写到src_prepare.sh/dst_prepare.sh
   脚本里了，prepare脚本还做了一些debug和网络的配置，你在运行L2系统之前先跑下prepare
   脚本就好。

4. 注意，遇到任何问题，不要直接修改测试脚本。


