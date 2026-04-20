#include "calculadora.h"

// Variables globales para la calculadora
static double memoria_temp = 0;
static char buffer_expresion[1024] = "";

// Función para evaluar expresiones matemáticas
double evaluar_expresion(const char *expresion) {
    if (!expresion || strlen(expresion) == 0) return 0;
    
    double resultado = 0;
    char operador = '+';
    double numero_actual = 0;
    int i = 0;
    int len = strlen(expresion);
    
    while (i < len) {
        // Saltar espacios
        while (i < len && expresion[i] == ' ') i++;
        
        // Leer número
        if (expresion[i] >= '0' && expresion[i] <= '9' || expresion[i] == '.') {
            char num_str[64] = "";
            int j = 0;
            while (i < len && (expresion[i] >= '0' && expresion[i] <= '9' || expresion[i] == '.')) {
                num_str[j++] = expresion[i++];
            }
            num_str[j] = '\0';
            numero_actual = atof(num_str);
        }
        
        // Procesar operación
        switch(operador) {
            case '+': resultado += numero_actual; break;
            case '-': resultado -= numero_actual; break;
            case '*': resultado *= numero_actual; break;
            case '/': 
                if (numero_actual != 0) resultado /= numero_actual;
                else return 0;
                break;
        }
        
        // Leer siguiente operador
        while (i < len && expresion[i] == ' ') i++;
        if (i < len) {
            operador = expresion[i];
            i++;
        }
        
        numero_actual = 0;
    }
    
    return resultado;
}

// Función para calcular porcentaje
double calcular_porcentaje(double valor, double porcentaje) {
    return (valor * porcentaje) / 100.0;
}

// Función para calcular potencia
double potencia(double base, double exponente) {
    return pow(base, exponente);
}

// Función para raíz cuadrada con validación
double raiz_cuadrada(double valor, int *error) {
    if (valor < 0) {
        *error = 1;
        return 0;
    }
    *error = 0;
    return sqrt(valor);
}

// Función para logaritmo natural
double logaritmo_natural(double valor, int *error) {
    if (valor <= 0) {
        *error = 1;
        return 0;
    }
    *error = 0;
    return log(valor);
}

// Función para logaritmo base 10
double logaritmo_base10(double valor, int *error) {
    if (valor <= 0) {
        *error = 1;
        return 0;
    }
    *error = 0;
    return log10(valor);
}

// Función para seno con modo angular
double calcular_seno(double valor, char modo_angular) {
    if (modo_angular == 'D') {
        valor = valor * M_PI / 180.0;
    }
    return sin(valor);
}

// Función para coseno con modo angular
double calcular_coseno(double valor, char modo_angular) {
    if (modo_angular == 'D') {
        valor = valor * M_PI / 180.0;
    }
    return cos(valor);
}

// Función para tangente con modo angular
double calcular_tangente(double valor, char modo_angular) {
    if (modo_angular == 'D') {
        valor = valor * M_PI / 180.0;
    }
    return tan(valor);
}

// Función para arcoseno
double calcular_arcoseno(double valor, int *error) {
    if (valor < -1 || valor > 1) {
        *error = 1;
        return 0;
    }
    *error = 0;
    return asin(valor);
}

// Función para arcocoseno
double calcular_arcocoseno(double valor, int *error) {
    if (valor < -1 || valor > 1) {
        *error = 1;
        return 0;
    }
    *error = 0;
    return acos(valor);
}

// Función para arcotangente
double calcular_arcotangente(double valor) {
    return atan(valor);
}

// Función para memoria (guardar)
void memoria_guardar(double valor) {
    memoria_temp = valor;
}

// Función para memoria (recuperar)
double memoria_recuperar(void) {
    return memoria_temp;
}

// Función para memoria (sumar)
void memoria_sumar(double valor) {
    memoria_temp += valor;
}

// Función para memoria (restar)
void memoria_restar(double valor) {
    memoria_temp -= valor;
}

