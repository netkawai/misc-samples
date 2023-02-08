CC=gcc
LD=gcc
RM=rm
INSTALL=install

OUTPUT=wayland-egl-gear
OUTPUT_PATH=.
INSTALL_PATH=/usr/local/bin/
OBJECTS=$(SRC_PATH)/wayland-egl-gear.o $(SRC_PATH)/xdg-shell-protocol.o

SRC_PATH=.

CFLAGS=-g -Wall -D_FILE_OFFSET_BITS=64 `pkg-config --cflags wayland-client wayland-cursor wayland-egl egl gl` 
LDFLAGS=`pkg-config --libs wayland-client wayland-cursor wayland-egl egl gl`

.PHONY: all

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS) $(OUTPUT_PATH)
	$(CC) $(OBJECTS) -o "$(OUTPUT_PATH)/$@" $(LDFLAGS)

clean:
	- $(RM) $(OUTPUT) $(OBJECTS) *~ $(SRC_PATH)/*~

install: $(OUTPUT)
	$(INSTALL) $(OUTPUT) $(INSTALL_PATH)

%.o: %.c
	$(CC) -c $(CFLAGS) -o "$@" "$<"

#xdg-shell-protocol.c: xdg-shell-client-protocol.h
#	wayland-scanner private-code < /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml > xdg-shell-protocol.c

#xdg-shell-client-protocol.h: 
#	wayland-scanner client-header < /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml > xdg-shell-client-protocol.h
