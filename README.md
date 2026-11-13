# The C-Team 🚀
Situación - Problema 2: Inserción y Estabilidad en Órbita Terrestre Baja — LEO (MCU y  Gravitación)

## 👥 Integrantes
* **Araujo Ledezma, Pilar** - Legajo: 31527
* **Perez, Bianca Belen** - Legajo: 31883
* **Sosa, Milena Tali** - Legajo:  31041

## 📚 Información Académica

**Universidad Tecnológica Nacional - Facultad Regional Resistencia**

<p align="center">
  <img width="222" height="227" alt="log-UTN 2" src="https://github.com/user-attachments/assets/4e1f2f7f-0178-444f-911a-454a80a7fc89" />
</p>

* **Cátedra:** Algoritmos y Estructuras de Datos.
* **Comisión:** C
* **Año:** 2026
  
## 📑 Descripción del Escenario 
### *Situación - Problema 1: Ascenso y Propulsión con el SLS (MRUV y Desprendimiento)*

Durante el despegue desde el Centro Espacial Kennedy, el cohete Space Launch System (SLS) eleva a la nave Orion partiendo del reposo (v<sub>0</sub> = 0,0 m/s) en la plataforma de lanzamiento (y<sub>0</sub> = 0,0 m). En la primera fase de ascenso vertical rectilíneo, el cohete acelera hacia arriba con una aceleración constante de a<sub>1</sub> = 20,0 m/s<sup>2</sup> impulsado por los motores principales y los dos cohetes de combustible sólido (SRB). A los t<sub>1</sub> = 120,0 s (2,00 min) de vuelo, los cohetes SRB agotados se desacoplan limpiamente. A partir de ese instante, el motor del núcleo principal continúa acelerando la nave a  a<sub>2</sub> = 12,5 m/s<sup>2</sup> hasta alcanzar una altitud de h = 160,0 km (160.000,0 m), donde los motores se apagan. Se desprecia la resistencia del aire en la alta atmósfera.

#### Fases del Ascenso:
1. **Fase 1 (Aceleración con SRB):** El cohete acelera verticalmente con a<sub>1</sub> = 20,0 m/s<sup>2</sup> impulsado por los motores principales y los cohetes de combustible sólido. A los t<sub>1</sub> = 120,0 s (2,00 min), los SRB se desacoplan de forma limpia.
2. **Fase 2 (Núcleo Principal):** El motor principal continúa acelerando la nave a a<sub>2</sub> = 12,5 m/s<sup>2</sup> hasta alcanzar la altitud objetivo de h = 160.000,0 m (160,0 km), momento en el cual se produce el apagado de motores (MECO).
3. **Fase 3 (Inercia y Caída Libre - Opcional/Contingencia):** Si los motores fallaran al llegar a los 160,0 km, la nave continúa por inercia bajo la aceleración de la gravedad (g = 9,80 m/s<sup>2</sup>) hasta alcanzar una altitud máxima (h<sub>máx</sub>).

## 🎮 Reglas del Juego
El simulador actúa como una consola interactiva de Centro de Control de Misión, donde el operador puede configurar parámetros físicos, monitorear el ascenso en tiempo real y responder ante contingencias:
1. **Configuración Inicial (Pre-Lanzamiento):**
* El usuario puede operar con los valores oficiales de Artemis I o personalizar cada parámetro dentro de rangos físicos seguros.
* Regla de Despegue: La aceleración inicial (a<sub>1</sub>) debe superar la gravedad local (g) para que el cohete logre vencer su propio peso y elevarse.

2. **Telemetría en Vivo (Tablero Fijo):**
* Una vez iniciada la secuencia (T-10 a T-0), la pantalla se actualiza dinámicamente en el mismo lugar sin desplazamientos infinitos, mostrando cronómetro T+ , barra de progreso visual de altitud, velocidad instantánea y carga en fuerzas g.

3. **Controles del Operador en Tiempo Real:**
* **[P] Pausa:** Congela el tiempo de la misión para inspeccionar variables instantáneas.
* **[X] Multiplicador de Velocidad:** Alterna la escala temporal entre x1 (tiempo real), x10 y x50 para agilizar vuelos prolongados.
* **[F] Simular Falla de Motores:** Corta el empuje repentinamente en pleno vuelo, activando el cálculo inmediato de trayectoria balística y estimando el apogeo máximo que alcanzará la cápsula Orion por inercia.
* **[Q] Abortar Misión:** Cancela el lanzamiento de emergencia por seguridad.

4. **Objetivo de Victoria:**
* Alcanzar la altitud objetivo (h<sub>obj</sub>), sin fallas ni abortos para ejecutar con éxito el corte del motor principal (MECO).

## 📐 Modelo Físico y Cálculos Analíticos (MRUV)

El motor físico del simulador resuelve las ecuaciones cinemáticas del **Movimiento Rectilíneo Uniformemente Variado (MRUV)** divididas por tramos de vuelo:

* **1. Condiciones al desacople de los SRB (t = t<sub>1</sub>):**
  Calcula la velocidad y la altura acumulada al finalizar la primera etapa de empuje conjunto:
  * **Rapidez al desacople:** 
     v<sub>1</sub>= a<sub>1</sub> * t<sub>1</sub>
  * **Altitud al desacople:** 
     y<sub>1</sub>= 0.5 * a<sub>1</sub> * (t<sub>1</sub>^2)

