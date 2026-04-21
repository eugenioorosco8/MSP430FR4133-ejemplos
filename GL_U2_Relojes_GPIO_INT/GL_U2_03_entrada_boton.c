/*
 * GL_U2_03_entrada_boton.c
 * Unidad 2 - GPIO como entrada digital
 *
 * Configura S1 (P1.2) como entrada con pull-up interno.
 * Mientras el boton esta presionado, LED1 (P1.0) enciende.
 * Sin presionar, LED1 apaga.
 *
 * Muestra ambos enfoques: registro directo (Bloque A) y DriverLib (Bloque B).
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
    P1DIR &= ~BIT2;    // P1.2 como entrada (S1)
    P1REN |=  BIT2;    // habilitar resistor interno
    P1OUT |=  BIT2;    // configurar como pull-up (nivel alto = no presionado)

    P1DIR |=  BIT0;    // P1.0 como salida (LED1)

    while (1)
    {
        if (!(P1IN & BIT2))           // boton presionado = nivel bajo
            P1OUT |=  BIT0;           // LED ON
        else
            P1OUT &= ~BIT0;           // LED OFF
    }

    /* ---- BLOQUE B: equivalente con DriverLib (comentado) ---- */
    /*
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);  // S1
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);                   // LED1

    while (1)
    {
        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2) == GPIO_INPUT_PIN_LOW)
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        else
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
    */

    return 0;
}
