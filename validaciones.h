#ifndef VALIDACIONES_H
#define VALIDACIONES_H

double leerDoubleSeguro(const char* mensaje, double min, double max);
int leerEnteroSeguro(const char* mensaje, int min, int max);

 
#endif

/* Definimos funciones de validaciónes de seguridad para los datos de entrada, que serian 
 * desarrolladas en el archivo validaciones.c, y que serian utilizados en el archivo simulador.c 
 * para validar los datos de entrada del usuario, asegurando que se mantengan dentro de los rangos establecidos 
 * y evitando errores de ejecucuión o comportamientos inesperados del sistema.
 */