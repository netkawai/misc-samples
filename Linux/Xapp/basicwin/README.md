I don't claim any code under this folder.
If the demo codes came from others, the copyright of them belong to them.

Volume 1 Xlib (X11)
basicwin automake,autoconf configure
Makefile Makefile.am,configure.in

jpwin basicwin

# 1998-1999
Copied from Xlib sample from the the manual
Modified basicwin to jpwin to demonstrate to display a Japanese character with EUC encoding in X11.
Add automake, autoconf, Makefile.am and Configure.in

# 2021 
I decided to deleted Japanese characters source code with non-UTF8 after 22 years.

# 2023-02-15
I migrated to meson.build from automake/autoconf. They had been never worked properly two decades in different distros.

