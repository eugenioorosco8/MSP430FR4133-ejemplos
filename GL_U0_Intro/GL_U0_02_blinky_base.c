/*
 * GL_U0_02_blinky_base.c
 * Unidad 0 - Introduccion al MSP430FR4133 y DriverLib
 *
 * Blinky - "Hola Mundo" embebido
 * LED1 (P1.0) parpadea con retardo por software de ~500 ms a 1 MHz.
 * Es el patron base de todo proyecto en el curso.
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
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        __delay_cycles(500000);   // ~500 ms con MCLK = 1 MHz (DCO por defecto)
    }

    return 0;
}
