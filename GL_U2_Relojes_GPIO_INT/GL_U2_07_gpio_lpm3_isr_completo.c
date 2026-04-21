/*
 * GL_U2_07_gpio_lpm3_isr_completo.c
 * Unidad 2 - Patron completo: GPIO + LPM3 + Interrupcion
 *
 * Patron de bajo consumo mas eficiente:
 *   - CPU dormido en LPM3 (consumo ~1-2 uA con ACLK activo)
 *   - ISR del boton S1 despierta el CPU y toglea LED1
 *   - El CPU vuelve a dormir automaticamente al salir de la ISR
 *     (porque __bic_SR_register_on_exit NO se llama -> LPM3 se mantiene)
 *
 * Secuencia de configuracion (7 pasos):
 *   1. Detener watchdog
 *   2. Desbloquear GPIO
 *   3. Configurar LED como salida
 *   4. Configurar boton con pull-up
 *   5. Limpiar flag de interrupcion
 *   6. Seleccionar flanco H->L y habilitar INT
 *   7. Entrar en LPM3 con GIE habilitado
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    /* Paso 1 */
    WDT_A_hold(WDT_A_BASE);

    /* Paso 2 */
    PMM_unlockLPM5();

    /* Paso 3: LED1 (P1.0) como salida, apagado */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Paso 4: S1 (P1.2) como entrada con pull-up */
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);

    /* Paso 5: limpiar flag previo */
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);

    /* Paso 6: flanco H->L y habilitar interrupcion */
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN2,
                             GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN2);

    /* Paso 7: entrar en LPM3 con interrupciones habilitadas */
    __bis_SR_register(LPM3_bits | GIE);

    /* El CPU no llega aqui mientras este en LPM3 */
    while (1);

    return 0;
}

/* ISR PORT1 - se ejecuta en cada flanco H->L del boton S1 */
#pragma vector = PORT1_VECTOR
__interrupt void ISR_Puerto1(void)
{
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* No llamamos __bic_SR_register_on_exit -> el CPU vuelve a LPM3
     * al terminar la ISR. Consumo tipico: ~1.5 uA */
}
