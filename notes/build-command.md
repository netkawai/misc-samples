#help2man
#git clone https://salsa.debian.org/bod/help2man.git

#bison - cyclic dependency with gettext for NLS
#git clone https://git.sv.gnu.org/git/bison.git
#bootstrap
#configure --disable-nls

#gettext
#git clone https://git.sv.gnu.org/git/gettext.git
#autogen.sh
#configure --disable-nls

#CMake
#Mirror??
git clone https://github.com/Kitware/CMake.git

#xmlto
#git clone 
#aclocal
#autoconf
#automake
#configure

##shared-mime-info - needed by GIO MIME in glib also depend on docbook docbook-xsl
#git clone https://gitlab.freedesktop.org/xdg/shared-mime-info.git
##after install
#update-mime-database /usr/local/share/mime

# glib archlinux 2.64 (git 2.65)
#git clone https://gitlab.gnome.org/GNOME/glib.git
#meson _build


# gobject-introspection - with cyclic dependency with glib
# archlinux 2.64 (git 2.65)
#git clone https://gitlab.gnome.org/GNOME/gobject-introspection.git

#libgudev
#https://gitlab.gnome.org/GNOME/libgudev

#libpng
#git clone https://git.code.sf.net/p/libpng/code

#openjpeg
#git clone https://github.com/uclouvain/openjpeg.git

#libwebp
#git clone https://chromium.googlsource.com/webm/libwebp
#autogen.sh

#fontconfig -- some dependencies
#git clone https://gitlab.freedesktop.org/fontconfig/fontconfig.git

#freetype2 -first build without harfbuzz and second with harfbuzz (twice)
#This is official
#git clone https://git.sv.nongnu.org/r/freetype/freetype2.git

# icu
#git clone https://github.com/unicode-org/icu.git

#harfbuzz - build
#git clone https://github.com/harfbuzz/harfbuzz.git

#cairo
#git clone https://gitlab.freedesktop.org/cairo/cairo.git

#pango
#git clone https://gitlab.gnome.org/GNOME/pango.git


# emacs
git clone https://git.sv.gnu.org/git/emacs.git
./autogen.sh
./configure --without-all --with-xml --with-cairo --with-harfbuzz --with-libotf --with-x-toolkit=lucid --with-modules

#libinput
git clone https://gitlab.freedesktop.org/libinput/libinput.git
meson _build -Ddebug-gui=false -Ddocuemtntation=false -Dlibwacom=false -Dtest=false -Dudev-dir=/lib/udev

#xserver
#gitlab??
#git clone git://anongit.freedesktop.org/xorg/xserver
git clone https://gitlab.freedesktop.org/xorg/xserver
./autogen.sh --prefix=/usr/local --disable-docs --disable-devel-docs --enable-xwayland --disable-xorg --disable-xvfb --disable-xnest --disable-xquartz --disable-xwin
 meson _build -Dxorg=false -Dxwayland=true -Dxnest=false -Dxvfb=false

#weston
git clone https://gitlab.freedesktop.org/wayland/weston 
meson _build -Dbackend-drm-screencast-vaapi=false -Dpipewire=false -Dremoting=false -Dshell-ivi=false -Dcolor-management-colord=false -Dbackend-rdp=false 

#wpe-webkit
#From tar.bz
cmake -DPORT=WPE -DCMAKE_BUILD_TYPE=RelWithDebInfo -GNinja -DENABLE_MEDIA_SOURCE=OFF -DENABLE_VIDEO=OFF -DENABLE_WEB_AUDIO=OFF -DENABLE_ACCESSIBILITY=OFF -DENABLE_MINIBROWSER=ON -DENABLE_BUBBLEWRAP_SANDBOX=OFF

# vlc
git clone https://git.videolan.org/git/vlc.git
# only compile cvlc

#gdk-pixbuf
git clone https://gitlab.gnome.org/GNOME/gdk-pixbuf.git

#graphene
git clone https://github.com/ebassi/graphene.git

#at-spi2-core
git clone https://gitlab.gnome.org/GNOME/at-spi2-core.git

#at-spi2-atk
git clone https://gitlab.gnome.org/GNOME/at-spi2-atk.git

#gtk
meson _build -Dprint=none -Dx11-backend=false -Dmedia=ffmpeg -Dcolord=disabled -Dsysprof=disabled
