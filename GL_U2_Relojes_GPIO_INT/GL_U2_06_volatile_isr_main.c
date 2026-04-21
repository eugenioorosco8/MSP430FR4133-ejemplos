/*
 * GL_U2_06_volatile_isr_main.c
 * Unidad 2 - Variable volatile compartida entre ISR y main()
 *
 * Patron correcto para comunicar una ISR con el hilo principal:
 *   1. La ISR activa un flag volatile
 *   2. main() verifica el flag, procesa, y vuelve a LPM3
 *
 * La palabra clave volatile impide que el compilador optimice
 * la lectura de la variable (sin ella, el bucle while seria infinito).
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

volatile uint8_t boton_presionado = 0;

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN2,
                             GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN2);

    while (1)
    {
        /* Entrar en bajo consumo esperando la ISR */
        __bis_SR_register(LPM3_bits | GIE);

        /* Al despertar, procesar el evento */
        if (boton_presionado)
        {
            boton_presionado = 0;
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }
    }

    return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void ISR_Puerto1(void)
{
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);

    boton_presionado = 1;

    /* Despertar al CPU para que main() procese el evento */
    __bic_SR_register_on_exit(LPM3_bits);
}
