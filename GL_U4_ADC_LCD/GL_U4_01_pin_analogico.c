/*
 * GL_U4_01_pin_analogico.c
 * Unidad 4 - ADC: habilitacion de pin analogico
 *
 * Muestra como configurar un pin como entrada analogica para el ADC.
 * El FR4133 tiene los canales ADC mapeados a puertos especificos.
 *
 * Mapeo de canales del FR4133:
 *   A0 -> P1.0    A1 -> P1.1    A2 -> P1.2    A3 -> P1.3
 *   A4 -> P1.4    A5 -> P1.5    A6 -> P1.6    A7 -> P1.7
 *   A8 -> P2.0    A9 -> P2.1    A10 -> P2.2   A11 -> P2.3
 *   A12 (temperatura interna) - sin pin fisico
 *
 * NOTA: en DriverLib la constante del puerto para ADC es
 *   GPIO_PORT_ADCx donde x es el numero de canal.
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* Configurar P1.0 (A0) como entrada de funcion de modulo (analogica) */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN0,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    /* Configurar P1.7 (A7) como entrada analogica */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN7,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    /* Con los pines configurados, el ADC puede tomar muestras de A0 o A7.
     * El ejemplo de lectura real esta en GL_U4_02_adc_polling.c */

    while (1);
    return 0;
}
