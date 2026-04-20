#ifndef CALCULADORA_H
#define CALCULADORA_H

#include <gtk/gtk.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// Estructura para la calculadora
typedef struct {
    GtkWidget *entry;           // Pantalla
    GtkWidget *notebook;        // Pestañas normal/científica
    GtkWidget *window;
    
    // Variables de estado
    double resultado;
    double memoria;
    char operacion;
    char modo_angular;          // 'D' para grados, 'R' para radianes
    gboolean nueva_entrada;
    gboolean segunda_funcion;    // Para funciones shift/2nd
    
    // Variables para cálculos
    char expresion[256];
    int pos_expresion;
} Calculadora;

// Funciones de la calculadora
void crear_calculadora(void);
void crear_pestana_normal(Calculadora *calc);
void crear_pestana_cientifica(Calculadora *calc);
void boton_click(GtkWidget *widget, Calculadora *calc);
void boton_operacion_click(GtkWidget *widget, Calculadora *calc);
void boton_funcion_click(GtkWidget *widget, Calculadora *calc);
void actualizar_pantalla(Calculadora *calc, const char *texto);
double evaluar_expresion(Calculadora *calc);

// Funciones matemáticas
double factorial(double n);
double grados_a_radianes(double grados);
double radianes_a_grados(double radianes);

#endif