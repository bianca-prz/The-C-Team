#include <stdio.h>
#include <math.h>
#include "simulador.h"
#include "validaciones.h"
 
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #define SLEEP_MS(ms) Sleep(ms)
#else
    #include <unistd.h>
    #include <termios.h>
    #include <sys/select.h>
    #define SLEEP_MS(ms) usleep((ms) * 1000)
#endif

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

#define EOL "\033[K\n"   /* borra el resto de la linea y salta de linea */
 
#ifdef _WIN32
 
static void prepararTerminal(void) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    if (GetConsoleMode(h, &modo)) {
        SetConsoleMode(h, modo | 0x0004); 
    }
}
static void restaurarTerminal(void) {
    while (_kbhit()) _getch();  
}
static int hayTecla(void)  { return _kbhit(); }
static int leerTecla(void) { return _getch(); }
 
#else
 
static struct termios termOriginal;
 
static void prepararTerminal(void) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &termOriginal);
    raw = termOriginal;
    raw.c_lflag &= ~(ICANON | ECHO);   
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}
static void restaurarTerminal(void) {
    tcflush(STDIN_FILENO, TCIFLUSH);   
    tcsetattr(STDIN_FILENO, TCSANOW, &termOriginal);
}
static int hayTecla(void) {
    fd_set fds;
    struct timeval tv = {0, 0};
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}
static int leerTecla(void) {
    unsigned char c;
    return (read(STDIN_FILENO, &c, 1) == 1) ? c : -1;
}
 
#endif
 
void ejecutarSimulacionEnVivo(ParametrosMision m) {
   
    printf("\n[CENTRO DE CONTROL] Iniciando secuencia de lanzamiento...\n");
    for (int t = 10; t >= 0; t--) {
        printf("\rT- %02d s   [ESTADO: Preparando ignicion]", t);
        fflush(stdout);
        SLEEP_MS(300);
    }
    printf("\rT- 00 s   >>> IGNICION Y DESPEGUE DEL SLS <<<\n\n");
    SLEEP_MS(1000);
 
    /* 2. Variables de estado */
    double t_actual = 0.0;
    double altitud = 0.0;
    double velocidad = 0.0;
    double aceleracion_actual = m.a1;
    int fase = 1;
    int velocidad_sim = 1;   
    int pausado = 0;
    int abortado = 0;
    int fallado = 0;
    int meco = 0;
    double dt = 0.5;
 
    /* Valores al momento del desacople (constantes) */
    double v1_srb = m.a1 * m.t1;
    double y1_srb = 0.5 * m.a1 * m.t1 * m.t1;
 
    prepararTerminal();
    printf("\033[2J");   /* limpia la pantalla una sola vez */
 
    while (1) {
        while (hayTecla()) {
            int c = leerTecla();
            if (c == 'p' || c == 'P') {
                pausado = !pausado;
            } else if (c == 'x' || c == 'X') {
                if (velocidad_sim == 1) velocidad_sim = 10;
                else if (velocidad_sim == 10) velocidad_sim = 50;
                else velocidad_sim = 1;
            } else if (c == 'f' || c == 'F') {
                fallado = 1;
            } else if (c == 'q' || c == 'Q') {
                abortado = 1;
            }
        }
        if (abortado || fallado) break;
 
        if (fase == 1 && t_actual >= m.t1) {
            fase = 2;
            printf("\033[H\033[J");
            printf("\n>>> [EVENTO] DESACOPLE DE SRB CONFIRMADO a t+%.1f s <<<\n", t_actual);
            fflush(stdout);
            SLEEP_MS(1500);
        }
 
        if (fase == 1) {
            aceleracion_actual = m.a1;
            velocidad = m.a1 * t_actual;
            altitud = 0.5 * m.a1 * t_actual * t_actual;
            if (altitud >= m.h_obj) {   /* objetivo alcanzado dentro de la fase 1 */
                altitud = m.h_obj;
                velocidad = sqrt(2 * m.a1 * m.h_obj);
                meco = 1;
            }
        } else {
            double t2 = t_actual - m.t1;
            aceleracion_actual = m.a2;
            velocidad = v1_srb + m.a2 * t2;
            altitud = y1_srb + v1_srb * t2 + 0.5 * m.a2 * t2 * t2;
            if (altitud >= m.h_obj) {
                altitud = m.h_obj;
                velocidad = sqrt(v1_srb * v1_srb + 2 * m.a2 * (m.h_obj - y1_srb));
                meco = 1;
            }
        }
 
        /* --- TABLERO DE TELEMETRIA --- */
        printf("\033[H");
        printf("===================================================================" EOL);
        printf(" 3. LA SIMULACION EN VIVO (CENTRO DE CONTROL ARTEMIS)" EOL);
        printf("===================================================================" EOL);
        printf(" T+ %05.1f s  |  FASE %d : %s %s" EOL,
               t_actual, fase,
               (fase == 1) ? "PROPULSION CONJUNTA (SRB + NUCLEO)" : "PROPULSION NUCLEO PRINCIPAL",
               pausado ? "[PAUSA]" : "");
        printf("-------------------------------------------------------------------" EOL);
 
        int barra = (int)((altitud / m.h_obj) * 30);
        if (barra > 30) barra = 30;
        printf(" ALTITUD    [");
        for (int i = 0; i < 30; i++) printf(i < barra ? "#" : "-");
        printf("] %6.1f km / %.1f km" EOL, altitud / 1000.0, m.h_obj / 1000.0);
 
        printf(" VELOCIDAD  %-8.1f m/s     | ACEL. : %.1f m/s^2 (%.2f g)" EOL,
               velocidad, aceleracion_actual, aceleracion_actual / m.g);
        printf("-------------------------------------------------------------------" EOL);
 
        if (fase == 1) {
            printf(" SRB: [ACTIVOS]          NUCLEO: [ENCENDIDO]" EOL);
            printf(" Proximo evento: DESACOPLE SRB en %.0f s" EOL,
                   (m.t1 - t_actual > 0) ? m.t1 - t_actual : 0.0);
        } else {
            printf(" SRB: [DESACOPLADOS]     NUCLEO: [ENCENDIDO]" EOL);
            printf(" Proximo evento: MECO (Corte de motores) al alcanzar objetivo" EOL);
        }
 
        printf("-------------------------------------------------------------------" EOL);
        printf(" [P] Pausa   [X] Velocidad (x%d)   [F] Falla motores   [Q] Abortar" EOL, velocidad_sim);
        printf("===================================================================" EOL);
        fflush(stdout);
 
        if (meco) break;
 
        if (!pausado) t_actual += dt * velocidad_sim;
        SLEEP_MS(200 / velocidad_sim);
    }
 
    restaurarTerminal();
 
    if (meco) {
        printf("\n>>> [EVENTO] CORTE DE MOTOR PRINCIPAL (MECO) ALCANZADO <<<\n");
    } else if (fallado) {
        double apogeo = altitud + (velocidad * velocidad) / (2 * m.g);
        printf("\n>>> [ALERTA] FALLA DE MOTORES a t+%.1f s <<<\n", t_actual);
        printf("Altitud al momento de la falla: %.2f m | Apogeo estimado por inercia: %.2f m\n",
               altitud, apogeo);
    } else if (abortado) {
        printf("\n>>> [ABORTO] MISION ABORTADA por el operador a t+%.1f s <<<\n", t_actual);
    }
 
    printf("\n[SIMULACION FINALIZADA] Presione Enter para volver al menu principal...");
    getchar();
}

