#include "calculadora.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    crear_calculadora();
    
    gtk_main();
    
    return 0;
}