* **2. Rapidez máxima al apagado de motores (MECO):**
  Determina la velocidad final que alcanza la cápsula Orion al llegar a la altitud objetivo (h<sub>obj</sub>)impulsada únicamente por el núcleo principal:
  * **Rapidez final:** 
    v<sub>2</sub>= √( (v<sub>1</sub> ^ 2) + 2 * a<sub>2</sub> * (h<sub>obj</sub> - y<sub>1</sub>) )

* **3. Altitud máxima en caída libre por contingencia (h<sub>max</sub>):**
  En caso de falla repentina de motores al alcanzar la altura objetivo, calcula el apogeo máximo que logra la nave por inercia antes de comenzar a descender por efecto de la gravedad (`g`):
  * **Altura extra por inercia:** d<sub>extra</sub> = (v<sub>2</sub>^ 2) / (2 * g)
  * **Apogeo total:** 
    h<sub>max</sub> = h<sub>obj</sub> + d<sub>extra</sub>

* **4. Carga gravitacional sobre la tripulación (Fuerza g):**
  Expresa la aceleración máxima soportada durante el despegue como múltiplo de la gravedad terrestre:
  * **Múltiplo de gravedad:** Carga g = a<sub>1</sub> / g

## 🏗️ Arquitectura Modular del Proyecto
El código está estructurado siguiendo principios de bajo acoplamiento y alta cohesión, separando la lógica de interfaz, la física y la seguridad de entrada en módulos independientes:
#### artemis-simulator/ 
- main.c            # Director principal: inicializa la misión y gestiona el menú.
- simulador.h       # Interfaz pública: estructuras de datos (ParametrosMision) y prototipos.
- simulador.c       # Núcleo lógico: tablero de telemetría en vivo, MRUV, panel de análisis y glosario.
- validaciones.h    # Cabecera del sistema de entrada segura.
- validaciones.c    # Blindaje de I/O (fgets + sscanf) contra entradas inválidas o fuera de rango.

**Características Técnicas Destacadas:**
  * Multiplataforma (Windows / Linux / macOS): Implementa compilación condicional (#ifdef _WIN32) para gestionar retardos temporales (Sleep/usleep) y lectura de teclado no bloqueante (conio.h en Windows / termios en modo raw en POSIX) sin requerir dependencias externas.
  * Panel de Análisis Multinivel: Permite consultar los resultados teóricos en 3 niveles de profundidad: 1) Resultado Rápido, 2) Desarrollo Matemático Paso a Paso y 3) Interpretación Física Conceptual.
  * Glosario Interactivo: Submenú integrado diseñado para usuarios sin conocimientos previos de física o cohetería.

## ⚙️ Compilación y Ejecución
**Requisitos Previos**
 - Compilador GCC (o compatible con estándar C99/C11).
 - Terminal con soporte para secuencias de escape ANSI (Windows Terminal, PowerShell moderno, Linux Bash o macOS Terminal).
**Comando de Compilación**
Desde la raíz del proyecto, ejecuta el siguiente comando (incluyendo -lm para enlazar la librería matemática <math.h>):

`gcc main.c simulador.c validaciones.c -o artemis_sim -lm`

**Nota importante sobre la compilación (Librería Matemática)**

Al compilar el proyecto en sistemas basados en GCC, es **estrictamente necesario** incluir el indicador `-lm` al final del comando. 
* ¿Por qué se requiere?: Archivos como `simulador.c` utilizan funciones matemáticas avanzadas de la biblioteca estándar de C (`<math.h>`), tales como la raíz cuadrada (`sqrt`) para velocidades y la potenciación (`pow`) para cálculos de altitud. 
* ¿Qué sucede si se omite?: Si intentas compilar ejecutando únicamente el comando base sin el `-lm`, el compilador se detendrá y arrojará un **error de enlace (*linker error*)** indicando una referencia indefinida a las funciones matemáticas (`undefined reference to 'sqrt'` o `'pow'`). Esto ocurre porque GCC no vincula la librería matemática de forma predeterminada por motivos de optimización de sistemas.

---

### 🤖 Uso de Inteligencia Artificial en el Desarrollo

Durante el desarrollo de este simulador de la misión Artemis, se empleó Inteligencia Artificial como una herramienta de colaboración técnica, diseño y depuración de código bajo los siguientes criterios: 

* **Arquitectura Modular y Buenas Prácticas:** Asistencia en la estructuración de la separación de responsabilidades (`main.c`, `simulador.c`, `validaciones.c` y sus respectivas cabeceras `.h`) para garantizar bajo acoplamiento y alta cohesión.
* **Planteamiento Inicial del Escenario:** Consulta para obtener ideas iniciales sobre cómo comenzar a plantear y estructurar la lógica del escenario de la misión en código C.
* **Resolución de Interfaz y I/O de Bajo Nivel:** Apoyo en el diseño del tablero de telemetría de actualización estática en tiempo real mediante secuencias de escape ANSI (`\033[H`), y en la implementación de la lectura de teclado no bloqueante condicional (`conio.h` para Windows y `termios` en modo *raw* para entornos POSIX/Linux).
* **Validación de Modelos Físicos:** Verificación de las ecuaciones cinemáticas del Movimiento Rectilíneo Uniformemente Variado (MRUV), cálculo de tramos por fases, eventos de desacople de SRB, corte de motor principal (MECO) y estimación balística de apogeo por inercia ante fallas.
* **Compilación y Depuración:** Utilización como apoyo en el proceso de compilación, en la detección de errores de sintaxis y en la revisión o depuración de estructuras desarrolladas.
* **Documentación y Glosario Didáctico:** Creación y estructuración de los contenidos del glosario interactivo multinivel, diseñado para traducir conceptos complejos de física aeroespacial a un lenguaje accesible para no expertos.



