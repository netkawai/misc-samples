CC=gcc
LD=gcc
RM=rm
INSTALL=install

OUTPUT=wayland-egl
OUTPUT_PATH=.
INSTALL_PATH=/usr/local/bin/
OBJECTS=$(SRC_PATH)/wayland-egl.o

SRC_PATH=.

CFLAGS=-Wall -D_FILE_OFFSET_BITS=64 `pkg-config --cflags wayland-client wayland-egl egl gl` 
LDFLAGS=`pkg-config --libs wayland-client wayland-egl egl gl`

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
