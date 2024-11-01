# Start without graphics and network.
# at least I saw kernel boot message, but it stuck on after mounted rootfs... 
/home/kawai/qemu-poky/poky/qemuarm/tmp/work/x86_64-linux/qemu-helper-native/1.0-r1/recipe-sysroot-native/usr/bin/qemu-system-arm \
-object rng-random,filename=/dev/urandom,id=rng0 \
-device virtio-rng-pci,rng=rng0 \
-drive id=disk0,file=/home/kawai/qemu-poky/poky/qemuarm/tmp/deploy/images/qemuarm/core-image-minimal-qemuarm-20241101191919.rootfs.ext4,if=none,format=raw \
-device virtio-blk-device,drive=disk0 \
-device qemu-xhci \
 -machine virt,highmem=off \
 -cpu cortex-a15 -smp 4 -m 256 -nographic \
 -kernel /home/kawai/qemu-poky/poky/qemuarm/tmp/deploy/images/qemuarm/zImage \
 -append 'root=/dev/vda rw  mem=256M  vmalloc=256 console=ttyAMA0,115200'
