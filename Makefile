CC = gcc
CFLAGS = `pkg-config --cflags gtk4` -Wall -Wextra -g -w
LDFLAGS = `pkg-config --libs gtk4`

# Ścieżki
SRCDIR = app
BUILDDIR = build
TARGET = gtk_app

# Pliki źródłowe i obiektowe (rekurencyjne wyszukiwanie)
SRC = $(shell find $(SRCDIR) -name '*.c')
OBJ = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))

# Główna reguła
all: $(TARGET)

# Kompilacja programu
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Kompilacja plików obiektowych
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Czyszczenie
cls:
	rm -rf $(BUILDDIR) $(TARGET)

# Uruchamianie
run: $(TARGET)
	./$(TARGET)