// Función para memoria (limpiar)
void memoria_limpiar(void) {
    memoria_temp = 0;
}

// Función para convertir unidades
double convertir_unidad(double valor, int tipo_conversion) {
    switch(tipo_conversion) {
        case 1: // km a millas
            return valor * 0.621371;
        case 2: // millas a km
            return valor * 1.60934;
        case 3: // kg a libras
            return valor * 2.20462;
        case 4: // libras a kg
            return valor * 0.453592;
        case 5: // celsius a fahrenheit
            return (valor * 9.0/5.0) + 32;
        case 6: // fahrenheit a celsius
            return (valor - 32) * 5.0/9.0;
        default:
            return valor;
    }
}

// Función para cálculos estadísticos básicos
double calcular_media(double *valores, int cantidad) {
    if (cantidad <= 0) return 0;
    double suma = 0;
    for (int i = 0; i < cantidad; i++) {
        suma += valores[i];
    }
    return suma / cantidad;
}

// Función para calcular desviación estándar
double calcular_desviacion_estandar(double *valores, int cantidad) {
    if (cantidad <= 1) return 0;
    double media = calcular_media(valores, cantidad);
    double suma_cuadrados = 0;
    for (int i = 0; i < cantidad; i++) {
        suma_cuadrados += pow(valores[i] - media, 2);
    }
    return sqrt(suma_cuadrados / (cantidad - 1));
}

// Función para resolver ecuación cuadrática
int resolver_cuadratica(double a, double b, double c, double *x1, double *x2) {
    if (a == 0) return -1; // No es cuadrática
    
    double discriminante = b*b - 4*a*c;
    
    if (discriminante < 0) {
        return 0; // Raíces complejas
    } else if (discriminante == 0) {
        *x1 = -b / (2*a);
        *x2 = *x1;
        return 1; // Una raíz real
    } else {
        *x1 = (-b + sqrt(discriminante)) / (2*a);
        *x2 = (-b - sqrt(discriminante)) / (2*a);
        return 2; // Dos raíces reales
    }
}

// Función para calcular interés compuesto
double interes_compuesto(double principal, double tasa, int periodos, int tiempo) {
    return principal * pow(1 + tasa/periodos, periodos * tiempo);
}

// Función para formatear números grandes
void formatear_numero(double numero, char *buffer, size_t tamaño) {
    if (fabs(numero) >= 1e12 || (fabs(numero) < 1e-12 && numero != 0)) {
        snprintf(buffer, tamaño, "%e", numero);
    } else {
        // Eliminar ceros decimales innecesarios
        snprintf(buffer, tamaño, "%g", numero);
    }
}

// Función para validar entrada
int validar_entrada(const char *texto) {
    if (!texto || strlen(texto) == 0) return 0;
    
    int puntos_decimales = 0;
    for (int i = 0; texto[i]; i++) {
        if (texto[i] == '.') {
            puntos_decimales++;
            if (puntos_decimales > 1) return 0;
        } else if (texto[i] < '0' || texto[i] > '9') {
            if (!(i == 0 && texto[i] == '-')) {
                return 0;
            }
        }
    }
    return 1;
}

// Función para procesar operaciones con historial
typedef struct {
    char operacion[256];
    double resultado;
    time_t timestamp;
} HistorialEntry;

static HistorialEntry historial[100];
static int historial_count = 0;

void agregar_al_historial(const char *operacion, double resultado) {
    if (historial_count < 100) {
        strncpy(historial[historial_count].operacion, operacion, 255);
        historial[historial_count].operacion[255] = '\0';
        historial[historial_count].resultado = resultado;
        historial[historial_count].timestamp = time(NULL);
        historial_count++;
    }
}

const HistorialEntry* obtener_historial(int index) {
    if (index < 0 || index >= historial_count) return NULL;
    return &historial[index];
}

int obtener_historial_count(void) {
    return historial_count;
}

void limpiar_historial(void) {
    historial_count = 0;
}