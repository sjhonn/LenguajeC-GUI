#include "calculadora.h"

static Calculadora *calc_global = NULL;

// Función para actualizar la pantalla
void actualizar_pantalla(Calculadora *calc, const char *texto) {
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(calc->entry));
    gtk_entry_buffer_set_text(buffer, texto, -1);
}

// Funciones matemáticas adicionales
double factorial(double n) {
    if (n < 0) return 0;
    if (n == 0 || n == 1) return 1;
    double result = 1;
    for (int i = 2; i <= (int)n; i++) {
        result *= i;
    }
    return result;
}

double grados_a_radianes(double grados) {
    return grados * M_PI / 180.0;
}

double radianes_a_grados(double radianes) {
    return radianes * 180.0 / M_PI;
}

// Manejo de clicks en botones numéricos
void boton_numero_click(GtkWidget *widget, Calculadora *calc) {
    const char *numero = gtk_button_get_label(GTK_BUTTON(widget));
    const char *texto_actual = gtk_entry_get_text(GTK_ENTRY(calc->entry));
    
    if (calc->nueva_entrada || strcmp(texto_actual, "0") == 0) {
        actualizar_pantalla(calc, numero);
        calc->nueva_entrada = FALSE;
    } else {
        char nuevo_texto[256];
        snprintf(nuevo_texto, sizeof(nuevo_texto), "%s%s", texto_actual, numero);
        actualizar_pantalla(calc, nuevo_texto);
    }
}

// Manejo de operaciones básicas
void boton_operacion_click(GtkWidget *widget, Calculadora *calc) {
    const char *op = gtk_button_get_label(GTK_BUTTON(widget));
    const char *texto_actual = gtk_entry_get_text(GTK_ENTRY(calc->entry));
    
    calc->resultado = atof(texto_actual);
    
    if (strcmp(op, "+") == 0) calc->operacion = '+';
    else if (strcmp(op, "-") == 0) calc->operacion = '-';
    else if (strcmp(op, "×") == 0) calc->operacion = '*';
    else if (strcmp(op, "÷") == 0) calc->operacion = '/';
    
    calc->nueva_entrada = TRUE;
}

// Manejo del botón igual
void boton_igual_click(GtkWidget *widget, Calculadora *calc) {
    const char *texto_actual = gtk_entry_get_text(GTK_ENTRY(calc->entry));
    double segundo_numero = atof(texto_actual);
    double resultado_final = calc->resultado;
    
    switch(calc->operacion) {
        case '+': resultado_final += segundo_numero; break;
        case '-': resultado_final -= segundo_numero; break;
        case '*': resultado_final *= segundo_numero; break;
        case '/': 
            if (segundo_numero != 0) {
                resultado_final /= segundo_numero;
            } else {
                actualizar_pantalla(calc, "Error: Div/0");
                calc->nueva_entrada = TRUE;
                return;
            }
            break;
    }
    
    char resultado_str[256];
    snprintf(resultado_str, sizeof(resultado_str), "%g", resultado_final);
    actualizar_pantalla(calc, resultado_str);
    calc->resultado = resultado_final;
    calc->nueva_entrada = TRUE;
}

// Funciones científicas
void boton_funcion_click(GtkWidget *widget, Calculadora *calc) {
    const char *funcion = gtk_button_get_label(GTK_BUTTON(widget));
    const char *texto_actual = gtk_entry_get_text(GTK_ENTRY(calc->entry));
    double valor = atof(texto_actual);
    double resultado = 0;
    
    if (strcmp(funcion, "sin") == 0) {
        if (calc->modo_angular == 'D') valor = grados_a_radianes(valor);
        resultado = sin(valor);
    }
    else if (strcmp(funcion, "cos") == 0) {
        if (calc->modo_angular == 'D') valor = grados_a_radianes(valor);
        resultado = cos(valor);
    }
    else if (strcmp(funcion, "tan") == 0) {
        if (calc->modo_angular == 'D') valor = grados_a_radianes(valor);
        resultado = tan(valor);
    }
    else if (strcmp(funcion, "ln") == 0) {
        if (valor > 0) resultado = log(valor);
        else {
            actualizar_pantalla(calc, "Error");
            return;
        }
    }
    else if (strcmp(funcion, "log") == 0) {
        if (valor > 0) resultado = log10(valor);
        else {
            actualizar_pantalla(calc, "Error");
            return;
        }
    }
    else if (strcmp(funcion, "√") == 0) {
        if (valor >= 0) resultado = sqrt(valor);
        else {
            actualizar_pantalla(calc, "Error");
            return;
        }
    }
    else if (strcmp(funcion, "x²") == 0) {
        resultado = pow(valor, 2);
    }
    else if (strcmp(funcion, "xʸ") == 0) {
        calc->resultado = valor;
        calc->operacion = '^';
        calc->nueva_entrada = TRUE;
        return;
    }
    else if (strcmp(funcion, "π") == 0) {
        resultado = M_PI;
    }
    else if (strcmp(funcion, "e") == 0) {
        resultado = M_E;
    }
    else if (strcmp(funcion, "n!") == 0) {
        resultado = factorial(valor);
    }
    else if (strcmp(funcion, "±") == 0) {
        resultado = -valor;
    }
    else if (strcmp(funcion, "1/x") == 0) {
        if (valor != 0) resultado = 1.0 / valor;
        else {
            actualizar_pantalla(calc, "Error");
            return;
        }
    }
    else if (strcmp(funcion, "DEG") == 0) {
        calc->modo_angular = 'D';
        actualizar_pantalla(calc, texto_actual);
        return;
    }
    else if (strcmp(funcion, "RAD") == 0) {
        calc->modo_angular = 'R';
        actualizar_pantalla(calc, texto_actual);
        return;
    }
    
    char resultado_str[256];
    snprintf(resultado_str, sizeof(resultado_str), "%g", resultado);
    actualizar_pantalla(calc, resultado_str);
    calc->resultado = resultado;
    calc->nueva_entrada = TRUE;
}

