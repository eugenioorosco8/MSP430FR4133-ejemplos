/*
 * GL_U4_04_adc_isr.c
 * Unidad 4 - ADC por interrupcion (patron eficiente con LPM0)
 *
 * La conversion ADC se inicia por software. Al completarse, la ISR ADC_VECTOR
 * lee el resultado, despierta al CPU y vuelve a disparar la siguiente conversion.
 * El CPU queda en LPM0 mientras el ADC trabaja.
 *
 * Esta es la forma mas eficiente en lugar de polling activo:
 *   - CPU en LPM0 (dormido) durante la conversion
 *   - ISR despierta al CPU con __bic_SR_register_on_exit(LPM0_bits)
 *   - main() procesa el resultado y vuelve a LPM0
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

volatile uint16_t nueva_muestra = 0;
volatile uint8_t  hay_muestra   = 0;

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* Pin A0 como entrada analogica */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1, GPIO_PIN0, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configurar ADC */
    ADC_init(ADC_BASE,
             ADC_SAMPLEHOLDSOURCE_SC,
             ADC_CLOCKSOURCE_ADCOSC,
             ADC_CLOCKDIVIDER_1);
    ADC_enable(ADC_BASE);
    ADC_setupSamplingTimer(ADC_BASE,
                           ADC_CYCLEHOLD_16_CYCLES,
                           ADC_MULTIPLESAMPLESDISABLE);
    ADC_configureMemory(ADC_BASE,
                        ADC_INPUT_A0,
                        ADC_VREFPOS_AVCC,
                        ADC_VREFNEG_AVSS);

    /* Habilitar interrupcion del ADC */
    ADC_clearInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT);
    ADC_enableInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT);

    while (1)
    {
        /* Iniciar conversion y dormir */
        ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);
        __bis_SR_register(LPM0_bits | GIE);

        /* La ISR deposita el resultado y despierta al CPU */
        if (hay_muestra)
        {
            hay_muestra = 0;
            uint32_t mv = (uint32_t)nueva_muestra * 3300UL / 1023UL;
            /* Procesar mv ... */
            __no_operation();   // breakpoint aqui
        }
    }

    return 0;
}

/* ISR del ADC - se ejecuta cuando la conversion finaliza */
#pragma vector = ADC_VECTOR
__interrupt void ISR_ADC(void)
{
    ADC_clearInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT);

    nueva_muestra = ADC_getResults(ADC_BASE);
    hay_muestra   = 1;

    /* Despertar al CPU */
    __bic_SR_register_on_exit(LPM0_bits);
}
