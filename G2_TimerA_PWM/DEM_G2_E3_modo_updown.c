/*
 * DEM_G2_E3_modo_updown.c
 * Gabinete 2 - Ejercicio 3: Timer_A Modo Up-Down (DEMOSTRATIVO)
 *
 * Timer_A0 en Modo Up-Down, SMCLK = 1 MHz, CCR0 = 500:
 *   Periodo real = 2 * CCR0 / SMCLK = 2 * 500 / 1e6 = 1 ms
 *   La ISR del CCR0 se dispara solo en el pico (cuando llega a 500).
 *
 * NOTA: el periodo es 2*CCR0 (no CCR0+1 como en Modo Up).
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

    Timer_A_initUpDownModeParam param = {0};
    param.clockSource        = TIMER_A_CLOCKSOURCE_SMCLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod        = 500;
    param.timerInterruptEnable_TAIE =
                               TIMER_A_TAIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE =
                               TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    param.timerClear         = TIMER_A_DO_CLEAR;
    param.startTimer         = true;

    Timer_A_initUpDownMode(TIMER_A0_BASE, &param);

    __bis_SR_register(LPM0_bits | GIE);

    while (1);
    return 0;
}

/* ISR solo en el pico (TA0R == CCR0), periodo = 1 ms */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void ISR_TA0_CCR0(void)
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
