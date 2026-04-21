/*
 * DEM_G3_E4_lcd_hal.c
 * Gabinete 3 - Ejercicio 4: HAL hal_LCD.h - showChar, clearLCD y scroll (DEMOSTRATIVO)
 *
 * Uso de las funciones de la HAL del LCD del FR4133:
 *   Init_LCD()           -> inicializa el controlador LCD
 *   clearLCD()           -> borra todos los segmentos
 *   showChar(c, posN)    -> muestra caracter ASCII en posicion 1-6
 *   displayScrollText()  -> hace scroll de un string
 *
 * Requiere: hal_LCD.h y hal_LCD.c (incluidos en el emptyProject)
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>
#include "hal_LCD.h"

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    Init_LCD();
    clearLCD();

    /* Mostrar "HOLA" */
    showChar('H', pos1);
    showChar('O', pos2);
    showChar('L', pos3);
    showChar('A', pos4);

    __delay_cycles(2000000);
    clearLCD();

    /* Mostrar numero 1234 descompuesto en digitos */
    uint16_t valor = 1234;
    showChar('0' +  valor / 1000,        pos1);
    showChar('0' + (valor / 100)  % 10,  pos2);
    showChar('0' + (valor / 10)   % 10,  pos3);
    showChar('0' +  valor         % 10,  pos4);

    /* Punto decimal despues del segundo digito */
    LCDMEMW[pos2 / 2] |= 0x0100;

    __delay_cycles(2000000);
    clearLCD();

    /* Scroll de texto */
    displayScrollText("MSP430");

    while (1);
    return 0;
}
