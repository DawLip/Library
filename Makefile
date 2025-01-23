CC = gcc
CFLAGS = `pkg-config --cflags gtk4` -Wall -Wextra -g -w
LDFLAGS = `pkg-config --libs gtk4`

SRCDIR = app
BUILDDIR = build
TARGET = Library

SRC = $(shell find $(SRCDIR) -name '*.c')
OBJ = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

cls:
	rm -rf $(BUILDDIR) $(TARGET)

setup:
	conda activate ML39

run: $(TARGET)
	./$(TARGET)
