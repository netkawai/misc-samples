default curosor images are in wayland/cursor/cursor-data.h



```
kawai@C001474:~$ python3 -m pip install meson
kawai@C001474:~$
kawai@C001474:~$
kawai@C001474:~$ python3 -m pip install ninja
Collecting ninja
  Downloading ninja-1.11.1-py2.py3-none-manylinux_2_12_x86_64.manylinux2010_x86_64.whl (145 kB)
     |████████████████████████████████| 145 kB 9.5 MB/s
Installing collected packages: ninja
Successfully installed ninja-1.11.1
kawai@C001474:~$ ls
kawai@C001474:~/weston/build$ sudo apt-get install pkg-config
kawai@C001474:~/weston/build$ sudo apt-get install cmake
kawai@C001474:~/weston/build$ sudo apt-get install libx11-dev
kawai@C001474:~/weston/build$ sudo apt-get install libcairo2-dev
kawai@C001474:~/weston/build$ sudo apt-get install libxkbcommon-dev
kawai@C001474:~/weston$ sudo apt-get install libinput-dev
kawai@C001474:~$ git clone https://gitlab.freedesktop.org/wayland/wayland-protocols.git
kawai@C001474:~/weston/build$ sudo apt-get install libpango1.0-dev
kawai@C001474:~/weston/build$ sudo apt-get install libegl-dev
kawai@C001474:~/weston/build$ sudo apt-get install libseat-dev
kawai@C001474:~/weston/build$ sudo apt-get install libxcb-dev
kawai@C001474:~/weston/build$ sudo apt-get install libdrm-dev
```
option(
        'backend-drm',
        type: 'boolean',
        value: false,
        description: 'Weston backend: DRM/KMS'
)
option(
        'backend-drm-screencast-vaapi',
        type: 'boolean',
        value: false,
        description: 'DRM/KMS backend support for VA-API screencasting'
)
option(
        'backend-headless',
        type: 'boolean',
        value: false,
        description: 'Weston backend: headless (testing)'
)
option(
        'backend-rdp',
        type: 'boolean',
        value: false,
        description: 'Weston backend: RDP remote screensharing'
)
option(
        'screenshare',
        type: 'boolean',
        value: false,
        description: 'Compositor: RDP screen-sharing support'
)
option(
        'backend-vnc',
        type: 'boolean',
        value: false,
        description: 'Weston backend: VNC remote screensharing'
)
option(
        'backend-wayland',
        type: 'boolean',
        value: true,
        description: 'Weston backend: Wayland (nested)'
)
option(
        'backend-x11',
        type: 'boolean',
        value: true,
        description: 'Weston backend: X11 (nested)'
)
option(
        'backend-default',
        type: 'combo',
        choices: [ 'auto', 'drm', 'wayland', 'x11', 'headless', 'rdp' ],
        value: 'wayland',
        description: 'Default backend when no parent display server detected'
)

option(
        'renderer-gl',
        type: 'boolean',
        value: true,
        description: 'Weston renderer: EGL / OpenGL ES 2.x'
)

option(
        'xwayland',
        type: 'boolean',
        value: false,
        description: 'Xwayland: support for X11 clients inside Weston'
)
option(
        'xwayland-path',
        type: 'string',
        value: '/usr/bin/Xwayland',
        description: 'Xwayland: path to installed Xwayland binary'
)

option(
        'systemd',
        type: 'boolean',
        value: false,
        description: 'systemd service plugin: state notify, watchdog, socket activation'
)

option(
        'remoting',
        type: 'boolean',
        value: false,
        description: 'Virtual remote output with GStreamer on DRM backend'
)

option(
        'pipewire',
        type: 'boolean',
        value: false,
        description: 'Virtual remote output with Pipewire on DRM backend'
)

option(
        'shell-desktop',
        type: 'boolean',
        value: true,
        description: 'Weston shell UI: traditional desktop'
)
option(
        'shell-fullscreen',
        type: 'boolean',
        value: true,
        description: 'Weston shell UI: fullscreen/kiosk'
)
option(
        'shell-ivi',
        type: 'boolean',
        value: true,
        description: 'Weston shell UI: IVI (automotive)'
)
option(
        'shell-kiosk',
        type: 'boolean',
        value: true,
        description: 'Weston shell UI: kiosk (desktop apps)'
)

