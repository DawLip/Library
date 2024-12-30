# CFLAGS=-Wall -Wunused -DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED \
# -DGDK_PIXBUF_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED

# program.run:main.c test.c
# 	gcc main.c test.c -o program.run $(CFLAGS) `pkg-config --cflags --libs gtk4`

# clean: 
# 	rm -f *.o $(PROG)

# CC = gcc
# CFLAGS = `pkg-config --cflags gtk4`
# LDFLAGS = `pkg-config --libs gtk4`
# SOURCES = test.c
# MAIN_FILE = main.c
# OBJECTS = $(SOURCES:.c=.o)
# EXECUTABLE = gtk_app

# all: $(EXECUTABLE)


# $(EXECUTABLE): $(OBJECTS)
# 	$(CC) $(MAIN_FILE) $(OBJECTS) -o $@ $(LDFLAGS)

# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJECTS) $(EXECUTABLE)

# Kompilator i flagi
# CC = gcc
# CFLAGS = `pkg-config --cflags gtk4` -Wall -Wextra -g
# LDFLAGS = `pkg-config --libs gtk4`

# # Ścieżki
# SRCDIR = app
# BUILDDIR = build
# TARGET = gtk_app

# # Pliki źródłowe i obiektowe
# SRC = $(wildcard $(SRCDIR)/*.c)
# OBJ = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))

# # Główna reguła
# all: $(TARGET)

# # Kompilacja programu
# $(TARGET): $(OBJ)
# 	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# # Kompilacja plików obiektowych
# $(BUILDDIR)/%.o: $(SRCDIR)/**%.c
# 	@mkdir -p $(BUILDDIR)
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Czyszczenie
# cls:
# 	rm -rf $(BUILDDIR) $(TARGET)

# # Uruchamianie
# run: $(TARGET)
# ./$(TARGET)
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
