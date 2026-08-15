History of IBM/PC   
   
Pins of a latest Intel Atom in 2011   
DDR2/SVDO/LVDS/4 channels of PCIe/SPI/SMBus/LPC   
http://download.intel.com/embedded/processor/datasheet/324208.pdf   
Intel Atom E6xx Series   
This chips has built-in compatible IBM PC devices(8259, 8254, 8250, RT-CMOS).   
Intel 8086  1978  release   
IBM PC      CPU 8088   128KB:RAM  NONE:HDD FDD:360KB  CGA/MDA 1981   
IBM PC/XT CPU 8088   256KB:RAM 20MB:HDD FDD:360KB  CGA/MDA 1983   
Intel 80286 1982 release   
IBM PC/AT CPU 80286 512KB:RAM 20MB:HDD  FDD:360KB/1.2MB EGA 1984   
IBM S229-9611, S229-9608   
Technical Reference Personal Computer AT, (IBM S229-9611, S229-9608)   
http://www.minuszerodegrees.net/manuals.htm#IBM   
Intel 80386 1986 reelease   
IBM PS/2 CPU 80386 2MB:RAM 30MB:HDD FDD:720KB/1.44MB 1987 VGA   
IBM Personal System/2 and Personal Computer BIOS Interface Technical Reference, (IBM S68X-2341)   
Personal System/2 Hardware Interface Technical Reference – AT Bus System,(IBM S85F-1646)   
Intel 80486  1990 release (add atomic operations and A20 line)   
UNIX 1970 1972 C Language UNIX   
MINI COMPUTER(PDP-11:16bit) UNIX 6th Edition   
1975 1978 UNIX 7th Edition   
PDP VAX(32bit) UNIX32/V)   
USL, Berckley ,Sun Microsystems   
IBM PC 1981 SCO/Microsoft ported1983   
Mr.Andrew S. Tanenbaum 1987 MINIX   
1991 Linus Linux 0.01 News group of minix Had been POSTed   
1991 BSD4.3 NET/2 AT&T Olitz   
386BSD 0.1 4.4BSD-Lite USL License   
BSD derived OS   
AT&T Linux   
UNIX open source kernel   
From 16bit to 32bit Unix OS/High level language   
1970s Minicomputer   
1970s PDP-11(16bit)/VAV-11   
LISP/UNIX/C Language Ethernet/IPv4   
These original ideas 1960s IBM Mainframe System360   
System360 is a Mainframe as defacto as IBM)   
80386 was introudced orthogonal, register beame as more general register not a paticular for instruction   
Add FS,GS as segment register   
IBM PC 1981 hit in small business   
Cheap $2000 keyboard was realiable   
The first generation of IBM PC, PC/XT, PC/AT was sold,   
They did not have inter-compatiblity of hardware which includes DISK controller. If programers did not use system call(software interrupt) provided by   
BIOS/DOS, user should select the correct hardware driver version and component.   
IBM introuced the second generation PS/2, shift licensed non-open Platform.   
However, PC market required the compablity with PC/AT, as standard platform PC which was established as Industrial Standard.   
PC/AT compatible platform intorduced prephieral of 3.5inch FDD(720KB), PS/2 AUX(Mouse), VGA] for increametal   
Functionalities. CPU becamse from 80286 to 80386SX, non-IBM braned IBM PC compatible PC became the   
Standard PC includes Qemu/VirtualBox   
The memory map of Intel CPU(x86,64)   
Logical address -->(segement)-->Linear address--(page transition)-->physical address   
The CPU has two Segment types (Data or Code etc) Descriptor in   
Global Descriptor Table(GDT) and Local Descriptor Table(LDT)   
The structure of segment Selector(CS/DS/SS/ES/FS/GS) is   
13bit            1bit 2bit   
(Selector)(T1) (privilege level)   
And T1=0 is GDT, T1=1 is LDT to indicate each descriptor table   
16bit Protect Mode in 80286 has logically the address of13bit + 16bit = 29bit/512MB   
If Both of GDT and LDT are considered separate address space, the CPU of logical   
Address space is 1G,   
GDT(Global Description Table) register should be set to address(16bit or 32bit)  which has the base address and length of array for GDT memory area, the register has called shadow register for storing the base address and length of GDT. That is why, GDTR is 16bit 4KB boundary.   
Descriptor Entry structure   
20bit(limit) + 32bit(base linear address)+12bit(Descirptor information)    = 1entry(8byte) .   
1entry is 8byte, GDT has 13bit => the maximu size of table is 8191 (8*8191 = 63KB)   
The protect mode in 80286 compatible mode cae use base linear whtin 20bit, limit is within 16bit   
This is controlled by  G bit in descriptor. This mode has compatibility with 80286   
The other register apart from 6 general registers are  IP,SP,BP, which become 16bit(Win16).   
The bit width of general registers are indicated by prefix, which was concept to add FPS instructions and   
extended instruction architecture.   
User can define LDT descript in the 8191 GDTs used by LDTR   
(Is this option? Not mondatory)   
LDT Descriptor   
20bit(limit) + 32bit(base linear address)+type(8bit)+ 0 (4bit)   
The entry sturecutre is the same, LDTR hold selector value which indicate   
which GDT has the segment for LDT.   
(least 3 bit is ignored in 16bit)   
Indeed, GDT can hld multiple LDT Descriptors. System software can change LDTR   
Even though CS, DS, ES has the same selector value, software can access the different physical memory address   
I still do not investigate that Windows 3.0/3.1 used this mechanism for local heap??   
IDT register is set to indicate the head of arrary of Interrupt descriptor tables   
IDT(Interrupt Description Table) register   
IDT table entry structure   
32bit(offset addr) , selector(16bit), (4bit)copy count, type(8bit), 0(4bit) = 1entry(8byte)   
INT instruction use 8it, IDT tables defined upto 256 (256*8 = 2KB)   
System register is the  only one set, even though dual/quard CPU(SMP)   
If system will load the selector into segment register, CPU has hidden registers to load   
Offset address as well as full descriptor information.   
Type of Global Descriptor/Interrupt Descriptor   
1  Active 16bit TSS   
2  LDT   
3  Busy 16bit TSS   
4  16bit call gate   
5  task gate   
6  16bit interrupt gate   
7  16bit trap gate   
8  reserve   
9  Active 32bit TSS   
10 reserve   
11 busy 32bit TSS   
12 32bit call gate   
13 reserve   
14 32bit interrupt gate   
15 32bit trap gate   
In x86, system can use Interrupt(INT) as system call which are defined in RISC for independent instuction.   
System software has to set Interrupt type for interrupt   
Task State Segment(TSS) was used in NTVM only to run DOS/WIN32 applications   
AMD64 does not support TSS.   
Linux2.0/2.2/2.4 did use TSS to implement task switch , however, Linux2.6 or the above do not use TSS.   
However Linux still uses to switch STACK for interrupt(software and hardware) Linux set 1TSS for 1CPU   
In dual core, linux set 2TSS. The system register set is one, however dual core can run simulaneously different interrupt task.   
Olitz wrote 386BSD book(The Basic Kernel Source Code Secrets). They mentioned the issue of TSS   
That is slow due to load total 104 byte memory for task context switch   
Kernel(System) address space does not have the information of kernel stack and stack status.   
All stack of each process should be shared within the same virtual(Linear) address.   
System can not create the nested thread within multiple process   
When some process will exit, system can not restore context from the process which will exit.   
However, TSS only can set EFLAG to set v86 flag to enter VM86 mode.   
Moreover, TSS can set CR3 to restore page directory entry to switch physical memory space   
against the same linear address(0x00000 - 0x10FFFE) between multi-real mode environment(specifically DOS).   
The standard mode and enchanced mode of Windows 3.0 is almost identical except memory handle and dos prompt.   
The standard mode can not handle multi-dos instance, which means the support only single dos (full screen mode).   
The enhacned mode support multi-dos instance with window, because os can utilize the paging feature of 386.   
And In my guessing, OS use 386 TSS for vm86 and 286 TSS to emulate standard mode in 386.   
Microsoft might not use 286 TSS in the standard mode for task switching. In the case, most probably,   
OS can not do preemptive switch between win16.   
There would be one 286 TSS for win16 apps, multiple 386 for dos instances   
The linear address of win16 apps were shared, no segregation.   
If OS specify all port map, the size of a TSS for v86 is about 4byte*64 + 256bit(8byte)+1byte=113byte   
Page Table Entry   
System should set the top 20bit address of CR3 to indiecate Page Entry Directory   
The MSB of 10bit in logical address(Linear address) +Page Entry Directory + the address of Page Entry   
The linear  of 10bit from Page Entry can indicate a page to decide an address.   
10bit + 10bit + 12bit   
For v86 mode, page directory is not required. OS can assume 0 in the directory.   
10bit~1024->1KB memory require for page management.+TSS ~ 2KB   
64KB(GDT+LDT)+IDT(64KB)+64KB(V86+PAGE+TSS)   
In Windows, however, Microsoft did not use 286TSS for task switching and memory separation.   
In my guessing, one is performance and another is compatibility.   
They  implemented Windows 1.0 in real mode. In real mode,   
CPU does not have any facility to separate memory space.   
In Windows 2.0, they started to support 286 protect mode to break the barrier of 640KB.   
However, they had already implemented DLL(shared object library) on assuming that   
all application can share memory space.   
Unlike paging, segment based memory swap such as LDT was too complicated   
to co-existent the separate and sharing areas in memory   
Microsoft simply gave up to separate application memory space in 80286(16bit protected mode).   
This decision make them keep easy to share the same code base between 286(Standard) and 386(Enchanced)   
except handle of DOS instances.   
Moreover, TSS for 286 was too heavy to utilize task switching at the time.   
Microsoft simply introduced cooperative multi-task.   
All win16 apps had to share  memory space and cooperative multi-task on task switching in GetMessage/PeekMessage/WaitEvent   
to kick reschedule task   
In DOS instances in enhanced mode, they only used native 386TSS and paging for lower-memory(1MB)   
Real Mode(8086 mode)   
If the mask of A20M# enables, the above 1MB was masked (8086)   
However,  the standard of APCI require to disable A20M#   
Intel CPU has three types of instructions   
Memory Space   
I/O Space   
MSR Space   
APIC requires hareware register in physical address of 0x000FEE00-0x000FFFFF   
This range of address in IBM PC/XT(8086) was the reset vector.   
80286 or the above CPU does not use this range. The above 03386 reset into  0xFFFFFFF0 in IP register.   
The CS selector register has hidden register to keep the address offset for code segment(0xFFFF).   
The reset value of EIP register offset has 0x0000FFF0 and CS + EIP is 0xFFFFFFF0   
Till system load cs selector value into the register, system execute far jump or far call,   
The segement of address range of 0xFFFF0000-0xFFFFFFFF(64KB) was shared among code and data   
Intel softare maual has a piece of code to switch real mode to protect mode.   
BIOS can access whole physical address after swithced Protect Mode, do check memory and   
Initialize chipset, register ACPI service as PC/AT, initialize VGA(Graphic)   
In my guessing, BIOS execute and issue the reuqest of initialization of PCI Bus Device.   
If BIOS has found VGA PCI Card, BIOS expect this PCI Card required VGA compatilibity with IO and memory space   
System BIOS initalize TEXT MODE(80x25)   
In legacy algorithm, BIOS search VIDEO BIOS in the address of 0xC0000.   
After initalize, system keep Boot Block, switch into Real Mode, far jump to Boot Block   
After switched Real Mode, APCI require  legacy compability with PC/AT BIOS service.   
The first generation.   
1981:IBM PC    CPU 8088 128KB:RAM NONE:HDD MDA   
1983:IBM PC/XT CPU 8088 256KB:RAM 10MB:HDD  MDA\CGA FDD:5.25inch(2DD)   
1984:IBM PC/AT CPU 80286 512KB:RAM 20MB:HDD  EGA FDD:5.25inch(2DD/2HC)   
The second generation.   
1986:Compaq Deskpro CPU i386DX 512KB:RAM 20MB:HDD EGA   
(Split CPU BUS from ISA BUS)   
1987:IBM PS/2   MODEL30 CPU 80286 1MB:RAM 29MB:HDD  VGA(160KB) FDD:3.5inch   
1988: Intel i386SX released   
1989:ISA-compatible PC CPU i386SX 2MB:RAM 40MB:HDD  VGA+(512KB)   
1989: Intel i486DX released   
PC-AT compability (software and extend card)   
Maximun RAM memory in 80286(PCAT) is the same16MB(24bit)   
The below address of 1MB Address are similar to IBM PC for 640KB for user rAM   
PC/XT has MDA/CGA  BIOS. EGA/VGA card had their own BIOS in the card placed in   
0xC0000-0xCFFFF   
Linus bought PC which of spec was 386-compatible PC CPU 80386SX 4MB:RAM 120MB:HDD in 1991   
In MS-DOS, Microsfot introduced HIMEM.SYS/EMM386.EXE on Windows 3.0.   
They can use high memory, however, still in real mode, they can not use 32bit mode.   
Some program use  D0000-EFFFF to switch page to allocate data into 1MB.   
INT10h MODE 02h,03h   
TEXT MODE:80x25 FONT SIZE:9x14 -> 720x400dot 16 color within 16bit color   
(MDA/EGA has the same number of displaying character)   
INT 10h MODE 12h   
GRAPHIC MODE: 640x480dot 16 color within 16bit color (High resoution mode in VGA)   
INT 10h MODE 13h/X-Mode   
GRAPHIC MODE: 320x200/320x240 256 with 16bit color (256 color mode in VGA)   
Independent VIDEO Chip Vendor started to sell Highend video card with compabile register of onriginal   
VGA Card.   
IBM PS/2 supported the resolutions of 800x600,1024x768. These vendors also supported the resolutions    
with their own way(VideoCard : IBM 8514/ET3000/ET400/Tur64 S3/GD5430 etc)   
DOS can not support all these cards, applications supported them within their code.   
To improve this situation, VESA was founded. Graphics extended BIOS was standardized as   
VESA Extension BIOS   
Windows95/NT supported this BIOS natively.   
After while, to increate Windows Application, the applications do not reply on BIOS, Windows provide APIs   
To access functionalities.   
After introduced USB, ACPI and 32bit, BIOS marker or BIOS open source were implemented with C Language   
in protect mode.However in VBE also, APIC was provided stanard access way to get adddress information and   
Legacy function which does not have high compatible among vendors, were called by INT instruction which was used    
by old sofware which does not need to worry about address space.   
APCI provide these mechanisms, however only OS vendors(Microsoft/Linux/BSD) should support them, application marker does not need it.   
Windows PC is the standard collection of IBM PC, Intel and Microsoft   
That is why, In physical real mode and virutla 8086 mode, application most only use INT insturction to call system functions.   
Even though, they use DOS4G mode(32bit mode), application do not worry about which mode is used.   
Application execute INT instructions, applications do not need to worry about which mode and address which were set by system library(DOS4G/NTVDM)   
0x00000000-0x0009FFFF Sytem Board(CONVENTIONAL RAM)   
0x000A0000-0x000BFFFF VGA VIDEO RAM(128KB)   
0x000C0000-0x000CFFFF Sytem Board(64KB) (Adapter BIOS)   
0x000D0000-0x000DFFFF (For EMM page?)   
0x000E0000-0x000FFFFF Sytem Board(128KB) (BIOS/NVRAM Area)   
0xFED00000-0xFED003FF CPU event timer(1KB)   
0xFED14000-0xFED19FFF System Board(6FFF:28KB)   
The implementation of PC/AT Original BIOS Service Routine   
;----- ESTABLISH BIOS SUBROUTINE CALL INTERRUPT VECTORS   
MOV AL,15H ;  <><><><><><><><><><><><>   
OUT MFG_PORT,AL ;  <><> CHECKPOINT  15 <><>   
MOV DI,OFFSET @VIDEO_INT ; SET VIDEO INTERRUPT AREA   
PUSH CS POP DS ; SET UP ADDRESS OF VECTOR TABLE   
MOV AX,DS ; SET AX=SEGMENT   
MOV SI,OFFSET VECTOR_TABLE+16  ; START WITH VIDEO ENTRY   
MOV CX,16   
D3A:   
MOVSW ; MOVE VECTOR TABLE TO LOW MEMORY   
INC DI INC DI ; SKIP SEGMENT POINTER   
LOOP D3A   
dseg.inc   
ORG 4*010H   
@VIDEO_INT DD ? ; VIDEO I/O INTERRUPT VECTOR   
;----- SOFTWARE INTERRUPTS  ( BIOS CALLS AND POINTERS )   
DW OFFSET VIDEO_IO                   ; INT 10H -- VIDEO DISPLAY   
DW OFFSET EQUIPMENT               ; INT 11H -- GET EQUIPMENT FLAG WORD   
DW OFFSET MEMORY_SIZE_DET ; INT 12H -- GET REAL MODE MEMORY SIZE   
DW OFFSET DISKETTE_IO             ; INT 13H -- DISKETTE   
DW OFFSET RS232_IO                    ; INT 14H -- COMMUNICATION ADAPTER   
DW OFFSET CASSETTE_IO            ; INT 15H -- EXPANDED BIOS FUNCTION CALL   
DW OFFSET KEYBOARD_IO          ; INT 16H -- KEYBOARD INPUT   
DW OFFSET PRINTER_IO                ; INT 17H -- PRINTER OUTPUT   
DW 00000H                                         ; INT 18H -- 0F600H INSERTED FOR BASIC   
DW OFFSET BOOT_STRAP              ; INT 19H -- BOOT FROM SYSTEM MEDIA   
DW OFFSET TIME_OF_DAY            ; INT 1AH -- TIME OF DAY   
DW OFFSET DUMMY_RETURN      ; INT 1BH -- KEYBOARD BREAK ADDRESS   
DW OFFSET DUMMY_RETURN      ; INT 1CH -- TIMER BREAK ADDRESS   
DW OFFSET VIDEO_PARMS           ; INT 1DH -- VIDEO PARAMETERS   
DW OFFSET DISK_BASE                 ; INT 1EH -- DISKETTE PARAMETERS   
DW 00000H                                         ; INT 1FH -- POINTER TO VIDEO EXTENSION   
In Real Mode, the least Address 4x256=1KB was used in INT VECTOR TABLE.   
The BIOS CALL used in Linux-0.96BIOS CALLs are   
INT 10H   
INT 13H   
INT 15H   
INT 16H of four.   
! Get memory size (extended mem, kB)   
mov ah,#0x88   
int 0x15   
mov [2],ax   
This INT 15H was extened as ACPI standard, system can get Query Address System Map to execute   
INT 15H with EAX=E820   
Hardware Interrupt   
Name InterruptVector  Priority Description   
NMI    02                       1         Memory Parity Error   
IRQ0   08                       2         Timer (Intel 8253 Chip 55 ms intervals)   
IRQ1   09                       3          Keyboard   
IRQ2   0A                      4          8259 PIC Slave   
IRQ3   0B                     13          Serial Port  (COM2 )   
IRQ4    0C                    14          Serial Port  (COM1)   
IRQ5    0D                    15           Reserved   
IRQ6    0E                    16           Floppy Disk Driver   
IRQ7    0F                    17           LPT1 Request (SoundBlaster16)   
IRQ8    70                      5           CMOS Real-Time Clock (RT Chip)   
IRQ9    71                      6            Re-directed to IRQ2 or Reserved (NE2000)   
IRQ10  72                      7            RESERVED   
IRQ11  73                       8           RESERVED   
IRQ12   74                      9           Mouse(IBM PS/2)   
IRQ13   75                    10           Math Coprocessor (NPX)   
IRQ14   76                     11         Primary Hard Disk (Primary ATA)   
IRQ15   77                     12         RESERVED (Secondary ATA)   
CGA/VGA System 16 color   
Intensity Red Green Blue Color         Red(8bit) Green(8bit) Blue(8bit)   
0 0 0 0 Black                   0     0       0   
0 0 0 1 Blue                     0     0   170   
0 0 1 0 Green                   0 170      0   
0 0 1 1 Cyan                    0  170  170   
0 1 0 0 Red                  170     0      0   
0 1 0 1 Magenta           170     0   170   
0 1 1 0 Brown               170   85       0   
0 1 1 1 White(Light Gray)      170 170 170   
1 0 0 0 Gray(Dark Gray)        85   85   85   
1 0 0 1 Light Blue            85 85 255   
1 0 1 0 Light Green          85 255 85   
1 0 1 1 Light Cyan            85 255 255   
1 1 0 0 Light Red            255  85    85   
1 1 0 1 Light Magenta     255   85  255   
1 1 1 0 Yellow                255 255    85   
1 1 1 1 White(High Intensity)  255 255 255   
The key scan code of AT Keyboard were implemented in ps2.c   
SVGA Mode was depend on VESA BIOS. However, Intel introduced built-in PCI VGA as 2nd defacto standard.   
Til PCI/Pentium/Windows95, many PC had ISA bus(16bit)   
However, before PCI, some PCs had VL bus for Graphics card to access 32bit.   
The first generation has ISA Bus as the CPU bus, i386/i486 compatible PC Machine split   
ISA Bus from CPU bus. CPU->Memory/VL(33MHz) ,CPU->ISA(8.25MHz)   
DOS app can use VM86 with EMM386.EXE(DPMI) or DOS/4G   
Some vendor created OS indepent standard of VCPI to switch 32bit/Protect mode.   
However, DPMI DOS/4G were simple enough to use the huge memory for these applications at time.   
VCPI did not exist.   
1991?   
386SX(486DX) + Main Memory 4MB + VLBUS(SVGA) + HDD(IDE:PIO Mode)80MB + ISA(SoundBlaster) + FDD(720KB/1.44MB) + PS/2 Mouse   
1996   
Pentium75MHz + Main Memory 8MB + PCI(SVGA GL-5446) + HDD(EIDE)520MB + CD-ROM(ATAI) + ISA(SoundBlaster16) + Serial Port(3.84Kbps Modem) + FDD(720KB/1.44MB) + PS/2 Mouse   
After i486 generation, the PC did not have hardeare compatibilty with PCAT(i286)   
The PC had only ISA bus compatibility.   
After booted from BIOS, the PC had the INT of BIOS, some IO ports, compatible register.    
However ACPI standard obsolated these non-standard   
But, in 16bit(Windows98/3.1), Windows can not allocate flat memory bigger than 64KB.   
Graphics driver slide memory to switch segment regsiter, which make system slow.   
After introduced Windows 3.0 enchance mode, except NT series, Windows 16bit applications    
required 16bit(Protect mode) developed in Windows 2. Windows had so many 16bit code(Thunking)   
However, Windows 95 introduced Protect Mode 32bit(VxD)   
In NT series, application could use 32bit flat address with Win32 API   
However Windows NT had NTVDM to execute Win16 through Windows on Windows.   
Mode   
Real        Protect   
16bit     8086      286/VM86   
32bit       -              386   
VGA 256KB   
320x200x8bit color ~64KB   
320x240x8bit color ~100KB(Mode-X)   
640x480x4bit color 150KB   
VGA 512KB   
640x480x8bit color 300KB   
800x600x8bit color ~470KB   
1MB   
640x480x16bit 600KB   
800x600x16bit color 920KB   
1024x768x8bit color 470KB   
For VL/PCI Bus   
1. 0x80000000 (for S3 Inc cards)          NG   
5. 0x4D00000  (for Cirrus Logic cards)    NG   
6. 0x4E00000  (for Cirrus Logic cards)    NG   
7. 0x4F00000  (for Cirrus Logic cards)    NG   
For ISA Bus   
2. 0xD00000  (for Cirrus Logic cards)      OK   
3. 0xE00000  (for Cirrus Logic cards)      OK   
4. 0xF00000  (for Cirrus Logic cards)      OK   
8. 0xC00000  (for Tseng Labs cards)       OK   
9. 0x100000-0x3F00000 (for Trident cards) OK   
VBEMP NT Project(VBE Video Card driver)   
Now a part of ReactOS (only support 2.0 or the above)   
VBE 1.02 INTERNAL 1Mb Cirrus Logic CL-GD5428   
VBE 1.02 INTERNAL 512Kb Cirrus Logic CL-GD5424   
VBE 1.02 VLB 1Mb Cirrus Logic CL-GD5428   
VBE 1.02 PCI 1Mb Cirrus Logic CL-GD5446 PCI   
VBE 2.00 INTERNAL Trident CyberBlade i7   
VBE 1.02 ISA 1Mb Trident 8900   
VBE 1.02 ISA 512Kb Trident 9000i [True Color DAC]   
VBE 1.02 ISA 256Kb Realtek 3106 [RTG3106]   
no VBE ISA 256KB Cirrus Logic CL-GD5401 - Acumos VGA (AVGA1)   
VBE 2.00 PCI 8Mb? VirtualBox VBE Adapter (Innotek VirtualBox)   
VBE 2.00 PCI 16Mb VMware SVGA II (VMware)   
VBE 2.00 PCI 4Mb Cirrus Logic CL-GD5446 PCI (QEMU)   
VBE 2.00 PCI 4Mb IBM Plex86/Bochs VBE Adapter (QEMU/Bochs)   
VBE 2.00 PCI ?Mb IBM Plex86/Bochs VBE Adapter [vmware] (QEMU/Bochs)   
no VBE ISA 256KB Elpin VGABIOS (Bochs)   
Physical Alignment   
1MB(Conventional Memory)+3MB(MainMemory:Paging)   
Linus comments   
only the kernel page functions use the >1Mb addresses directly.   
All "normal" functions use just the lower 1Mb, or the local data space,   
which will be mapped to some other place - mm keeps track of that.   
#define DEF_INITSEG 0x9000(the last 64KB)   
#define DEF_SYSSEG 0x1000(the second)   
#define DEF_SETUPSEG 0x9020(the last 64KB)   
#define DEF_SYSSIZE 0x4000(the fifth)   
32bit code begin from 0x0 after kernel code GDT, IDT, LDT resident in memory(0.11).   
ORIGINAL   
INT13H:    DISKETTE_IO   
IBM PC(XT)   360KB(5.25inch)   
DRIVE NUMBER                   (0-3)   
TRACK(CYLINDER) NUMBER (0-39)   
HEAD NUMBER                    (0-1)   
SECTOR NUMBER                (1-9)   
ibm-xt-1986-05-09.rom (PC/AT)   
INT 13H: DISKETTE_IO   
DRIVE NUMBER                   (0-3)   
TRACK(CYLINDER) NUMBER (0-79)   
HEAD NUMBER                    (0-1)   
SECTOR NUMBER                (1-15)   
This original limited BIOS paramter in IBM PC(X) can support 720KB.   
One sector is 512byte 80x2x9 = 1,440 x 512 = 737,280 -> 720KB(PCXT)   
Non-IBM PC 80x2x18 = 2,880  x 512 = 1,474,560 (1.44MB)      
They can not support1.44MB. Non-IBM PC introducted 1.44KB support with Windows.   
OS/2 were not supported well by Microsoft, after Microsoft introducted Widows 3.1   
The history of consumer PC OS were DOS1.1 -> DOS3.3 -> Windows 3.x/DOS5(co-existent)->   
Windows 95->Windows98/NT4(SP6)->Windows 2000->Windows XP.   
This happened from 1981->2001(20 years)   
1.44MB required sectors from 1 to 18.   
Microsoft Macro Assembler was developed for MS-DOS   
The success of IBM PC make this assembler defacto and then, introduced a lots of peudo instructions to    
support micro rather than C Language to implement this CPU complicated CPU mode handling.    
Indeed, if someone deeply knew Intel CPU, these micros were very useful to implement system rather than C Language.AT&T   
Microsft assember is using the opposite description for source and destination.   
   
   
X86 and x86-64 have different first bytes instruction scheme.   
But some byte value shared the same semantics.   
0x0f is for two bytes instruction   
0x66, 0x067 are for operand-size and address-size override(prefix)   
x86-32 can support SSE instruction series SSE,SSE2, SSE3,SSE4.1,SSE4.2,   
because SSE instruction scheme is almost identical to MMX.   
MMX was introduced into 32bit(x86)   
SSE   
Operand prefix, Two bytes marks, MMX opcode, operand1, operand2   
0x66                   0x0F                       , 0x38,               , xmm0 ,xmm1   
However, AVX was newly introduced into x86-64.   
In x86-64, AMD assigned some reserved first byte value for the future extension.   
Intel try to use the reserved byte.   
VEX prefix : 0xc4 and 0xc5   
EVEX prefix: 0x62   
In x86-32bit mode, these bytes had the different semantics of instructions.   
But , these new instructions only focus in 64bit mode.   
   
Windows 1.01    
Graphics    
1. IBM (or compatible) Color/Graphics Adapter    
2. Hercules Graphics Card (or compatible) with Monochrome Displary    
3. Enahcned Graphics Adapter (EGA) with Monochorme Personal Computer    
4. EGA with Enhanced Color Display (Black and White)   
5. EGA with Enhanced Color Display or Personal Computer Color Display    
6. EAG (more than 64KB) with Enhanced Color Display.    
Pointing Mouse    
Microsoft Mouse(Bus/Serial)    
   
Windows 2.0    
Graphics    
IBM (or 100% compatible) VGA (Video Graphics Array)    
Pointing Device    
PS/2 Mouse. 