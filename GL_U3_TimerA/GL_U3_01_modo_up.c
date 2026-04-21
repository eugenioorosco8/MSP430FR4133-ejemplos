/*
 * GL_U3_01_modo_up.c
 * Unidad 3 - Timer_A en Modo Up
 *
 * Timer_A0 en Modo Up con CCR0 = 999 y SMCLK = 1 MHz:
 *   Periodo = (CCR0 + 1) / SMCLK = 1000 / 1e6 = 1 ms
 *   La ISR del CCR0 se ejecuta a 1 kHz -> toglea LED1 cada 1 ms.
 *   El CPU queda en LPM0 (SMCLK activo, CPU dormido).
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

    /* Configurar Timer_A0 en Modo Up */
    Timer_A_initUpModeParam param = {0};
    param.clockSource            = TIMER_A_CLOCKSOURCE_SMCLK;        // 1 MHz
    param.clockSourceDivider     = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod            = 999;                               // CCR0
    param.timerInterruptEnable_TAIE  = TIMER_A_TAIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE =
                                   TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    param.timerClear             = TIMER_A_DO_CLEAR;
    param.startTimer             = true;

    Timer_A_initUpMode(TIMER_A0_BASE, &param);

    /* LPM0: CPU duerme, SMCLK y timer siguen activos */
    __bis_SR_register(LPM0_bits | GIE);

    while (1);
    return 0;
}

/* ISR CCR0 del Timer_A0 - se ejecuta cada 1 ms */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void ISR_TA0_CCR0(void)
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
