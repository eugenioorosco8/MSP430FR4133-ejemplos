/*
 * GL_U4_07_integracion_timer_adc_lcd.c
 * Unidad 4 - Patron integrador: Timer_A + ADC (ISR) + HAL LCD + LPM3
 *
 * Sistema completo de medicion y visualizacion:
 *   1. Timer_A0 en Modo Up con ACLK (1 Hz, CCR0 = 32767)
 *   2. ISR del Timer dispara una conversion ADC por software
 *   3. ISR del ADC lee el resultado, calcula mV y actualiza el LCD
 *   4. CPU en LPM3 entre eventos (consumo minimo)
 *
 * Flujo de eventos:
 *   LPM3 -> [Timer ISR] -> inicia ADC -> LPM0 -> [ADC ISR] -> muestra en LCD -> LPM3
 *
 * Requiere: hal_LCD.h y hal_LCD.c en el proyecto
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>
#include "hal_LCD.h"

volatile uint8_t nueva_medicion = 0;
volatile uint16_t resultado_raw = 0;

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* LCD */
    Init_LCD();
    clearLCD();

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

    /* Timer_A0: Modo Up, ACLK, periodo = 1 s */
    Timer_A_initUpModeParam timerParam = {0};
    timerParam.clockSource        = TIMER_A_CLOCKSOURCE_ACLK;
    timerParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    timerParam.timerPeriod        = 32767;
    timerParam.timerInterruptEnable_TAIE =
                                  TIMER_A_TAIE_INTERRUPT_DISABLE;
    timerParam.captureCompareInterruptEnable_CCR0_CCIE =
                                  TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    timerParam.timerClear         = TIMER_A_DO_CLEAR;
    timerParam.startTimer         = true;
    Timer_A_initUpMode(TIMER_A0_BASE, &timerParam);

    /* LPM3: solo ACLK y timer activos */
    __bis_SR_register(LPM3_bits | GIE);

    while (1);
    return 0;
}

/* ISR Timer_A0 CCR0 - cada 1 segundo: lanza conversion ADC */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void ISR_TA0_CCR0(void)
{
    ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);
    /* Salir de LPM3 a LPM0 para que el ADC pueda correr */
    __bic_SR_register_on_exit(LPM3_bits);
}

/* ISR ADC - cuando la conversion termina: actualizar LCD */
#pragma vector = ADC_VECTOR
__interrupt void ISR_ADC(void)
{
    ADC_clearInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT);

    resultado_raw = ADC_getResults(ADC_BASE);
    uint32_t mv = (uint32_t)resultado_raw * 3300UL / 1023UL;

    /* Mostrar en LCD: 4 digitos en mV (0000 a 3300) */
    clearLCD();
    showChar('0' + (mv / 1000),       pos1);
    showChar('0' + (mv / 100) % 10,   pos2);
    showChar('0' + (mv / 10)  % 10,   pos3);
    showChar('0' + mv % 10,           pos4);
    showChar('m', pos5);
    showChar('V', pos6);

    /* Volver a LPM3 */
    __bis_SR_register_on_exit(LPM3_bits);
}
