/*
 * DEM_G2_E2_modo_continuo.c
 * Gabinete 2 - Ejercicio 2: Timer_A Modo Continuo con overflow (DEMOSTRATIVO)
 *
 * Timer_A0 en Modo Continuo, SMCLK = 1 MHz:
 *   Cuenta 0 -> 65535 -> 0 (overflow cada 65536 ciclos = ~65.5 ms)
 *   La ISR de overflow (TAIV = 0x0A) toglea LED1.
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

    Timer_A_initContinuousModeParam param = {0};
    param.clockSource        = TIMER_A_CLOCKSOURCE_SMCLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
    param.timerClear         = TIMER_A_DO_CLEAR;
    param.startTimer         = true;

    Timer_A_initContinuousMode(TIMER_A0_BASE, &param);

    __bis_SR_register(LPM0_bits | GIE);

    while (1);
    return 0;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void ISR_TA0_CCR1_OVF(void)
{
    switch (__even_in_range(TA0IV, 14))
    {
        case 0x00: break;
        case 0x02: break;   // CCR1
        case 0x04: break;   // CCR2
        case 0x0A:           // overflow TAIFG (~65.5 ms)
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
            break;
        default:   break;
    }
}