// Botón de limpiar
void boton_limpiar_click(GtkWidget *widget, Calculadora *calc) {
    actualizar_pantalla(calc, "0");
    calc->resultado = 0;
    calc->operacion = 0;
    calc->nueva_entrada = TRUE;
}

// Crear pestaña normal
void crear_pestana_normal(Calculadora *calc) {
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_widget_set_margin_start(grid, 10);
    gtk_widget_set_margin_end(grid, 10);
    gtk_widget_set_margin_top(grid, 10);
    gtk_widget_set_margin_bottom(grid, 10);
    
    // Pantalla
    calc->entry = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(calc->entry), 1.0);
    gtk_widget_set_hexpand(calc->entry, TRUE);
    gtk_grid_attach(GTK_GRID(grid), calc->entry, 0, 0, 4, 1);
    actualizar_pantalla(calc, "0");
    
    // Definición de botones [texto, col, fila, ancho, función]
    char *botones[][5] = {
        {"C", "0", "1", "1", "limpiar"},
        {"±", "1", "1", "1", "funcion"},
        {"%", "2", "1", "1", "operacion"},
        {"÷", "3", "1", "1", "operacion"},
        
        {"7", "0", "2", "1", "numero"},
        {"8", "1", "2", "1", "numero"},
        {"9", "2", "2", "1", "numero"},
        {"×", "3", "2", "1", "operacion"},
        
        {"4", "0", "3", "1", "numero"},
        {"5", "1", "3", "1", "numero"},
        {"6", "2", "3", "1", "numero"},
        {"-", "3", "3", "1", "operacion"},
        
        {"1", "0", "4", "1", "numero"},
        {"2", "1", "4", "1", "numero"},
        {"3", "2", "4", "1", "numero"},
        {"+", "3", "4", "1", "operacion"},
        
        {"0", "0", "5", "2", "numero"},
        {".", "2", "5", "1", "numero"},
        {"=", "3", "5", "1", "igual"}
    };
    
    // Crear botones
    for (int i = 0; i < 19; i++) {
        GtkWidget *boton = gtk_button_new_with_label(botones[i][0]);
        gtk_widget_set_hexpand(boton, TRUE);
        gtk_widget_set_vexpand(boton, TRUE);
        
        int col = atoi(botones[i][1]);
        int fila = atoi(botones[i][2]);
        int ancho = atoi(botones[i][3]);
        
        gtk_grid_attach(GTK_GRID(grid), boton, col, fila, ancho, 1);
        
        // Conectar señal según el tipo
        if (strcmp(botones[i][4], "numero") == 0) {
            g_signal_connect(boton, "clicked", G_CALLBACK(boton_numero_click), calc);
        } else if (strcmp(botones[i][4], "operacion") == 0) {
            g_signal_connect(boton, "clicked", G_CALLBACK(boton_operacion_click), calc);
        } else if (strcmp(botones[i][4], "igual") == 0) {
            g_signal_connect(boton, "clicked", G_CALLBACK(boton_igual_click), calc);
        } else if (strcmp(botones[i][4], "limpiar") == 0) {
            g_signal_connect(boton, "clicked", G_CALLBACK(boton_limpiar_click), calc);
        } else if (strcmp(botones[i][4], "funcion") == 0) {
            g_signal_connect(boton, "clicked", G_CALLBACK(boton_funcion_click), calc);
        }
    }
    
    gtk_notebook_append_page(GTK_NOTEBOOK(calc->notebook), grid, 
                             gtk_label_new("Normal"));
}

