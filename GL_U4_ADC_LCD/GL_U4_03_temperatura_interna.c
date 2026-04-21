/*
 * GL_U4_03_temperatura_interna.c
 * Unidad 4 - ADC: sensor de temperatura interno
 *
 * El FR4133 tiene un sensor de temperatura interno conectado al canal A12.
 * Requiere:
 *   - Referencia interna (PMM_enableInternalReference + espera de estabilizacion)
 *   - Tiempo de muestreo minimo de 192 ciclos (especificacion del datasheet)
 *   - ADC_VREFPOS_INT como referencia positiva
 *
 * Formula de temperatura (calibracion tipica, no usa constantes de fabrica):
 *   T [gradosC] = (raw - 673) * 10 / 24
 *
 * Para mayor precision usar las constantes de calibracion de TLV:
 *   TLV->CAL_ADC_15T30 y TLV->CAL_ADC_15T85
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* Habilitar referencia interna de 1.5V */
    PMM_enableInternalReference();

    /* Habilitar sensor de temperatura (deshabilitado por defecto) */
    PMM_enableTempSensor();

    /* Esperar a que la referencia se estabilice */
    while (PMM_REFGEN_NOTREADY == PMM_getVariableReferenceVoltageStatus());

    /* Inicializar ADC */
    ADC_init(ADC_BASE,
             ADC_SAMPLEHOLDSOURCE_SC,
             ADC_CLOCKSOURCE_ADCOSC,
             ADC_CLOCKDIVIDER_1);
    ADC_enable(ADC_BASE);

    /* Tiempo de muestreo minimo para sensor de temperatura: 192 ciclos */
    ADC_setupSamplingTimer(ADC_BASE,
                           ADC_CYCLEHOLD_192_CYCLES,
                           ADC_MULTIPLESAMPLESDISABLE);

    /* Canal A12 (sensor interno), referencia interna positiva */
    ADC_configureMemory(ADC_BASE,
                        ADC_INPUT_TEMPSENSOR,
                        ADC_VREFPOS_INT,
                        ADC_VREFNEG_AVSS);

    volatile uint16_t raw_temp = 0;       // volatile: visible en debugger
    volatile int32_t  temperatura_c = 0;

    while (1)
    {
        ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);
        while (ADC_isBusy(ADC_BASE) == ADC_BUSY);

        raw_temp = ADC_getResults(ADC_BASE);

        /* Formula tipica de temperatura */
        temperatura_c = ((int32_t)raw_temp - 673L) * 10L / 24L;

        /* Ver raw_temp y temperatura_c en Debug > Expressions */
        __no_operation();

        __delay_cycles(1000000);   // esperar 1 s entre mediciones (~1 MHz)
    }

    return 0;
}
