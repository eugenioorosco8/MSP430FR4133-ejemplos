/*
 * GL_U1_01_estructura_main.c
 * Unidad 1 - Arquitectura y CPU del MSP430
 *
 * Estructura minima de un programa C para MSP430FR4133:
 *  - Detener el watchdog timer
 *  - Desbloquear GPIO (LPM5 lock)
 *  - Habilitar interrupciones globales
 *  - Bucle infinito (el CPU queda disponible para ISRs)
 *
 * Nota: el vector de RESET en el linker script (lnk_msp430fr4133.cmd)
 * apunta a _c_int00 (startup de C runtime), que luego llama a main().
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    /* 1. Detener watchdog - obligatorio, de lo contrario resetea en ~32 ms */
    WDT_A_hold(WDT_A_BASE);

    /* 2. Desbloquear pines GPIO (quedan bloqueados al salir de LPM5) */
    PMM_unlockLPM5();

    /* 3. Habilitar interrupciones globales (bit GIE del registro SR) */
    __enable_interrupt();

    /* 4. Bucle infinito - la aplicacion real pondria el CPU en LPM aqui */
    while (1);

    return 0;
}
