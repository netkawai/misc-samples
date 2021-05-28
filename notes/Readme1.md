https://www.epa.gov/sites/production/files/2013-09/version30readme_0.txt
multiple public domain system components (FLEE equation parser, Accord.Net math libraries, R statistical libraries, DotSpatial mapping libraries, 
Weifen Luo Docking UI, ZedGraph, and GMap.Net) public domain??
VB3.0??



The maximum size for each file is 4 MB (four megabytes). We accept these file formats:
PDF (Portable Document)
JPG, TIFF, or PNG (Image)
DOC or DOCX (Microsoft Word Document.
If your file is bigger than 4 MB, you should reduce the size. Here are some tips for making reducing your file size.

To move Gentoo partition(this is relady done)


1. resize boot partition.

1.1 boot gentoo.
1.2 copy whole files. unmount /boot delete partition 4
    recreate partition.. be careful calculation for start (1GB)
    mkfs.ex2, copy back to file and reboot and check it.
    in this partiion, copy stage2 from cd with kernel and others.
    if I want to change the distructive modification for partition of sda3, I will use this.

2. reduce the size of partition3, boot gentoo-mini. and delete partition 3
   the start position of partiti3 must be the same. the size
   check pvsize first, if pvsize is big, fist reduce the pvsize before chaning partition.
   
3. move date partition3, should I use gparted? or anyother disk? or minipartition???


Another partition move.
Delete the primary and if we can add GPT partition

Make two partitions for Allocate block EFI(future/200MB) and Main(around 200GB future for Windows)
Make ext4 partition in Main and copy the all vlroot and sda6
and boot with vlroot delete extended partition and make two primary partion Linux Main(180GB) and boot(20GB)

try to compile Chromium without Xlib if, emerge --ask www-client/chromium ask for Xlib,
try not to install java, try to install font manually.

https://chromium.googlesource.com/chromium/src/+/master/docs/linux_build_instructions.md

Right now, I can compile webkit-gtk to remove webgl and libnotify without X.
Chromium does not have option without X gtk. I am not sure what is different.
 
Blink vs Webkit2 (2013)


1. resize boot partition.

1.1 boot gentoo.
1.2 copy whole files. unmount /boot delete partition 4
    recreate partition.. be careful calculation for start (650MB)
	mkfs.ex2, copy back to file and reboot and check it.

2. reduce the size of partition3, boot gentoo-mini. and delete partition 3
   the start position of partiti3 must be the same. the size
   check pvsize first, if pvsize is big, fist reduce the pvsize before chaning partition.
   
3. move date partition3, should I use gparted? or anyother disk? or minipartition???



Hudson(Jenkins) is a open-source to help automation build and smartphone.


If I knew that OpenGLES2 was already adopted as Unified sharder as well as programmable,
I could modernirize UI scheme and caption engine, and it is better to support OCAP system,
and utilize and emphasize the UI and TV basic functionality for color adjustment as well.

Nehalm does not have unrestricted mode for VT-x

kernel parameter passwd=root
To avoid memory corruption above 4GB and plus graphics memory of 1GB.
/dev/sda3 is LVM
/dev/sda4 is ext2


grub rescue> set prefix=(hd0,6)/grub
grub rescue> set root=(hd0,6)
grub rescue> insmod normal
grub rescue> normal

grub>linux (hd0,6)/kernel dolvm root=/dev/vg1/lvroot
grub>initrd (hd0,6)/initramfs
grub>boot
booting

GPT
grub>linux (hd0,gpt6)/linux root=/dev/sda5
grub>initrd (hd0,gpt6)/initramfs.img


bcdedit set device 

X.org.log

DRI2 modules
DRI3 enabled <- OK

vblank_mode=0 LIBGL_DEBUG=verbose glxgears
FPS is around 2000 FPS.

$ watch -n 1 'ps -eo pid,ppid,cmd,%mem,%cpu --sort=-%cpu | head'

bcdedit /create /d grub /application bootsector
bcdedit {ID} /set device   partition=C:
bcdedit {ID} /set path     \boot.bin

grub(i386-pc) uses the below 4 real mode INT

Internal INT 0H-05H(8086)
0: Divide Error
1: Single Step
2: NMI
3: Breakpoint
4: Overflow


BIOS set default IRQ0-7 mapping to from INT 08H to INT 0FH

INT 10H( Video/console)
INT 13H( Disk )
INT 15H( Device information)
INT 16H( Keyboard )

FD(1.44MB 2 heads, 80 tracks/cylinders, 18 sectors)

MBR is only 512byte, however , GPT can start from LBA 34(0-33) in 512 sector size, 
"The UEFI specification stipulates that a minimum of 16,384 bytes(16KB), regardless of sector size, 
be allocated for the Partition Entry Array."  
Old tools/fdisk in IBM PCXT/DOS used INT 13h BIOS 
The partition can only start from the head of cylinder. At the time one cylinder had 63 sectors(LBA: 0-62)
The first partition start from sector63 (CHS =(1:0:0))
62 sectors * 512 byte = 31KB

In this situation, even the MBR(bootloder is 512byte) but still another 31KB space is available in the beginning of HDD.
Grub2 can store loader for core.img(448KB) + core.img(<31KB). This program should contain filesystem to access the file.
 
Recent tools create first primary partion from 2048(LBA) sector.
The modern tool are following 1MB partition alignment, when users install a new OS to be ready to convert GPT.
because microsoft utility still use head 4KB boundary in  file cluster and 1MB partition alignment,

But grub assumes that the core bootloader use tiny(COM) model which is CS=DS=SS in 64KB,
they load 0x8000 offset, the remain is should be less than 32KB, 

i386-pc the kernel.img of grub contains general functions for bios(int) and real2proto, proto2real.
User can build a custom image which use can specify some paticular modules such ext2 filesystem or msdos_part.

Grub on i386-pc heavily rely on real mode BIOS call to archieve the basic function 
to avoid the writing native drivers for all aspects
Moreover, mkimage limits the size of core image to 960 sectors for real hardware, 832 sectors for qemu(coreboot).
(it assumes 512 bytes per sector) and in historical reasons
grub limit that the maximum size 0x78000 = 480KB??? = 960 sectors(??)
or let's say second booting image which directly is loaded from MBR without filesystem.

Grub is strictly a boot loader anything not more than that.

Bootstrap address
0000H:7C00H (offset: 32KB - 1KB(MBR bootstrap)) original IBM PC with DOC should have minimum 32KB memory for BIOS.
0000H:6000H (copy lnxboot.S, diskboot.S and startup_raw.S)
0800H:0000H (original offset[32KB] of core.img, 8000H-8200H diskboot.S(512Byte) is not used)
0000H:8200H (GRUB_BOOT_MACHINE_KERNEL_ADDR + 0x200:Startup_raw.S in core.img)

The transition of real->protect is in Startup_Raw.S
bootMgr can use the boot image of (lnxboot.img + core.img)
boot/lnxboot.S(1KB)[7C00H] -> boot/startup_raw.S(gateA20)[8200H]  -> kern/startup.S -> kern/main.c
and display "Welcome grub!"
If grub can not find device or normal.mod in prefix and root, it enters into rescue mode to limit the usage and command.

In protected mode, grub disables any interrupts to avoid crash with cli.
then whenever grub needs to call BIOS int call, grub switch back to real mode , 
then allow interrupts including hardware interrupts.
In protected mode, grub uses a single inifinitive loop to poll keyboard I/O port, timer I/O port.

0x00000-0x003FF - Real Mode Interrupt Vector Table
0x00400-0x004FF - BIOS Data Area

0xA0000-0xAFFFF(VGA/EGA)
0xB0000-0xB7FFF(MDA:Text VideoRAM:32KB)
0xB8000-0xBFFFF(CGA:Text VideoRAM:32KB)
0xC0000-0xC7FFF(VideoROM:32KB)
0xC8000-0xDFFFF(RAM disk or EMS , Video ROM option, BIOS Shadow Area)
0xE0000-0xFFFFF(BIOS ROM:128KB)
the area of 0xC8000-0xFFFFF depends on system 

VGA Text(9x16,9th column is for space)=(9x80),(16x25)=(720,400)pixel
        (8x16)=(640,480)
VGA-ROM.F16
second character(8x16){u+263A}
00000000
00000000
01111110
10000001
10100101
10000001
10000001
10111101
10011001
10011001
10011001
01111110

00000000
00011000
00111100
00111100
00111100
00011000
00011000
00011000
00000000
00011000
00011000
00000000
00000000

KANA16
00010000
00010000
00010000
00011110
00010010
00100010
00110010
01001100
00000100
00000100
00001000
00001000
00010000
00100000
01000000
00000000
