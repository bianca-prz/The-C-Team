#ifndef simulador_h
#define simulador_h


// Estructura para almacenar los parámetros de la misión (Sin valores iniciales)
typedef struct {
    double a1;      // Aceleración fase 1
    double t1;      // Tiempo de desacople SRB (s)
    double a2;      // Aceleración fase 2 
    double h_obj;   // Altitud objetivo de apagado 
    double g;       // Aceleración gravitacional 
} ParametrosMision;


#endif
