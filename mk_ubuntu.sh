#! /bin/bash

make mrproper

make meson8_x11_defconfig

make uImage -j2
make modules -j2

if [ -d ./deploy ]; then
	rm -rf ./deploy
fi

mkdir deploy

make modules_install INSTALL_MOD_PATH=./deploy

make meson8_dbximx8.dtd
make meson8_dbximx8.dtb

make dtd

#cd ../root/g18
#find .| cpio -o -H newc | gzip -9 > ../ramdisk.img

#rootfs.cpio -- original buildroot rootfs, busybox
#m8rootfs.cpio -- build from buildroot
ROOTFS="rootfs.cpio"

#cd ..
./mkbootimg --kernel ./arch/arm/boot/uImage --ramdisk ./${ROOTFS} --second ./arch/arm/boot/dts/amlogic/meson8_dbximx8.dtb --output ./boot.img
ls -l ./boot.img
echo "boot.img done"
