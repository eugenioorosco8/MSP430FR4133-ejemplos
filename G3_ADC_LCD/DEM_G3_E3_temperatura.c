/*
 * DEM_G3_E3_temperatura.c
 * Gabinete 3 - Ejercicio 3: Sensor de temperatura interno (DEMOSTRATIVO)
 *
 * Lee el sensor de temperatura interno del FR4133 (canal A12).
 * Requiere referencia interna habilitada y 192 ciclos de muestreo.
 * Formula tipica: T[gradosC] = (raw - 673) * 10 / 24
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    PMM_enableInternalReference();
    PMM_enableTempSensor();
    while (PMM_REFGEN_NOTREADY == PMM_getVariableReferenceVoltageStatus());

    ADC_init(ADC_BASE, ADC_SAMPLEHOLDSOURCE_SC,
             ADC_CLOCKSOURCE_ADCOSC, ADC_CLOCKDIVIDER_1);
    ADC_enable(ADC_BASE);
    ADC_setupSamplingTimer(ADC_BASE, ADC_CYCLEHOLD_192_CYCLES,
                           ADC_MULTIPLESAMPLESDISABLE);
    ADC_configureMemory(ADC_BASE, ADC_INPUT_TEMPSENSOR,
                        ADC_VREFPOS_INT, ADC_VREFNEG_AVSS);

    volatile uint16_t raw_temp      = 0;   // volatile: visible en debugger
    volatile int32_t  temperatura_c = 0;

    while (1)
    {
        ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);
        while (ADC_isBusy(ADC_BASE) == ADC_BUSY);

        raw_temp      = ADC_getResults(ADC_BASE);
        temperatura_c = ((int32_t)raw_temp - 673L) * 10L / 24L;

        /* Breakpoint aqui: ver raw_temp y temperatura_c */
        __no_operation();

        __delay_cycles(1000000);   // ~1 s entre mediciones (MCLK=1MHz)
    }

    return 0;
}
