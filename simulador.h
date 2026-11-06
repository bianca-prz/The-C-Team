#ifndef simulador_h
#define simulador_h


// Estructura para almacenar los parámetros de la misión (Sin valores iniciales)
typedef struct {
    double a1;      // Aceleración fase 1
    double t1;      // Tiempo de desacople SRB (s)
    double a2;      // Aceleración fase 2 
    double h_obj;   // Altitud objetivo de apagado 
    double g;       // Gravedad 
} ParametrosMision;

// Definición de funciones necesarias según menu interactivo 
void mostrarMenuPrincipal();
ParametrosMision configurarMision(ParametrosMision actual); // Centraliza el registro de variables de configuración del cohete (aceleraciones, tiempos, altitud objetivo y gravedad).
void ejecutarSimulacionEnVivo(ParametrosMision m);
void mostrarPanelAnalisis(ParametrosMision m);
void mostrarAyudaGlosario();


#endif
