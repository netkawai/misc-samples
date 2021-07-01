Memo secureboot archlinux

Archlinux does not have their/Microsoft shim signed binary.
So I need to download either Debian or Ubuntu shim signed binary.

The problem is that some shim signed binaries can be disabled for MOK(machine owner key) mechanism in default.
In this case, I cannnot add my self-signed key. but I am not sure how MOK does work still.

Since I use systemd-bootx64.efi (I mean neither ELILO or Grub2), to boot linux without locking, it looks ok.

1. Download shim-signed and mmx64.efi in the same version
2. Copy or rename systemd-bootx64.efi to grubx64.efi
3. Create my machine owner key. (copy and paste from archlinux wiki)

$ openssl req -newkey rsa:4096 -nodes -keyout MOK.key -new -x509 -sha256 -days 3650 -subj "/CN=my Machine Owner Key/" -out MOK.crt
$ openssl x509 -outform DER -in MOK.crt -out MOK.cer

4. signing

# sbsign --key MOK.key --cert MOK.crt --output /boot/vmlinuz-linux /boot/vmlinuz-linux
# sbsign --key MOK.key --cert MOK.crt --output /boot/EFI/Boot/grubx64.efi /boot/EFI/Boot/grubx64.efi

4.1 Copy the cer file to /boot/EFI any place
# cp MOK.cer /boot/EFI/Boot 

5. reboot and kick mmx64.efi (MokManager)

NOTE: When I tried this, in unknown reason, mmx64.efi did not lunch (LoadImage error), so I switch off SecureBoot,
rename temporarily from mmx64.efi to grubx64.efi then, it works. This is not supposed to be, but I don't know.

6. In MokManager, select Enroll key(Certificate), and select the cer file which was copied in Step4.1 and then reboot

NOTE2: I actually renamed bootx64.efi to grubx64.efi, but it looks the that boox64.efi was the same as systemd-bootx64.efi
