#!/bin/bash
ROOTFS=/usr/local/data/project/rootfs
LINUX_KERNEL_VERSION=3.14.5

qemu-system-x86_64 -hda ${ROOTFS}/rootfs.img -kernel ${ROOTFS}/resources/linux-${LINUX_KERNEL_VERSION}/arch/x86/boot/bzImage -append "root=/dev/sda rw rootfstype=ext4 console=ttyS0" -nographic