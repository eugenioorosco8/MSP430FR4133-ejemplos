/*
 * DEM_G3_E2_adc_isr.c
 * Gabinete 3 - Ejercicio 2: ADC por interrupcion - patron eficiente (DEMOSTRATIVO)
 *
 * El CPU queda en LPM0 mientras el ADC convierte.
 * La ISR ADC_VECTOR lee el resultado y despierta al CPU.
 * main() procesa el dato y vuelve a dormir.
 *
 * Patron:
 *   main -> ADC_startConversion -> LPM0
 *   ISR  -> getResults -> __bic_SR_register_on_exit(LPM0_bits)
 *   main -> procesa -> vuelve a LPM0
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

    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1, GPIO_PIN0, GPIO_PRIMARY_MODULE_FUNCTION);  // A0

    ADC_init(ADC_BASE, ADC_SAMPLEHOLDSOURCE_SC,
             ADC_CLOCKSOURCE_ADCOSC, ADC_CLOCKDIVIDER_1);
    ADC_enable(ADC_BASE);
    ADC_setupSamplingTimer(ADC_BASE, ADC_CYCLEHOLD_16_CYCLES,
                           ADC_MULTIPLESAMPLESDISABLE);
    ADC_configureMemory(ADC_BASE, ADC_INPUT_A0,
                        ADC_VREFPOS_AVCC, ADC_VREFNEG_AVSS);

    ADC_clearInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT);
    ADC_enableInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT);

    while (1)
    {
        ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);
        __bis_SR_register(LPM0_bits | GIE);

        if (hay_muestra)
        {
            hay_muestra = 0;
            uint32_t mv = (uint32_t)nueva_muestra * 3300UL / 1023UL;
            __no_operation();   // breakpoint: ver nueva_muestra y mv
        }
    }

    return 0;
}

#pragma vector = ADC_VECTOR
__interrupt void ISR_ADC(void)
{
    ADC_clearInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT);
    nueva_muestra = ADC_getResults(ADC_BASE);
    hay_muestra   = 1;
    __bic_SR_register_on_exit(LPM0_bits);
}
