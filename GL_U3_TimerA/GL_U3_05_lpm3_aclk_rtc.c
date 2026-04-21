/*
 * GL_U3_05_lpm3_aclk_rtc.c
 * Unidad 3 - RTC por software: Timer_A + ACLK + LPM3
 *
 * Timer_A0 en Modo Up clocked por ACLK (REFO = 32768 Hz):
 *   CCR0 = 32767 -> periodo = 32768 / 32768 = 1 segundo exacto
 *   La ISR toglea LED1 cada 1 segundo.
 *   El CPU queda en LPM3 (ACLK activo, MCLK/SMCLK apagados).
 *
 * Consumo tipico:
 *   LPM0 con SMCLK 1 MHz: ~250 uA
 *   LPM3 con ACLK 32768 Hz: ~1.5 uA (factor de mejora x165)
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* ACLK = REFO = 32768 Hz (configuracion por defecto del FR4133) */
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Timer_A0 en Modo Up clocked por ACLK, periodo = 1 s */
    Timer_A_initUpModeParam param = {0};
    param.clockSource        = TIMER_A_CLOCKSOURCE_ACLK;       // 32768 Hz
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod        = 32767;                           // CCR0
    param.timerInterruptEnable_TAIE  = TIMER_A_TAIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE =
                               TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    param.timerClear         = TIMER_A_DO_CLEAR;
    param.startTimer         = true;

    Timer_A_initUpMode(TIMER_A0_BASE, &param);

    /* LPM3: CPU, MCLK y SMCLK apagados. ACLK y timer siguen activos. */
    __bis_SR_register(LPM3_bits | GIE);

    while (1);
    return 0;
}

/* ISR CCR0 - se ejecuta cada 1 segundo */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void ISR_TA0_CCR0(void)
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    /* El CPU vuelve a LPM3 automaticamente al salir de la ISR */
}
