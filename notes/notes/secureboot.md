Meme for grub/boot process for dual boot for Archlinux and Ubuntu.

Archlinux is less patch from upstream, it is good dependency check and no extra modification.
However, commerical software XMind or other software basically are targeting to Ubuntu.

Since each open sources are parts or components, as computer system, Ubuntu has most users and de-fact configurations.
To minimize the impact of Ubuntu Installer (https://wiki.ubuntu.com/Ubiquity), I need to understand what Ubuntu/Debian patched Grub

You can search the patch files for Grub2 in https://launchpad.net/ubuntu/+source/grub2

1. ~~First of all, Ubuntu/Debian had never accepted upstream 2.06, stop in 2.04
I still don't know the reason.~~

Ubuntu 22.04 and Debian bullseye moved to 2.06

As of 20.10 Ubuntu, there are 110 patches !!??? This proves that FSF/GNU actually doesn't accept patches from Canonical lol 

One of the most import patch is set default prefix folder in EFI directory in EFS partition.Since, each vendor? (even Microsoft) makes their own folder. so ubuntu set default grub's prefix path to 'ubuntu' (of course), if you use Ubuntu's grub, you have to put the first grub.cfg in 'ubuntu'

2. In UEFI, EFS filesystem will be fat32(in default). You can put the kernel image and initramfs image in EFS. However. long long age, Debian(Ubuntu) decided that since linux or initramfs heavily depends on the kernel version, when the kernel version updates, they want to keep old versions also, so they will use the image file name with the version number. Then how we know which version is default, well they decided to use __Symbolic Link__. __FAT32 has no Symbolic Link__. So in the default, you cannot put kernel and initramfs image in EFS for Debian(Ubuntu)


So I ended up, I moved only grub (module) folder to EFS and manually made (EFS)/EFI/Ubuntu directory and put grub.cfg which will load a different grub.cfg in a different path. This one changes prefix to /boot/grub then indicate real grub.cfg which has boot entry and then, replace upstream grubx64.efi to Ubunut's grubx64.efi

And then, mount /dev/sda1 /mnt/EFS , then mount --bind /mnt/EFS/grub /boot/grub in Ubuntu

NOTE: Why I did not create a simple boot partition with ext2 for /boot ?

NOTE2: I cannot map the EFS partition in /boot/grub directly? 

NOTE3: Default bootx64.efi, it will be /EFI/Boot/bootx64.efi, but we can choose in BIOS menu?

NOTE4: I could not find an easy way to share boot/grub settings between two different Linux distributors. Since they will overwrite each others and somehow somewhere, chain from the default boot efi file (EFI/Boot/bootx64.efi -> not necessary boot folder)

Memo secureboot archlinux

Archlinux does not have their/Microsoft shim signed binary.
So I need to download either Debian or Ubuntu shim signed binary.

The problem is that some shim signed binaries can disable for MOK(machine owner key) mechanism in default seemingly.
In that case, I cannot add my machine owner key.

Since Archlinux can use systemd-bootx64.efi (simple efi bootloader in a part of systemd) to boot linux without multi-bootloader like Grub2

1. Download shim-signed and mmx64.efi in the same version
2. Copy or rename systemd-bootx64.efi to grubx64.efi
3. Create my machine owner key. (copy and paste from archlinux wiki)
```
$ openssl req -newkey rsa:4096 -nodes -keyout MOK.key -new -x509 -sha256 -days 3650 -subj "/CN=my Machine Owner Key/" -out MOK.crt
$ openssl x509 -outform DER -in MOK.crt -out MOK.cer
```
4. signing
```
# sbsign --key MOK.key --cert MOK.crt --output /boot/vmlinuz-linux /boot/vmlinuz-linux
# sbsign --key MOK.key --cert MOK.crt --output /boot/EFI/Boot/grubx64.efi /boot/EFI/Boot/grubx64.efi
```
4.1 Copy the cer file to /boot/EFI any place
```
# cp MOK.cer /boot/EFI/Boot 
```
5. reboot and kick mmx64.efi (MokManager)

NOTE: When I tried this, in unknown reason, mmx64.efi did not lunch (LoadImage error), so I switch off SecureBoot,
rename temporarily from mmx64.efi to grubx64.efi then, it works. This is not supposed to be, but I don't know.

6. In MokManager, select Enroll key(Certificate), and select the cer file which was copied in Step4.1 and then reboot

NOTE2: I actually renamed bootx64.efi to grubx64.efi, but it looks the that boox64.efi was the same as systemd-bootx64.efi
