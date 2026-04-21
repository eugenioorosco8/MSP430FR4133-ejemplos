/*
 * GL_U5_01_uart_eco.c
 * Unidad 5 - UART asincronico: eco por ISR
 *
 * Configura eUSCI_A0 como UART a 9600 bps, 8N1.
 * Cada caracter recibido se retransmite inmediatamente (eco).
 * La ISR RX despierta al CPU brevemente y vuelve a LPM0.
 *
 * Conexion (FR4133 LaunchPad):
 *   TX: P1.0 (UART TX hacia PC via USB-UART del programador)
 *   RX: P1.1 (UART RX desde PC)
 *
 * Calculo de baudrate para 9600 bps con SMCLK = 1 MHz (modo de sobremuestreo):
 *   UCBRx  = 6
 *   UCBRFx = 8
 *   UCBRSx = 0x20
 *
 * Terminal: 9600 bps, 8 bits, sin paridad, 1 stop bit, sin control de flujo
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* Configurar pines TX y RX de UART (funcion primaria de modulo) */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN0 | GPIO_PIN1,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    /* Parametros UART: 9600 bps, 8N1, SMCLK = 1 MHz */
    EUSCI_A_UART_initParam uartParam = {0};
    uartParam.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    uartParam.clockPrescalar    = 6;       // UCBRx
    uartParam.firstModReg       = 8;       // UCBRFx
    uartParam.secondModReg      = 0x20;    // UCBRSx
    uartParam.parity            = EUSCI_A_UART_NO_PARITY;
    uartParam.msborLsbFirst     = EUSCI_A_UART_LSB_FIRST;
    uartParam.numberofStopBits  = EUSCI_A_UART_ONE_STOP_BIT;
    uartParam.uartMode          = EUSCI_A_UART_MODE;
    uartParam.overSampling      = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    EUSCI_A_UART_init(EUSCI_A0_BASE, &uartParam);
    EUSCI_A_UART_enable(EUSCI_A0_BASE);

    /* Habilitar interrupcion de recepcion RX */
    EUSCI_A_UART_clearInterrupt(EUSCI_A0_BASE,
                                EUSCI_A_UART_RECEIVE_INTERRUPT);
    EUSCI_A_UART_enableInterrupt(EUSCI_A0_BASE,
                                 EUSCI_A_UART_RECEIVE_INTERRUPT);

    __bis_SR_register(LPM0_bits | GIE);

    while (1);
    return 0;
}

/* ISR UART - se ejecuta al recibir un caracter */
#pragma vector = USCI_A0_VECTOR
__interrupt void ISR_UART_A0(void)
{
    switch (__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_UART_UCRXIFG:
        {
            uint8_t rxData = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
            EUSCI_A_UART_transmitData(EUSCI_A0_BASE, rxData);  // eco
            break;
        }
        default:
            break;
    }
}
