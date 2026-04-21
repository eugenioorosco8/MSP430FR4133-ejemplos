/*
 * GL_U4_06_lcd_simbolos.c
 * Unidad 4 - LCD: simbolos especiales con LCDMEMW
 *
 * El LCD del FR4133 tiene iconos/simbolos que NO son caracteres alfanumericos.
 * Se activan escribiendo directamente en los registros LCDMEMW[]:
 *
 *   Simbolo   | Mascara   | Registro
 *   ----------|-----------|----------
 *   TMR       | LCD_TMR   | LCDMEM[12]  (bit 3 - verificado codigo TI)
 *   HRT       | LCD_HRT   | LCDMEM[12]  (bit 2)
 *   REC       | LCD_REC   | LCDMEM[12]  (bit 1)
 *   ANT       | LCD_ANT   | LCDMEM[12]  (bit 0)
 *   BAT       | LCD_BAT   | LCDMEM[13]  (bit 2)
 *   ":"       | LCD_COLON | LCDMEM[pos+1] bit 2 (verificado codigo TI)
 *   grado     | LCD_DEGREE| LCDMEM[3]   (verificado codigo TI)
 *   punto dec.| LCD_POINT | LCDMEM[pos+1] bit 0
 *
 * Constantes definidas en hal_LCD.h. LCD_TMR verificado en StopWatchMode.c
 * del proyecto out-of-box de TI. El resto ocupa los bits libres del mismo
 * banco de iconos (LCDMEM[12]/LCDMEM[13]).
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

    /* Simbolos especiales: todos en LCDMEM[12] y LCDMEM[13] */
    LCDMEM[12] |= LCD_TMR;   // cronometro  (bit 3, verificado TI)
    LCDMEM[12] |= LCD_HRT;   // corazon     (bit 2)
    LCDMEM[12] |= LCD_REC;   // grabacion   (bit 1)
    LCDMEM[12] |= LCD_ANT;   // antena      (bit 0)
    LCDMEM[13] |= LCD_BAT;   // bateria     (bit 2 de byte siguiente)

    /* Dos puntos ":" entre digitos (verificado codigo TI) */
    LCDMEM[pos2 + 1] |= LCD_COLON;   // ":" entre pos2 y pos3
    LCDMEM[pos4 + 1] |= LCD_COLON;   // ":" entre pos4 y pos5

    /* Grado "grados" junto a pos5 (verificado codigo TI) */
    LCDMEM[pos5 + 1] |= LCD_DEGREE;

    /* Mostrar digitos "12:34" */
    showChar('1', pos1);
    showChar('2', pos2);
    showChar('3', pos3);
    showChar('4', pos4);

    /* Punto decimal junto a un digito */
    LCDMEM[pos3 + 1] |= LCD_POINT;   // punto despues de pos3

    /* Para apagar un simbolo: AND con complemento
     * Ejemplo: LCDMEM[12] &= ~LCD_TMR; */

    while (1);
    return 0;
}
