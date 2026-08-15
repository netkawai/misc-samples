gcloud beta functions deploy ocr-extract --stage-bucket myocr-testprogs --trigger-bucket myocr-images --entry-point processImage --region 
gcloud beta functions deploy ocr-save --stage-bucket myocr-testprogs --trigger-topic myocr-topic --entry-point saveResult

myocr-images
myocr-storage
myocr-testprogs

gcloud app instances delete aef-default-20171117t152125-gw30
gcloud app versions delete 20171117t152125


inline only for inline implementation function, NOT replace Macro function.
template to implement generic concept in C++, NOT replace pointer concept.

Vulkan C specificaiton

#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;

#if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__) ) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
        #define VK_DEFINE_NON_DISPATCHABLE_HANDLE(object) typedef struct object##_T *object;
#else
        #define VK_DEFINE_NON_DISPATCHABLE_HANDLE(object) typedef uint64_t object;
#endif

VK_DEFINE_HANDLE(vkInstance)


Monitor GUI terminal option(electron linux+network+HDMI output) : Mobile App, PC, Mac
Compute Engine+Data storage: Cloud , on-premise(Cloud sync) 
(display optional powerbutton or web server REST(sample Web UI))
PC(Desktop?) High efficiency machine(Android HOME, Raspberry 3+SSD)


Sensor terminal: Collector (RTOS: wifi+battery or ether+powerline : minimum power consumption)
Raspberry zero (w): RTOS based(linux) for monitor
Sensor: temperature, humidity, brightness, noise level?, location(GPS), weather?, (solar voltage, ammeter, electricity metering?), 
(electric water metering???), (gas metering?), (food metering??? how gramme? type? vegitable? or buying? barcode???? price and other?????)
(TV watching meter or PC),(non intelligent device only power consumption??? no control???)

Sleep function, wakeup on lan?, runtime(nodejs), QAMPP??? 


As of 2018, the latest mimium version should be API level 14(Kitkat), 
if developers want to support particial package structure and the latest AI.
Google enforce Target API should be nougat API Level 26 and 28 from November 2019

Msdn said that IE10 and IE11 should work with xml:lang(XHTML);lang(HTML5), 
but if you specify compatibility mode , does not work.

Now Microsoft move Edge to Chromium base

CSS designer should specify font for CJK 
Japanese font :(Hiragino Kaku Gothic Pro,MS PGothic,Arial Unicode MS,HanaMinA,HanaMinB,Code2000,sans-serif)


C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\ClangC2\14.10.25903\bin\HostX64\clang.exe test.c -c -o test.obj
link.exe test.obj ucrt.lib vcruntime.lib msvcrt.lib /OUT:test.exe /SUBSYSTEM:CONSOLE  user32.lib kernel32.lib


// NAT64/DNS64 measurement
ping.exe -6 -t -w 10000 2001:67c:27e4:1064::5bef:6015

clang wcsftime.c -fshort-wchar in Linux(GNU)

If a program was compiled with -fshort-wchar in Linux(GNU)/MacOS, printf does not show any character.
The main reason is wchar_t is signed int(int32_t) in Linux/MacOS, 
on the other hand, wchar_t is unsigned short(uint16_t) in Windows environment.

OpenBSD automatic disk allocation

wd0a : root(/)
wd0b : swap
wd0c : logical partition(extended partition)
wd0d : /usr
wd0e : /home


MSYS2 and MING64 environments are completely different.

MSYS2 compile with MSYS2(Cygwin) compatibility layer library to simulate the file path, execution environment and POSIX native functions.
The designer decided that the pure cygwin environment should support only command line base program to prevent mixing up Windows UI and Unix simulated system view.
MING64 is actually not environment, intend to be merely tools for compling Windows Applications with the same manner with open source compilers such as gcc or clang(-llvm).
The bash kicked by the batch file is MSYS2 binary which is built in Cygwin environment, however, if apps are compiled with MING64 toolsets , the apps do not have the Cygwin 
compatibility layer which can not see the simulation layer path such as "/c/Windows". That should be C:\Windows in the Windows manner.
Moreover, the primary executable paths are different. MSYS2 uses /usr/bin, MING64 uses /mingw64/bin. And then, includes and libraries and so on.
In this situation, the building tools leads to misconfiguration and confusions. For example, In MSYS2, building tool found python2 with Cygwin like uname literate,
However, In MING64, it became Python3, it seems Cygwin like, but it completes different, 
even though, they can not execute m4 mirco or bash command, because of lack of Cygwin compatibility layer.

