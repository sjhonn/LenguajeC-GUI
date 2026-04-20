# Detecta el sistema operativo
UNAME_S := $(shell uname -s)

# Compilador
CC = gcc

# Flags base
CFLAGS_BASE = -Wall -O2 -lm

# Windows (MinGW/MSYS2)
ifeq ($(OS),Windows_NT)
	CFLAGS = $(CFLAGS_BASE) `pkg-config --cflags gtk+-3.0` -mwindows
	LIBS = `pkg-config --libs gtk+-3.0`
	EXECUTABLE = calculadora.exe
	ICON = calculadora.res
else
    # Linux
    ifeq ($(UNAME_S),Linux)
		CFLAGS = $(CFLAGS_BASE) `pkg-config --cflags gtk+-3.0`
		LIBS = `pkg-config --libs gtk+-3.0`
		EXECUTABLE = calculadora
    endif
    # macOS
    ifeq ($(UNAME_S),Darwin)
		CFLAGS = $(CFLAGS_BASE) -I/usr/local/include/gtk-3.0
		LIBS = -L/usr/local/lib -lgtk-3 -lgdk-3
		EXECUTABLE = calculadora
    endif
endif

SOURCES = main.c calculadora.c operaciones.c
OBJECTS = $(SOURCES:.c=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $(EXECUTABLE) $(OBJECTS) $(LIBS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

install: $(EXECUTABLE)
	@echo "Instalando calculadora..."
	@if [ "$(UNAME_S)" = "Linux" ]; then \
		sudo cp $(EXECUTABLE) /usr/local/bin/; \
		echo "Calculadora instalada en /usr/local/bin/"; \
	elif [ "$(UNAME_S)" = "Darwin" ]; then \
		sudo cp $(EXECUTABLE) /usr/local/bin/; \
		echo "Calculadora instalada en /usr/local/bin/"; \
	elif [ "$(OS)" = "Windows_NT" ]; then \
		echo "En Windows, copia manualmente $(EXECUTABLE) a tu ubicación deseada"; \
	fi

uninstall:
	@if [ "$(UNAME_S)" = "Linux" ] || [ "$(UNAME_S)" = "Darwin" ]; then \
		sudo rm -f /usr/local/bin/$(EXECUTABLE); \
		echo "Calculadora desinstalada"; \
	fi

.PHONY: all clean run install uninstall