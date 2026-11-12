#include <stdio.h>
#include <math.h>
#include "simulador.h"
#include "validaciones.h"

void mostrarMenuPrincipal() {
    printf("\n");
    printf("===================================================\n");
    printf("||          ARTEMIS · CENTRO DE CONTROL          ||\n");
    printf("===================================================\n");
    printf("||  1) Configurar misión                         ||\n");
    printf("||  2) Iniciar lanzamiento                       ||\n");
    printf("||  3) Panel de análisis                         ||\n");
    printf("||  4) Ayuda / Glosario                          ||\n");
    printf("||  5) Salir                                     ||\n");
    printf("===================================================\n");
}

ParametrosMision configurarMision(ParametrosMision actual) {
    int opcion;
    printf("\n--- CONFIGURACIÓN DE LA MISIÓN ---\n");
    printf("1) Cargar valores por defecto oficiales (Artemis I)\n");
    printf("2) Personalizar parametros manualmente\n");
    opcion = leerEnteroSeguro("Seleccione una opcion (1-2): ", 1, 2);

   if (opcion == 1) { //se reasignan los valores por si el usuario desea reestablecer lo valores por defecto
        actual.a1 = 20.0;
        actual.t1 = 120.0;
        actual.a2 = 12.5;
        actual.h_obj = 160000.0;
        actual.g = 9.80;
        printf("\nParametros oficiales cargados con exito.\n");
    } else {
        printf("\nIngrese los nuevos parametros:\n");
        actual.a1 = leerDoubleSeguro("Aceleracion fase 1 a1 (m/s^2) [> 9.8]: ", 9.81, 100.0);
        actual.t1 = leerDoubleSeguro("Tiempo de desacople SRB t1 (s) [> 0]: ", 1.0, 1000.0);
        actual.a2 = leerDoubleSeguro("Aceleracion fase 2 a2 (m/s^2) [> 0]: ", 0.1, 100.0);
        actual.h_obj = leerDoubleSeguro("Altitud objetivo h (m): ", 1000.0, 1000000.0);
        actual.g = leerDoubleSeguro("Gravedad local g (m/s^2): ", 1.0, 30.0);
    }
    return actual;
}

/* ---------- FUNCIÓN DE CÁLCULO ---------- */
/** 
 * FUNCIÓN DE CÁLCULO:
 *Aísla todos los cálculos de MRUV y devuelve una estructura con los resultados listos.
*/
ResultadosTeoricos calcularResultadosTeoricos(ParametrosMision m) {
    ResultadosTeoricos r;
    // 1. Velocidad y altitud al final de la Fase 1
    r.v1 = m.a1 * m.t1;
    r.y1 = 0.5 * m.a1 * pow(m.t1, 2);

    //2. Velocidad al apagado de motores (MECO)
    double v2_cuadrado = pow(r.v1, 2) + 2 * m.a2 * (m.h_obj - r.y1);
    r.v2 = (v2_cuadrado > 0) ? sqrt(v2_cuadrado) : 0.0;

    //3. Altitud máxima en caída libre (inercia)
    double d_extra = pow(r.v2, 2) / (2 * m.g);
    r.h_max = m.h_obj + d_extra;

    //4. Fueeza G experimentada por la tripulación
    r.g_force = m.a1 / m.g;

    return r;
}
