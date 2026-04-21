/*
 * DEM_G1_E4b_blinky_cs.c
 * Gabinete 1 - Ejercicio 4.2: Blinky con CS configurado (DEMOSTRATIVO)
 *
 * Demuestra el efecto de la frecuencia de MCLK en el retardo por software.
 * Con MCLK = 8 MHz, el mismo __delay_cycles(500000) produce ~62.5 ms
 * en lugar de ~500 ms (porque hay 8x mas ciclos por segundo).
 *
 * El alumno puede cambiar la frecuencia del DCO y observar el cambio
 * en la velocidad de parpadeo del LED.
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* Configurar DCO a 8 MHz via FLL (ratio = 8,000,000 / 32,768 = 244) */
    CS_initFLLSettle(8000, 244);
    CS_initClockSignal(CS_MCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_1);

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    while (1)
    {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        __delay_cycles(500000);   // con 8 MHz -> ~62.5 ms (no ~500 ms)
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        __delay_cycles(500000);
    }

    return 0;
}
