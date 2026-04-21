/*
 * DEM_G2_E1_modo_up.c
 * Gabinete 2 - Ejercicio 1: Timer_A Modo Up con ISR (DEMOSTRATIVO)
 *
 * Timer_A0 en Modo Up, SMCLK = 1 MHz, CCR0 = 999:
 *   Periodo ISR = (CCR0+1) / SMCLK = 1 ms
 *   LED1 toglea cada 1 ms (500 Hz visual).
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

    Timer_A_initUpModeParam param = {0};
    param.clockSource            = TIMER_A_CLOCKSOURCE_SMCLK;
    param.clockSourceDivider     = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod            = 999;
    param.timerInterruptEnable_TAIE =
                                   TIMER_A_TAIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE =
                                   TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    param.timerClear             = TIMER_A_DO_CLEAR;
    param.startTimer             = true;

    Timer_A_initUpMode(TIMER_A0_BASE, &param);

    __bis_SR_register(LPM0_bits | GIE);

    while (1);
    return 0;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void ISR_TA0_CCR0(void)
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
