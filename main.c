#include <stdio.h>
#include "simulador.h"
#include "validaciones.h"


int main() {
    // Inicializar misión con los valores por defecto del enunciado oficial
    // Esto asegura que el simulador siempre arranque con datos válidos y listos para operar, 
    //incluso si el usuario decide no usar el menú de configuración manual
  ParametrosMision mision = {
    .a1 = 20.0,
    .t1 = 120.0,
    .a2 = 12.5,
    .h_obj = 160000.0,
    .g = 9.80
};
int opcion;

    do {
        mostrarMenuPrincipal();
        opcion = leerEnteroSeguro("Seleccione una opcion del menu (1-5): ", 1, 5);

        switch (opcion) {
            case 1:
                mision = configurarMision(mision);
                break;
            case 2:
                ejecutarSimulacionEnVivo(mision);
                break;
            case 3:
                mostrarPanelAnalisis(mision);
                break;
            case 4:
                mostrarAyudaGlosario();
                break;
            case 5:
                printf("\n[CENTRO DE CONTROL] Desconectando sistemas... Mision finalizada con exito.\n");
                break;
            default:
                printf("\n[ERROR] Opcion fuera de rango.\n");
        }
    } while (opcion != 5);

    return 0;
}