void mostrarPanelAnalisis (ParametrosMision m) {
 //Calculos analiticos excactos basados en el modelo de MRUV
 double v1 = m.a1 * m.t1;
 double y1 = 0.5 * m.a1 * pow(m.t1, 2);
 double v2_cuadrado = pow(v1, 2) + 2 * m.a2 * (m.h_obj - y1)
 double v2 = (v2_cuadrado > 0) ? sqrt(v2_cuadrado) : 0.0;
 double d_extra = pow(v2, 2) / (2 * m.g);
 double h_max = m.h_obj + d_extra;
 double g_force = m.a1 / m.g;

 int nivel = leerEnteroSeguro("\nSeleccione nivel de profundidad (1: Rapido, 2: Paso a paso, 3: Fisico): ", 1, 3);

 printf("\n===================================================\n");
 printf("     	PANEL DE ANALISIS - RESULTADOS            \n");
 printf("===================================================\n");

 if (nivel == 1) {
  printf("1. Rapidez al desacople (v1): %.2f m/s | Altitud (y1): %.2f m\n", v1 y1);
  printf("2. Rapidez maxima al apagado (v2): %.2f m/s\n", v2);
  printf("3. Altitud maxima en caida libre (h_max): %.2f m\n", h_max);
  printf("4. Aceleracion maxima: %.2f g\n, g_force);

 } else if (nivel == 2) {
   printf("1. v1 = a1 * t1 = %.1f * %.1f = %.2f m/s\n", m.a1, m.t1, v1);
   printf("   y1 = 0.5 * a1 * t1^2 = %.2f m\n", y1);
   printf("2. v2 = sqrt(v1^2 + 2 * a2 * (h - y1)) = %.2f m/s\n", v2);
   printf("3. h_max = h + (v2^2 / (2 * g)) = %.2f m\n", h_max);
   printf("4. Carga g = a1 / g_tierra = %.2f g\n", g_force);

 } else {
   printf("1. En t1=%.0fs los SRB se separan. La nave acumula velocidad y altura.\n", m.t1);
   printf("2. El nucleo continua acelerando hasta apagar motores a los %.0f m.\n", m.h_obj);
   printf("3. Si los motores fallan, la inercia lleva a Orion hasta un apogeo de &.2f m.\n", h_max);
   printf("4. La tripulacion experimenta una fuerza equivalente a %.2f veces su peso.\n", g_force);
 }
 print("===================================================\n");
 printf("Presione Enter para continuar...");
 getchar();
}

void mostrarAyudaGlosario() {
 printf("\n--- GLOSARIO Y AYUDA ---\n")
 printf("- MRUV: Movimiento Rectilineo Uniformemente Variado.\n");
 printf("- SRB: Cohetes de combustible solido auxiliares del SLS.\n");
 printf("- MECO: Corte del motor principal (Main Engine Cut Off).\n");
 printf("- Apogeo: Punto mas alto de la trayectoria antes de descender.\n");
 printf("Presione Enter para volver...");
 getchar();
}