// Crear pestaña científica
void crear_pestana_cientifica(Calculadora *calc) {
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_widget_set_margin_start(grid, 10);
    gtk_widget_set_margin_end(grid, 10);
    gtk_widget_set_margin_top(grid, 10);
    gtk_widget_set_margin_bottom(grid, 10);
    
    // Pantalla (usamos la misma)
    gtk_grid_attach(GTK_GRID(grid), calc->entry, 0, 0, 5, 1);
    
    // Botones científicos
    char *botones_cientificos[][5] = {
        {"sin", "0", "1", "1", "funcion"},
        {"cos", "1", "1", "1", "funcion"},
        {"tan", "2", "1", "1", "funcion"},
        {"DEG", "3", "1", "1", "funcion"},
        {"RAD", "4", "1", "1", "funcion"},
        
        {"ln", "0", "2", "1", "funcion"},
        {"log", "1", "2", "1", "funcion"},
        {"√", "2", "2", "1", "funcion"},
        {"x²", "3", "2", "1", "funcion"},
        {"xʸ", "4", "2", "1", "funcion"},
        
        {"π", "0", "3", "1", "funcion"},
        {"e", "1", "3", "1", "funcion"},
        {"n!", "2", "3", "1", "funcion"},
        {"1/x", "3", "3", "1", "funcion"},
        {"±", "4", "3", "1", "funcion"},
        
        // Botones numéricos
        {"7", "0", "4", "1", "numero"},
        {"8", "1", "4", "1", "numero"},
        {"9", "2", "4", "1", "numero"},
        {"÷", "3", "4", "1", "operacion"},
        {"C", "4", "4", "1", "limpiar"},
        
        {"4", "0", "5", "1", "numero"},
        {"5", "1", "5", "1", "numero"},
        {"6", "2", "5", "1", "numero"},
        {"×", "3", "5", "1", "operacion"},
        
        {"1", "0", "6", "1", "numero"},
        {"2", "1", "6", "1", "numero"},
        {"3", "2", "6", "1", "numero"},
        {"-", "3", "6", "1", "operacion"},
        
        {"0", "0", "7", "2", "numero"},
        {".", "2", "7", "1", "numero"},
        {"=", "3", "7", "1", "igual"},
        {"+", "4", "6", "1", "operacion"}
    };
    
    // Crear botones científicos
    for (int i = 0; i < 30; i++) {
        GtkWidget *boton = gtk_button_new_with_label(botones_cientificos[i][0]);
        gtk_widget_set_hexpand(boton, TRUE);
        gtk_widget_set_vexpand(boton, TRUE);
        
        int col = atoi(botones_cientificos[i][1]);
        int fila = atoi(botones_cientificos[i][2]);
        int ancho = atoi(botones_cientificos[i][3]);
        
        gtk_grid_attach(GTK_GRID(grid), boton, col, fila, ancho, 1);
        
        // Conectar señal
        if (strcmp(botones_cientificos[i][4], "numero") == 0) {
            g_signal_connect(boton, "clicked", G_CALLBACK(boton_numero_click), calc);
        } else if (strcmp(botones_cientificos[i][4], "operacion") == 0) {
            g_signal_connect(boton, "clicked", G_CALLBACK(boton_operacion_click), calc);
        } else if (strcmp(botones_cientificos[i][4], "igual") == 0) {
            g_signal_connect(boton, "clicked", G_CALLBACK(boton_igual_click), calc);
        } else if (strcmp(botones_cientificos[i][4], "limpiar") == 0) {
            g_signal_connect(boton, "clicked", G_CALLBACK(boton_limpiar_click), calc);
        } else if (strcmp(botones_cientificos[i][4], "funcion") == 0) {
            g_signal_connect(boton, "clicked", G_CALLBACK(boton_funcion_click), calc);
        }
    }
    
    gtk_notebook_append_page(GTK_NOTEBOOK(calc->notebook), grid, 
                             gtk_label_new("Científica"));
}

// Crear la calculadora completa
void crear_calculadora(void) {
    Calculadora *calc = g_new0(Calculadora, 1);
    calc_global = calc;
    calc->modo_angular = 'D';  // Grados por defecto
    calc->nueva_entrada = TRUE;
    
    // Crear ventana principal
    calc->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(calc->window), "Calculadora C");
    gtk_window_set_default_size(GTK_WINDOW(calc->window), 400, 500);
    g_signal_connect(calc->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Crear notebook para pestañas
    calc->notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(calc->window), calc->notebook);
    
    // Crear pestañas
    crear_pestana_normal(calc);
    crear_pestana_cientifica(calc);
    
    gtk_widget_show_all(calc->window);
}