/*
 * GL_U0_01_driverlib_vs_registro.c
 * Unidad 0 - Introduccion al MSP430FR4133 y DriverLib
 *
 * Comparacion: acceso directo a registros vs uso de DriverLib
 * Ambos bloques hacen exactamente lo mismo: configurar P1.0 como
 * salida y ponerlo en alto (LED encendido).
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* ---- BLOQUE A: acceso directo a registros ---- */
    P1DIR |= BIT0;          // P1.0 como salida
    P1OUT |= BIT0;          // P1.0 en alto (LED encendido)

    /* ---- BLOQUE B: equivalente con DriverLib ---- */
    // GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    // GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Ambos bloques son equivalentes. DriverLib hace mas legible
     * el codigo y reduce errores de mascara de bits. */

    while (1);

    return 0;
}
