/*
 * DEM_G3_E1_adc_polling.c
 * Gabinete 3 - Ejercicio 1: ADC canal A0 por polling (DEMOSTRATIVO)
 *
 * Lee el potenciometro en P1.0 (canal A0) cuando se presiona S1 (P1.2).
 * Convierte el resultado raw (0-1023) a milivoltios (0-3300 mV).
 * Poner breakpoint en __no_operation() y ver raw_adc y milivoltios
 * en Debug > Expressions.
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);  // S1

    /* Pin A0 (P1.0) como entrada analogica */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1, GPIO_PIN0, GPIO_PRIMARY_MODULE_FUNCTION);

    ADC_init(ADC_BASE, ADC_SAMPLEHOLDSOURCE_SC,
             ADC_CLOCKSOURCE_ADCOSC, ADC_CLOCKDIVIDER_1);
    ADC_enable(ADC_BASE);
    ADC_setupSamplingTimer(ADC_BASE, ADC_CYCLEHOLD_16_CYCLES,
                           ADC_MULTIPLESAMPLESDISABLE);
    ADC_configureMemory(ADC_BASE, ADC_INPUT_A0,
                        ADC_VREFPOS_AVCC, ADC_VREFNEG_AVSS);

    uint16_t raw_adc    = 0;
    uint32_t milivoltios = 0;

    while (1)
    {
        /* Esperar presion del boton S1 */
        while (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2) == GPIO_INPUT_PIN_HIGH);

        ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);
        while (ADC_isBusy(ADC_BASE) == ADC_BUSY);

        raw_adc     = ADC_getResults(ADC_BASE);
        milivoltios = (uint32_t)raw_adc * 3300UL / 1023UL;

        /* Breakpoint aqui: ver raw_adc (0-1023) y milivoltios (0-3300) */
        __no_operation();

        /* Esperar a que suelte el boton */
        while (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2) == GPIO_INPUT_PIN_LOW);
    }

    return 0;
}