option(
        'desktop-shell-client-default',
        type: 'string',
        value: 'weston-desktop-shell',
        description: 'Weston desktop shell: default helper client selection'
)

option(
        'color-management-lcms',
        type: 'boolean',
        value: false,
        description: 'Compositor color management: Little CMS'
)
option(
        'deprecated-color-management-static',
        type: 'boolean',
        value: false,
        description: 'DEPRECATED: color management plugin cms-static'
)
option(
        'deprecated-color-management-colord',
        type: 'boolean',
        value: false,
        description: 'DEPRECATED: color management plugin cms-colord (requires cms-static)'
)

option(
        'deprecated-launcher-logind',
        type: 'boolean',
        value: false,
        description: 'DEPRECATED: Compositor: support systemd-logind D-Bus protocol (superseded by launcher-libseat)'
)

option(
        'launcher-libseat',
        type: 'boolean',
        value: true,
        description: 'Compositor: support libseat'
)

option(
        'image-jpeg',
        type: 'boolean',
        value: true,
        description: 'JPEG loading support'
)
option(
        'image-webp',
        type: 'boolean',
        value: true,
        description: 'WebP loading support'
)

option(
        'tools',
        type: 'array',
        choices: [ 'calibrator', 'debug', 'info', 'terminal', 'touch-calibrator' ],
        description: 'List of accessory clients to build and install'
)
option(
        'demo-clients',
        type: 'boolean',
        value: true,
        description: 'Sample clients: toytoolkit demo programs'
)
option(
        'simple-clients',
        type: 'array',
        choices: [ 'all', 'damage', 'im', 'egl', 'shm', 'touch', ],
        value: [ damage', 'im', 'egl', 'shm' ],
        description: 'Sample clients: simple test programs'
)

option(
        'resize-pool',
        type: 'boolean',
        value: true,
        description: 'Sample clients: optimize window resize performance'
)
option(
        'wcap-decode',
        type: 'boolean',
        value: true,
        description: 'Tools: screen recording decoder tool'
)

option(
        'test-junit-xml',
        type: 'boolean',
        value: false,
        description: 'Tests: output JUnit XML results'
)
option(
        'test-skip-is-failure',
        type: 'boolean',
        value: false,
        description: 'Tests: consider skip to be a failure'
)
option(
        'doc',
        type: 'boolean',
        value: false,
        description: 'Generate documentation'
)
```
kawai@C001474:~/weston/build$ sudo apt-get install libglvnd-dev
kawai@C001474:~$ git clone https://gitlab.freedesktop.org/wayland/wayland.git
kawai@C001474:~$ cd wayland
```
option('libraries',
  description: 'Compile Wayland libraries',
  type: 'boolean',
  value: true)
option('scanner',
  description: 'Compile wayland-scanner binary',
  type: 'boolean',
  value: true)
option('tests',
  description: 'Compile Wayland tests',
  type: 'boolean',
  value: false)
option('documentation',
  description: 'Build the documentation (requires Doxygen, dot, xmlto, xsltproc)',
  type: 'boolean',
  value: false)
option('dtd_validation',
  description: 'Validate the protocol DTD (requires libxml2)',
  type: 'boolean',
  value: false)
option('icon_directory',
  description: 'Location used to look for cursors (defaults to ${datadir}/icons if unset)',
  type: 'string',
  value: '')
```
kawai@C001474:~/wayland$ meson build/ setup --prefix=/usr
kawai@C001474:~/wayland$ sudo apt-get install libxml-dev
kawai@C001474:~/wayland$ ninja -C build/
ninja: Entering directory `build/'
[30/30] Linking target cursor/libwayland-cursor.so.0.21.90
kawai@C001474:~/wayland$ sudo ninja -C build/ install
kawai@C001474:~/wayland$ sudo -s
root@C001474:/home/kawai/wayland#
root@C001474:/home/kawai/wayland#
root@C001474:~# python3 -m pip install meson
Collecting meson
  Downloading meson-1.0.0-py3-none-any.whl (900 kB)
     |████████████████████████████████| 900 kB 11.3 MB/s
Installing collected packages: meson
Successfully installed meson-1.0.0
root@C001474:~# python3 -m pip install ninja
Collecting ninja
  Downloading ninja-1.11.1-py2.py3-none-manylinux_2_12_x86_64.manylinux2010_x86_64.whl (145 kB)
     |████████████████████████████████| 145 kB 11.1 MB/s
Installing collected packages: ninja
kawai@C001474:~/wayland$ sudo ninja -C build/ install
kawai@C001474:~$ cd wayland-protocols/
kawai@C001474:~/wayland-protocols$ meson build/
kawai@C001474:~/wayland-protocols$ sudo ninja -C build/ install^C
kawai@C001474:~/wayland-protocols$ rm -rf build
kawai@C001474:~/wayland-protocols$ meson build/ --prefix=/usr
kawai@C001474:~/wayland-protocols$ sudo ninja -C build/ install
kawai@C001474:~/weston$ meson build/ -Dtest-junit-xml=false -Dremoting=false -Dcolor-management-lcms=false
kawai@C001474:~/weston$ ninja -C build/
ninja: Entering directory `build/'
[137/450] Compiling C object libweston/libweston-12.so.0.0.0.p/pixel-formats.c.o
../libweston/pixel-formats.c: In function ‘pixel_format_get_modifier’:
../libweston/pixel-formats.c:732:18: warning: implicit declaration of function ‘drmGetFormatModifierName’ [-Wimplicit-function-declaration]
  732 |  modifier_name = drmGetFormatModifierName(modifier);
      |                  ^~~~~~~~~~~~~~~~~~~~~~~~
../libweston/pixel-formats.c:732:16: warning: assignment to ‘char *’ from ‘int’ makes pointer from integer without a cast [-Wint-conversion]
  732 |  modifier_name = drmGetFormatModifierName(modifier);
      |                ^
../libweston/pixel-formats.c:733:16: warning: implicit declaration of function ‘drmGetFormatModifierVendor’ [-Wimplicit-function-declaration]
  733 |  vendor_name = drmGetFormatModifierVendor(modifier);
      |                ^~~~~~~~~~~~~~~~~~~~~~~~~~
../libweston/pixel-formats.c:733:14: warning: assignment to ‘char *’ from ‘int’ makes pointer from integer without a cast [-Wint-conversion]
  733 |  vendor_name = drmGetFormatModifierVendor(modifier);
      |              ^
[248/450] Linking target libweston/libweston-12.so.0.0.0
FAILED: libweston/libweston-12.so.0.0.0
cc  -o libweston/libweston-12.so.0.0.0 libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_linux-dmabuf-unstable-v1-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_linux-explicit-synchronization-unstable-v1-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_input-method-unstable-v1-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_input-timestamps-unstable-v1-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_presentation-time-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_pointer-constraints-unstable-v1-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_relative-pointer-unstable-v1-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_single-pixel-buffer-v1-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_tearing-control-v1-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_text-cursor-position-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_text-input-unstable-v1-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_weston-touch-calibration-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_weston-content-protection-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_viewporter-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_xdg-output-unstable-v1-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_weston-debug-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_weston-direct-display-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_weston-output-capture-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_xdg-shell-unstable-v6-protocol.c.o libweston/libweston-12.so.0.0.0.p/meson-generated_.._.._protocol_xdg-shell-protocol.c.o libweston/libweston-12.so.0.0.0.p/animation.c.o libweston/libweston-12.so.0.0.0.p/auth.c.o libweston/libweston-12.so.0.0.0.p/bindings.c.o libweston/libweston-12.so.0.0.0.p/clipboard.c.o libweston/libweston-12.so.0.0.0.p/color.c.o libweston/libweston-12.so.0.0.0.p/color-noop.c.o libweston/libweston-12.so.0.0.0.p/compositor.c.o libweston/libweston-12.so.0.0.0.p/content-protection.c.o libweston/libweston-12.so.0.0.0.p/data-device.c.o libweston/libweston-12.so.0.0.0.p/drm-formats.c.o libweston/libweston-12.so.0.0.0.p/input.c.o libweston/libweston-12.so.0.0.0.p/linux-dmabuf.c.o libweston/libweston-12.so.0.0.0.p/linux-explicit-synchronization.c.o libweston/libweston-12.so.0.0.0.p/linux-sync-file.c.o libweston/libweston-12.so.0.0.0.p/log.c.o libweston/libweston-12.so.0.0.0.p/noop-renderer.c.o libweston/libweston-12.so.0.0.0.p/output-capture.c.o libweston/libweston-12.so.0.0.0.p/pixel-formats.c.o libweston/libweston-12.so.0.0.0.p/pixman-renderer.c.o libweston/libweston-12.so.0.0.0.p/plugin-registry.c.o libweston/libweston-12.so.0.0.0.p/screenshooter.c.o libweston/libweston-12.so.0.0.0.p/timeline.c.o libweston/libweston-12.so.0.0.0.p/touch-calibration.c.o libweston/libweston-12.so.0.0.0.p/weston-log-wayland.c.o libweston/libweston-12.so.0.0.0.p/weston-log-file.c.o libweston/libweston-12.so.0.0.0.p/weston-log-flight-rec.c.o libweston/libweston-12.so.0.0.0.p/weston-log.c.o libweston/libweston-12.so.0.0.0.p/weston-direct-display.c.o libweston/libweston-12.so.0.0.0.p/desktop_libweston-desktop.c.o libweston/libweston-12.so.0.0.0.p/desktop_client.c.o libweston/libweston-12.so.0.0.0.p/desktop_seat.c.o libweston/libweston-12.so.0.0.0.p/desktop_surface.c.o libweston/libweston-12.so.0.0.0.p/desktop_xwayland.c.o libweston/libweston-12.so.0.0.0.p/desktop_xdg-shell.c.o libweston/libweston-12.so.0.0.0.p/desktop_xdg-shell-v6.c.o libweston/libweston-12.so.0.0.0.p/shell-utils_shell-utils.c.o libweston/libweston-12.so.0.0.0.p/.._shared_matrix.c.o -Wl,--as-needed -Wl,--no-undefined -shared -fPIC -Wl,--start-group -Wl,-soname,libweston-12.so.0 -Wl,--whole-archive shared/libshared.a -Wl,--no-whole-archive /usr/lib/x86_64-linux-gnu/libwayland-server.so /usr/lib/x86_64-linux-gnu/libpixman-1.so -lm -ldl /usr/lib/x86_64-linux-gnu/libdrm.so /usr/lib/x86_64-linux-gnu/libxkbcommon.so /usr/lib/x86_64-linux-gnu/libEGL.so /usr/lib/x86_64-linux-gnu/libwayland-client.so -Wl,--end-group
/usr/bin/ld: libweston/libweston-12.so.0.0.0.p/pixel-formats.c.o: in function `pixel_format_get_modifier':
/home/kawai/weston/build/../libweston/pixel-formats.c:732: undefined reference to `drmGetFormatModifierName'
/usr/bin/ld: /home/kawai/weston/build/../libweston/pixel-formats.c:733: undefined reference to `drmGetFormatModifierVendor'
collect2: error: ld returned 1 exit status
[265/450] Compiling C object clients/weston-subsurfaces.p/subsurfaces.c.o
ninja: build stopped: subcommand failed.
kawai@C001474:~/weston$ ninja -C build/
ninja: Entering directory `build/'
[17/188] Compiling C object libweston/libweston-12.so.0.0.0.p/pixel-formats.c.o
../libweston/pixel-formats.c: In function ‘pixel_format_get_modifier’:
../libweston/pixel-formats.c:732:18: warning: implicit declaration of function ‘drmGetFormatModifierName’ [-Wimplicit-function-declaration]
  732 |  modifier_name = drmGetFormatModifierName(modifier);
      |                  ^~~~~~~~~~~~~~~~~~~~~~~~
../libweston/pixel-formats.c:732:16: warning: assignment to ‘char *’ from ‘int’ makes pointer from integer without a cast [-Wint-conversion]
  732 |  modifier_name = drmGetFormatModifierName(modifier);
      |                ^
ninja: build stopped: subcommand failed.
```
__Comment out drmGetFormatModifierName and drmGetFormatModifierNameVendor__
```
kawai@C001474:~/weston$ ninja -C build/
kawai@C001474:~/weston$ cd build/clients/
kawai@C001474:~/weston/build/data$ cd /usr/local/share/
kawai@C001474:/usr/local/share$ sudo mkdir weston
kawai@C001474:/usr/local/share/weston$ sudo cp -a ~/weston/data/*.png .
kawai@C001474:~/weston/build/clients$ ./weston-terminal
```