That is the reason, if you need , for instance, in MING64 to run m4 macro or bash through Python, that is not success.......
This is the limitation.

Luckily, some famous applications rely on Python2 (As of ealry 2018), they do not transit Python3. If I keep Python2 in MSYS2 and if I do not allow execute Python3 in MING64,
If the building process does not use the executable files generated in building processes for generating another files, it may work.....

First generation iCore 7/5(Nehalem) do not support Unrestricted guest mode, which can run real mode OS directly in VM.
(Intel64-ia-32-architectures-software-developer-vol-3c-part-3 25.6 UNRESTRICTED GUESTS)

Qemu compiling
./configure --enable-sdl --disable-vnc  --disable-werror  --enable-capstone='system' --target-list=x86_64-softmmu --prefix=/qemu --python=/mingw64/bin/python3

qemu-system-x86_64 -bios OVMF-pure-efi.fd -cdrom cm-x86_64-14.1-rc1.iso -hda android.qcow2 -usb -device usb-tablet -boot d 
qemu-system-x86_64 -cdrom cd62.iso -hda hdd.qcow2 -usb -device usb-tablet -boot d


"C:\Program Files\qemu\qemu-system-x86_64.exe" -m 1024 -cdrom archlinux-2020.03.01-x86_64.iso -display sdl -machine q35 -accel whpx -nic user

Fontconfig configuration is done via /mingw64/etc/fonts/conf.avail and conf.d.
Read /mingw64/etc/conf.d/README

gcc -dM -E - < /dev/null

(localdb)\MSSQLLocalDB
(localdb)\ProjectsV13(generated by VS)

(No render)
./xclock.exe  -norender -digital  -update 1
(With render) 
xterm -fa 'UbuntuMono' -fs 9.5
xclock -render -digital -face Dejavu -update 1

Java has two toolkit, one is AWT, another is Swing.

Old AWT Java function does not support surroage pair, only UCS2. 
it intends to support only BMP, because this GUI depends on System UI(Windows, X Window System)
Java does not want to be peasant, they did not update the function to follow the change of Microsoft, X.org
to keep compatibility.

Java uses the native code for ttf and loads font from system, get a text image 
with AWTFontGenerateImage(sdk/src/solaris/native/sun/java2d/x11/X11FontScaler_md.c) which 
calls XDrawImageString16. (they don't want ot use Xutf8DrawImageString)

Java also uses only PutImage to draw Glyph images in X Server and has their own layout engine for Swing 
and support extensions GUI toolkit such as SWT.
Java supports OpenType(TrueTYpe) with system fonts, either a specific path which is specified from clinet.
This is AWTDrawGlyphList(src/solaris/native/sun/java2d/x11/X11TextRenderer_md.c) which calls PutImage.


<a id="my-saved-words-icon" href="/my-saved-words" class="sticky-icon save" data-ref="dictionary" data-word="guarantee" onclick="return false;">&nbsp;</a>


The access of Bing Map REST API
Authentification error.(Old API obsolate of the entry point and key both)
Microsoft App Potal for Graph(nodejs-connect-rest-sample)

Handbrake imitation of electron version(multiplatform UI wrapper)
Fontforge for Windows simply rely on VcXsrv.

LGPL vs GPL is different.
LGPL
GTK+
GLIB2


x264(notable....):
GPL

MIT/BSD
Enlightment
Xorg
Mesa

FFmpeg:
only decode: LGPL
with encode: GPL

The angels of God in Torah, Bible, Quran
Justice(strength): Michael
Messenger(lead): Gabriel
Healing : Raphael
