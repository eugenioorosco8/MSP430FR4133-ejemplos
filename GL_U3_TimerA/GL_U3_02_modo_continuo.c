/*
 * GL_U3_02_modo_continuo.c
 * Unidad 3 - Timer_A en Modo Continuo
 *
 * Timer_A0 en Modo Continuo con SMCLK = 1 MHz:
 *   El timer cuenta de 0 a 0xFFFF (65535) y desborda.
 *   Periodo de overflow = 65536 / 1e6 = ~65.5 ms
 *   La ISR de overflow (TAIV = 0x0A) toglea LED1 cada ~65.5 ms.
 *
 * Se usa __even_in_range() para generar una tabla de salto eficiente
 * con el registro TAIV (lectura de TAIV limpia el flag automaticamente).
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

    /* Configurar Timer_A0 en Modo Continuo */
    Timer_A_initContinuousModeParam param = {0};
    param.clockSource        = TIMER_A_CLOCKSOURCE_SMCLK;     // 1 MHz
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
    param.timerClear         = TIMER_A_DO_CLEAR;
    param.startTimer         = true;

    Timer_A_initContinuousMode(TIMER_A0_BASE, &param);

    __bis_SR_register(LPM0_bits | GIE);

    while (1);
    return 0;
}

/* ISR compartida Timer_A0: CCR1, CCR2 y overflow usan TIMER0_A1_VECTOR */
#pragma vector = TIMER0_A1_VECTOR
__interrupt void ISR_TA0_CCR1_OVF(void)
{
    switch (__even_in_range(TA0IV, 14))
    {
        case 0x00: break;   // sin interrupcion pendiente
        case 0x02: break;   // CCR1
        case 0x04: break;   // CCR2
        case 0x06: break;   // reservado
        case 0x08: break;   // reservado
        case 0x0A:           // overflow TAIFG
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
            break;
        case 0x0C: break;
        case 0x0E: break;
        default:   break;
    }
}
