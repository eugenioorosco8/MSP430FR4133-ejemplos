/*
 * GL_U3_03_modo_updown.c
 * Unidad 3 - Timer_A en Modo Up-Down
 *
 * Timer_A0 en Modo Up-Down con CCR0 = 500 y SMCLK = 1 MHz:
 *   El timer cuenta 0->500->0->500->... (simetrico)
 *   Periodo real = 2 * CCR0 / SMCLK = 2 * 500 / 1e6 = 1 ms
 *   NOTA: la ISR del CCR0 se dispara SOLO en el pico (cuando llega a CCR0),
 *   NO en el valle (cero). El periodo de la ISR es 1 ms.
 *
 * Diferencia con Modo Up: en Up-Down el periodo es 2*CCR0 (no CCR0+1).
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

    /* Configurar Timer_A0 en Modo Up-Down */
    Timer_A_initUpDownModeParam param = {0};
    param.clockSource        = TIMER_A_CLOCKSOURCE_SMCLK;     // 1 MHz
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod        = 500;                            // CCR0
    param.timerInterruptEnable_TAIE  = TIMER_A_TAIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE =
                               TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    param.timerClear         = TIMER_A_DO_CLEAR;
    param.startTimer         = true;

    Timer_A_initUpDownMode(TIMER_A0_BASE, &param);

    __bis_SR_register(LPM0_bits | GIE);

    while (1);
    return 0;
}

/* ISR CCR0 - se ejecuta solo en el pico (TA0R == CCR0), periodo = 1 ms */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void ISR_TA0_CCR0(void)
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
