/*
 * GL_U2_05_isr_gpio.c
 * Unidad 2 - Interrupcion de GPIO (ISR)
 *
 * Al presionar S1 (P1.2) se genera una interrupcion por flanco descendente.
 * La ISR toglea LED1 (P1.0).
 * El CPU queda en LPM0 entre interrupciones.
 *
 * IMPORTANTE: siempre limpiar el flag de interrupcion al inicio de la ISR
 * para evitar reentrada inmediata.
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

    /* S1 (P1.2): entrada pull-up, interrupcion en flanco H->L */
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN2,
                             GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN2);

    /* Habilitar interrupciones globales y entrar en bajo consumo */
    __bis_SR_register(LPM0_bits | GIE);

    /* El codigo no llega aqui normalmente */
    while (1);

    return 0;
}

/* ISR del Puerto 1 */
#pragma vector = PORT1_VECTOR
__interrupt void ISR_Puerto1(void)
{
    /* Limpiar flag PRIMERO para no perder el proximo flanco */
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);

    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Salir de LPM si estaba en bajo consumo (opcional segun diseno) */
    /* __bic_SR_register_on_exit(LPM0_bits); */
}
