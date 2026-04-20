#!/bin/bash

echo "Instalando Calculadora C"

# Detectar sistema operativo
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "Sistema: Linux detectado"
    
    # Verificar si GTK3 está instalado
    if ! pkg-config --exists gtk+-3.0; then
        echo "Instalando GTK3..."
        sudo apt-get update
        sudo apt-get install -y libgtk-3-dev
    fi
    
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Sistema: macOS detectado"
    
    # Verificar si Homebrew está instalado
    if ! command -v brew &> /dev/null; then
        echo "Instalando Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi
    
    # Instalar GTK3
    if ! brew list gtk+3 &> /dev/null; then
        echo "Instalando GTK3..."
        brew install gtk+3
    fi
    
elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
    echo "Sistema: Windows detectado"
    echo "Por favor, instala MSYS2 y GTK3 manualmente desde: https://www.msys2.org/"
    echo "Luego ejecuta: pacman -S mingw-w64-x86_64-gtk3"
fi

# Compilar el proyecto
echo "Compilando calculadora..."
make clean
make

if [ $? -eq 0 ]; then
    echo "✓ Compilación exitosa"
    
    # Instalar en el sistema
    make install
    
    echo "✓ Instalación completada"
    echo "Puedes ejecutar 'calculadora' desde la terminal"
else
    echo "✗ Error en la compilación"
    exit 1
fi