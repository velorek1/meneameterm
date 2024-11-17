# Compiler
CC = gcc

# Compiler flags
CFLAGS = -funsigned-char

# Libraries
LIBS = -lcurl

# Source files
SRC = rterm.c scbuf.c ui.c tm.c main.c download.c parse.c listbox.c keyb.c fileop.c

# Object files (replace .c with .o in SRC)
OBJ = $(SRC:.c=.o)

# Output binary
TARGET = meneame

# Default rule
all: $(TARGET)

# Link object files to create the binary
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(CFLAGS) $(LIBS)

# Compile each .c file into an .o file
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Install libcurl libraries
install-libs:
	sudo apt-get install libcurl4 libcurl4-openssl-dev

# Clean up generated files
clean:
	rm -f $(OBJ) $(TARGET)

# Information message
info:
	@echo "Meneame for Terminals - Coded By v3l0r3k"
	@echo "Run as ./$(TARGET)"
	@echo "Enjoy it!"

.PHONY: all clean install-libs info

