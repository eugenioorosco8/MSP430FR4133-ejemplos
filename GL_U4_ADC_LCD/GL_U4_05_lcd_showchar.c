/*
 * GL_U4_05_lcd_showchar.c
 * Unidad 4 - LCD de segmentos: HAL hal_LCD.h
 *
 * Uso de la HAL del LCD de 6 digitos del FR4133:
 *   Init_LCD()       -> inicializa el controlador LCD
 *   clearLCD()       -> borra todos los segmentos
 *   showChar(c, pos) -> muestra caracter ASCII en posicion 1-6
 *   displayScrollText() -> hace scroll de un string
 *
 * Posiciones del LCD: pos1 (izquierda) ... pos6 (derecha)
 * Las constantes pos1..pos6 estan definidas en hal_LCD.h
 *
 * Requiere: hal_LCD.h y hal_LCD.c en el proyecto
 *   (incluidos en el emptyProject)
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

    /* Inicializar LCD */
    Init_LCD();
    clearLCD();

    /* Mostrar "HOLA" en las primeras 4 posiciones */
    showChar('H', pos1);
    showChar('O', pos2);
    showChar('L', pos3);
    showChar('A', pos4);

    /* Mostrar un numero de 4 digitos: 1234 */
    uint16_t valor = 1234;
    showChar('0' + valor / 1000,       pos1);   // millar
    showChar('0' + (valor / 100) % 10, pos2);   // centena
    showChar('0' + (valor / 10)  % 10, pos3);   // decena
    showChar('0' + valor % 10,         pos4);   // unidad

    /* Punto decimal despues del cuarto digito (pos4):
     * Los segmentos especiales se escriben directamente en LCDMEMW */
    LCDMEMW[pos4 / 2] |= 0x0100;   // bit del punto decimal

    /* Scroll de texto largo */
    __delay_cycles(2000000);
    clearLCD();
    displayScrollText("MSP430");

    while (1);
    return 0;
}
