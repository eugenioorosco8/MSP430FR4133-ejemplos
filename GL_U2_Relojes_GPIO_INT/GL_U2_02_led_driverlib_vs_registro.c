/*
 * GL_U2_02_led_driverlib_vs_registro.c
 * Unidad 2 - GPIO como salida digital
 *
 * Muestra el acceso a LED1 (P1.0) y LED2 (P4.0) con dos enfoques:
 *   Bloque A: acceso directo a registros (P1DIR, P1OUT, P4DIR, P4OUT)
 *   Bloque B: equivalente con DriverLib
 *
 * Descomentando el bloque B y comentando el A se obtiene el mismo resultado.
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* ---- BLOQUE A: acceso directo a registros ---- */
    P1DIR |= BIT0;           // P1.0 salida (LED1)
    P4DIR |= BIT0;           // P4.0 salida (LED2)

    while (1)
    {
        P1OUT |= BIT0;       // LED1 ON
        P4OUT &= ~BIT0;      // LED2 OFF
        __delay_cycles(500000);

        P1OUT &= ~BIT0;      // LED1 OFF
        P4OUT |= BIT0;       // LED2 ON
        __delay_cycles(500000);

        /* Toggle alternativo con XOR:
         * P1OUT ^= BIT0;
         * P4OUT ^= BIT0; */
    }

    /* ---- BLOQUE B: equivalente con DriverLib (comentado) ---- */
    /*
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);   // LED1
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0);   // LED2

    while (1)
    {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
        __delay_cycles(500000);

        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0);
        __delay_cycles(500000);
    }
    */

    return 0;
}
