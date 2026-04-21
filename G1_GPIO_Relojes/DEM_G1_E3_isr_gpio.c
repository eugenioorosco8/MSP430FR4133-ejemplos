/*
 * DEM_G1_E3_isr_gpio.c
 * Gabinete 1 - Ejercicio 3: Interrupciones GPIO con ISR (DEMOSTRATIVO)
 *
 * Al presionar S1 (P1.2) se genera una interrupcion por flanco H->L.
 * La ISR del PORT1 toglea LED1 (P1.0).
 * El CPU queda en LPM0 entre interrupciones.
 *
 * Secuencia de 7 pasos documentados en clase.
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* LED1 como salida */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* S1: entrada pull-up */
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);

    /* Limpiar flag, configurar flanco y habilitar INT */
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN2,
                             GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN2);

    /* LPM0 + GIE */
    __bis_SR_register(LPM0_bits | GIE);

    while (1);
    return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void ISR_Puerto1(void)
{
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
