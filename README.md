# MSP430FR4133 - Ejemplos para CCS con DriverLib

Coleccion de ejemplos en C para el microcontrolador **MSP430FR4133**
(LaunchPad MSP-EXP430FR4133), desarrollados para usar con
**Code Composer Studio 11** y la libreria **DriverLib MSP430FR2xx_4xx**.

## Contenido

### Guias de Lectura (GL_)
Ejemplos demostrativos que acompanan las guias teoricas de cada unidad.

| Carpeta | Tema |
|---|---|
| `GL_U0_Intro/` | Introduccion, herramientas, primer proyecto |
| `GL_U1_GPIO/` | GPIO, entradas, salidas, interrupciones |
| `GL_U2_Relojes_GPIO_INT/` | Sistema de relojes (CS), FLL, LPM |
| `GL_U3_TimerA/` | Timer_A: modos, PWM, captura |
| `GL_U4_ADC_LCD/` | ADC de 10 bits, LCD segmentado FH-1138P |
| `GL_U5_Comunicacion/` | UART, SPI, I2C (eUSCI) |

### Gabinetes de Practica (DEM_)
Ejemplos demostrativos para las sesiones de laboratorio.

| Carpeta | Tema |
|---|---|
| `G1_GPIO_Relojes/` | GPIO, debounce, ISR, relojes |
| `G2_TimerA_PWM/` | Timer_A, PWM, captura |
| `G3_ADC_LCD/` | ADC, LCD, temperatura interna |
| `G4_Serie/` | UART, SPI, I2C |

## Requisitos

- **CCS 11** (Code Composer Studio)
- Proyecto base tipo `emptyProject` con DriverLib agregada
- Para ejemplos de LCD: copiar `hal_LCD.h` y `hal_LCD.c` al proyecto

## Uso rapido

1. Abrir CCS, crear un `emptyProject` para MSP430FR4133
2. Agregar la DriverLib al proyecto
3. Copiar el `.c` deseado como `main.c`
4. Para ejemplos LCD: agregar tambien `hal_LCD.h` y `hal_LCD.c`
5. Compilar y depurar

## Convenciones de nombres

| Prefijo | Tipo | Descripcion |
|---|---|---|
| `GL_` | Guia de Lectura | Ejemplo de unidad teorica |
| `DEM_` | Demostrativo | Ejemplo completo para el docente |

## Target

MSP430FR4133 - Familia FR2xx/FR4xx  
Frecuencias tipicas: MCLK = 1 MHz (DCO default) o hasta 16 MHz con FLL
