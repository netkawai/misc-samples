# Name of your binary
TARGET = gsk_rect_app

# Source files
SRC = main.c

# Compiler and Flags
CC = gcc
# Use pkg-config to get GTK 4, Graphene (for math), and Pango (for text) flags
CFLAGS = `pkg-config --cflags gtk4` -Wall -Wextra -O2
LIBS = `pkg-config --libs gtk4`

# Default rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

# Clean rule
clean:
	rm -f $(TARGET)

# Run rule
run: all
	./$(TARGET)
