
#include <stdio.h>
#include "validaciones.h"

double leerDoubleSeguro(const char* mensaje, double min, double max) {
    double valor;
    char buffer[100];
    int valido = 0;

do {
        printf("%s", mensaje); // Imprime dinámicamente el mensaje de solicitud en pantalla.
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) { // fgets lee la línea completa de la consola (stdin) y valida que no sea nula.
            
            for (int i = 0; buffer[i] != '\0'; i++) {
                if (buffer[i] == ',') buffer[i] = '.'; // Reemplazar coma por punto si el usuario ingresó decimales con coma
            }
            if (sscanf(buffer, "%lf", &valor) == 1) {  // sscanf intenta convertir la cadena leída en un número de tipo double. Si tiene éxito devuelve 1
                if (valor >= min && valor <= max) {
                    valido = 1;
                } else {
                    printf("[ERROR] El valor debe estar entre %.2f y %.2f.\n", min, max);
                }
            } else {
                printf("[ERROR] Entrada invalida. Ingrese un numero valido.\n");
            }
        }
    } while (!valido);

    return valor;
}

int leerEnteroSeguro(const char* mensaje, int min, int max) {
    int valor;
    char buffer[100];
    int valido = 0;

    do {
        printf("%s", mensaje);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &valor) == 1) {
                if (valor >= min && valor <= max) {  // && indica que ambas condiciones deben cumplirse
                    valido = 1;
                } else {
                    printf("[ERROR] Opcion fuera de rango (%d - %d).\n", min, max);
                }
            } else {
                printf("[ERROR] Entrada invalida. Ingrese un numero entero.\n");
            }
        }
    } while (!valido);

    return valor;
}
