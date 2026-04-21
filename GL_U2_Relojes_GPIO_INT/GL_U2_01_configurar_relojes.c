/*
 * GL_U2_01_configurar_relojes.c
 * Unidad 2 - Sistema de Relojes (CS)
 *
 * Configura los tres relojes del MSP430FR4133:
 *   MCLK  = DCO = 8 MHz  (CPU y perifericos rapidos)
 *   SMCLK = DCO/8 = 1 MHz
 *   ACLK  = REFO = 32768 Hz (timers de bajo consumo)
 *
 * Verificacion: usar Debug > Expressions con
 *   CS_getMCLK(), CS_getSMCLK(), CS_getACLK()
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

void configurarRelojes(void);

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    configurarRelojes();

    /* Con relojes configurados, un delay de 8000000 ciclos = 1 segundo */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    while (1)
    {
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        __delay_cycles(8000000);   // 1 s con MCLK = 8 MHz
    }

    return 0;
}

void configurarRelojes(void)
{
    /* FLL: DCO a 8 MHz usando REFO (32768 Hz) como referencia
     * ratio = 8,000,000 / 32,768 = 244
     * CS_initFLLSettle espera hasta que el FLL se estabilice */
    CS_initFLLSettle(8000, 244);

    /* MCLK = DCOCLKDIV = 8 MHz */
    CS_initClockSignal(CS_MCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_1);

    /* SMCLK = DCOCLKDIV / 8 = 1 MHz */
    CS_initClockSignal(CS_SMCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_8);

    /* ACLK = REFO = 32768 Hz */
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
}
