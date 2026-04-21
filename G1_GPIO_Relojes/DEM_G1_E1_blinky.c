/*
 * DEM_G1_E1_blinky.c
 * Gabinete 1 - Ejercicio 1: Blinky (DEMOSTRATIVO)
 *
 * LED1 (P1.0) parpadea con retardo por software de ~500 ms.
 * Codigo completo que el docente muestra y ejecuta en clase.
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    while (1)
    {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        __delay_cycles(500000);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        __delay_cycles(500000);
    }

    return 0;
}
