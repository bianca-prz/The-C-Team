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
}