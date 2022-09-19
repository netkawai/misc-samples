bin_PROGRAMS = basicwin jpwin 
basicwin_SOURCES = basicwin.c  
basicwin_DEPENDENCIES = basicwin.c basicwin_icon.xbm 
basicwin_LDADD = -lX11
basicwin_LDFLAGS = @X_LIBS@

jpwin_SOURCES = ujiswin.c ujiscode.c
jpwin_DEPENDENCIES = ujiswin.c ujiscode.c basicwin_icon.xbm jpcode.h
jpwin_LDADD = -lX11
jpwin_LDFLAGS = @X_LIBS@

INCLUDES = @X_CFLAGS@
