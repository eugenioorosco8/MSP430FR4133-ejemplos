/*
 * GL_U3_04_ccr1_dutycycle.c
 * Unidad 3 - CCR1 en modo comparacion: duty cycle 50%
 *
 * Timer_A0 en Modo Up con CCR0 = 999 (periodo 1 ms a 1 MHz).
 * CCR1 = 499: modo TOGGLE -> la salida cambia cada vez que TA0R == CCR1.
 * Resultado: onda cuadrada al 50% con periodo 2 ms en LED1 por software.
 *
 * Las dos ISRs usan distintos vectores:
 *   TIMER0_A0_VECTOR -> exclusivo para CCR0
 *   TIMER0_A1_VECTOR -> compartido entre CCR1, CCR2 y overflow (usa TAIV)
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

    /* Timer_A0 en Modo Up, periodo = 1 ms */
    Timer_A_initUpModeParam upParam = {0};
    upParam.clockSource        = TIMER_A_CLOCKSOURCE_SMCLK;
    upParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    upParam.timerPeriod        = 999;
    upParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    upParam.captureCompareInterruptEnable_CCR0_CCIE =
                               TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;
    upParam.timerClear         = TIMER_A_DO_CLEAR;
    upParam.startTimer         = true;
    Timer_A_initUpMode(TIMER_A0_BASE, &upParam);

    /* CCR1 en modo comparacion, TOGGLE a los 499 cuentas */
    Timer_A_initCompareModeParam cmpParam = {0};
    cmpParam.compareRegister   = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    cmpParam.compareInterruptEnable =
                               TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    cmpParam.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE;
    cmpParam.compareValue      = 499;
    Timer_A_initCompareMode(TIMER_A0_BASE, &cmpParam);

    __bis_SR_register(LPM0_bits | GIE);

    while (1);
    return 0;
}

/* ISR CCR1 - se ejecuta cuando TA0R == CCR1 (cada 500 us) */
#pragma vector = TIMER0_A1_VECTOR
__interrupt void ISR_TA0_CCR1_OVF(void)
{
    switch (__even_in_range(TA0IV, 14))
    {
        case 0x02:   // CCR1
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
            break;
        case 0x0A:   // overflow
            break;
        default:
            break;
    }
}
