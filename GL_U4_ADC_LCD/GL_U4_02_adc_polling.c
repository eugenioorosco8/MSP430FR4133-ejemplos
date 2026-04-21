/*
 * GL_U4_02_adc_polling.c
 * Unidad 4 - ADC canal A0 por polling
 *
 * Lee el potenciometro conectado a P1.0 (canal A0) por polling.
 * Al presionar S1 (P1.2) se realiza una conversion.
 * El resultado raw (0-1023) se convierte a milivoltios (0-3300 mV).
 *
 * Configuracion ADC (5 pasos):
 *   1. ADC_init(): reloj, modo single-channel single-conversion
 *   2. ADC_enable()
 *   3. ADC_setupSamplingTimer(): tiempo de muestreo
 *   4. ADC_configureMemory(): canal, referencia
 *   5. ADC_startConversion() + polling del flag
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* Boton S1 (P1.2) */
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);

    /* Pin A0 (P1.0) como entrada analogica */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1, GPIO_PIN0, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Paso 1: inicializar ADC */
    ADC_init(ADC_BASE,
             ADC_SAMPLEHOLDSOURCE_SC,          // inicio de conversion por software
             ADC_CLOCKSOURCE_ADCOSC,           // reloj interno del ADC (~5 MHz)
             ADC_CLOCKDIVIDER_1);

    /* Paso 2: habilitar ADC */
    ADC_enable(ADC_BASE);

    /* Paso 3: configurar tiempo de muestreo (16 ciclos) */
    ADC_setupSamplingTimer(ADC_BASE,
                           ADC_CYCLEHOLD_16_CYCLES,
                           ADC_MULTIPLESAMPLESDISABLE);

    /* Paso 4: canal A0, referencia VCC/GND */
    ADC_configureMemory(ADC_BASE,
                        ADC_INPUT_A0,
                        ADC_VREFPOS_AVCC,
                        ADC_VREFNEG_AVSS);

    volatile uint16_t raw_adc = 0;      // volatile: visible en debugger sin optimizar
    volatile uint32_t milivoltios = 0;

    while (1)
    {
        /* Esperar presion del boton S1 */
        while (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2) == GPIO_INPUT_PIN_HIGH);

        /* Paso 5: iniciar conversion y esperar por polling */
        ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);

        while (ADC_isBusy(ADC_BASE) == ADC_BUSY);

        /* Leer resultado (0 a 1023 para ADC de 10 bits) */
        raw_adc = ADC_getResults(ADC_BASE);

        /* Convertir a milivoltios: raw * 3300 / 1023 */
        milivoltios = (uint32_t)raw_adc * 3300UL / 1023UL;

        /* Poner breakpoint aqui y ver raw_adc y milivoltios en Expressions */
        __no_operation();
    }

    return 0;
}
