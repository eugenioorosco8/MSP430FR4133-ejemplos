/*
 * GL_U2_04_polling_debounce.c
 * Unidad 2 - Polling con antirrebote (debounce)
 *
 * Al presionar S1 (P1.2) se toglea LED1 (P1.0).
 * Incluye doble verificacion de boton y espera de liberacion
 * para evitar detecciones multiples por rebote mecanico.
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
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);  // S1

    while (1)
    {
        /* Esperar flanco descendente (boton presionado) */
        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2) == GPIO_INPUT_PIN_LOW)
        {
            __delay_cycles(50000);   // ~50 ms de debounce con MCLK = 1 MHz

            /* Verificar que sigue presionado (no fue ruido) */
            if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2) == GPIO_INPUT_PIN_LOW)
            {
                GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

                /* Esperar a que el usuario suelte el boton */
                while (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2) == GPIO_INPUT_PIN_LOW);
                __delay_cycles(50000);   // debounce en flanco de subida
            }
        }
    }

    return 0;
}
