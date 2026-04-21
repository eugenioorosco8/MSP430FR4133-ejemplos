/*
 * DEM_G3_E5_integracion_adc_lcd.c
 * Gabinete 3 - Ejercicio 5: Integracion ADC + LCD (patron completo) (DEMOSTRATIVO)
 *
 * Sistema completo de medicion y visualizacion:
 *   - Timer_A0 con ACLK (1 Hz) dispara conversion ADC
 *   - ISR ADC lee resultado, calcula mV y muestra en LCD
 *   - CPU en LPM3 entre eventos
 *
 * Display: "XXXX mV" donde XXXX va de 0000 a 3300
 *
 * Requiere: hal_LCD.h y hal_LCD.c
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>
#include "hal_LCD.h"

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    Init_LCD();
    clearLCD();
    displayScrollText("INICIO");

    /* Pin A0 como entrada analogica */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1, GPIO_PIN0, GPIO_PRIMARY_MODULE_FUNCTION);

    /* ADC */
    ADC_init(ADC_BASE, ADC_SAMPLEHOLDSOURCE_SC,
             ADC_CLOCKSOURCE_ADCOSC, ADC_CLOCKDIVIDER_1);
    ADC_enable(ADC_BASE);
    ADC_setupSamplingTimer(ADC_BASE, ADC_CYCLEHOLD_16_CYCLES,
                           ADC_MULTIPLESAMPLESDISABLE);
    ADC_configureMemory(ADC_BASE, ADC_INPUT_A0,
                        ADC_VREFPOS_AVCC, ADC_VREFNEG_AVSS);
    ADC_clearInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT);
    ADC_enableInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT);

    /* ACLK = REFO = 32768 Hz */
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Timer_A0: Modo Up, ACLK, 1 Hz */
    Timer_A_initUpModeParam tp = {0};
    tp.clockSource        = TIMER_A_CLOCKSOURCE_ACLK;
    tp.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    tp.timerPeriod        = 32767;
    tp.captureCompareInterruptEnable_CCR0_CCIE =
                            TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    tp.timerClear         = TIMER_A_DO_CLEAR;
    tp.startTimer         = true;
    Timer_A_initUpMode(TIMER_A0_BASE, &tp);

    __bis_SR_register(LPM3_bits | GIE);

    while (1);
    return 0;
}

/* ISR Timer: cada 1 s lanza ADC */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void ISR_TA0(void)
{
    ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);
    __bic_SR_register_on_exit(LPM3_bits);   // pasar a LPM0 para ADC
}

/* ISR ADC: actualizar LCD y volver a LPM3 */
#pragma vector = ADC_VECTOR
__interrupt void ISR_ADC(void)
{
    ADC_clearInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT);

    uint16_t raw = ADC_getResults(ADC_BASE);
    uint32_t mv  = (uint32_t)raw * 3300UL / 1023UL;

    clearLCD();
    showChar('0' +  mv / 1000,       pos1);
    showChar('0' + (mv / 100) % 10,  pos2);
    showChar('0' + (mv / 10)  % 10,  pos3);
    showChar('0' +  mv        % 10,  pos4);
    showChar('m', pos5);
    showChar('V', pos6);

    __bis_SR_register_on_exit(LPM3_bits);   // volver a LPM3
}
