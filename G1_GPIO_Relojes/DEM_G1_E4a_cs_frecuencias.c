/*
 * DEM_G1_E4a_cs_frecuencias.c
 * Gabinete 1 - Ejercicio 4.1: verificar frecuencias por defecto (DEMOSTRATIVO)
 *
 * Lee las frecuencias actuales de MCLK, SMCLK y ACLK mediante funciones
 * de DriverLib y las almacena en variables para visualizarlas en
 * Debug > Expressions (Watch Window).
 *
 * Poner un breakpoint en __no_operation() y agregar las variables
 * mclk_hz, smclk_hz y aclk_hz a la ventana de Expressions.
 *
 * Valores por defecto del FR4133:
 *   MCLK  = DCO = 1 MHz
 *   SMCLK = DCO = 1 MHz
 *   ACLK  = REFO = 32768 Hz
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    uint32_t mclk_hz  = CS_getMCLK();
    uint32_t smclk_hz = CS_getSMCLK();
    uint32_t aclk_hz  = CS_getACLK();

    /* Poner breakpoint aqui y ver mclk_hz, smclk_hz, aclk_hz */
    __no_operation();

    while (1);
    return 0;
}
