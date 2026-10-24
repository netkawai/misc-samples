# Name of your binary
TARGETS = custom_rect flowbox_app

# Source files
SRC = main.c

# Compiler and Flags
CC = gcc
# Use pkg-config to get GTK 4, Graphene (for math), and Pango (for text) flags
CFLAGS = `pkg-config --cflags gtk4` -Wall -Wextra -O2
LIBS = `pkg-config --libs gtk4`

# Default rule: builds everything in the TARGETS list
all: $(TARGETS)

# 2. Pattern Rule: This tells Make how to build ANY target from a .c file
# % matches the filename
# $@ is the target name (e.g., custom_rect)
# $< is the first dependency (e.g., custom_rect.c)
%: %.c
	$(CC) $< -o $@ $(CFLAGS) $(LIBS)

# Clean rule
clean:
	rm -f $(TARGETS)