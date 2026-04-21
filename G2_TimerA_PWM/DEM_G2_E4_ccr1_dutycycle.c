/*
 * DEM_G2_E4_ccr1_dutycycle.c
 * Gabinete 2 - Ejercicio 4: CCR1 y CCR2 - duty cycle por comparacion (DEMOSTRATIVO)
 *
 * Timer_A0 en Modo Up, CCR0 = 999 (periodo 1 ms).
 * CCR1 = 499: modo TOGGLE -> duty cycle 50% en LED1 (toggle cada 500 us).
 *
 * Dos ISRs:
 *   TIMER0_A0_VECTOR (CCR0 exclusivo) -> no se usa aqui (deshabilitado)
 *   TIMER0_A1_VECTOR (CCR1/CCR2/OVF) -> maneja CCR1
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

    /* Modo Up: periodo 1 ms */
    Timer_A_initUpModeParam upParam = {0};
    upParam.clockSource        = TIMER_A_CLOCKSOURCE_SMCLK;
    upParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    upParam.timerPeriod        = 999;
    upParam.timerInterruptEnable_TAIE =
                                 TIMER_A_TAIE_INTERRUPT_DISABLE;
    upParam.captureCompareInterruptEnable_CCR0_CCIE =
                                 TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;
    upParam.timerClear         = TIMER_A_DO_CLEAR;
    upParam.startTimer         = true;
    Timer_A_initUpMode(TIMER_A0_BASE, &upParam);

    /* CCR1: modo comparacion TOGGLE, ISR a los 499 cuentas */
    Timer_A_initCompareModeParam cmpParam = {0};
    cmpParam.compareRegister        = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    cmpParam.compareInterruptEnable =
                                 TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    cmpParam.compareOutputMode      = TIMER_A_OUTPUTMODE_TOGGLE;
    cmpParam.compareValue           = 499;
    Timer_A_initCompareMode(TIMER_A0_BASE, &cmpParam);

    __bis_SR_register(LPM0_bits | GIE);

    while (1);
    return 0;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void ISR_TA0_CCR1_OVF(void)
{
    switch (__even_in_range(TA0IV, 14))
    {
        case 0x02:   // CCR1 - duty cycle 50%
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
            break;
        default: break;
    }
}
