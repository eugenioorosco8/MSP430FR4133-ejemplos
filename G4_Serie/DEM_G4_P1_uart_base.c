/*
 * DEM_G4_P1_uart_base.c
 * Gabinete 4 - Parte 1: UART asincronico - codigo base (DEMOSTRATIVO)
 *
 * eUSCI_A0 configurado como UART 9600 bps, 8N1.
 * Cada caracter recibido se retransmite (eco).
 * El CPU queda en LPM0 entre caracteres.
 *
 * Conexion (FR4133 LaunchPad):
 *   TX: P1.0 (UCA0TXD) - hacia el PC via USB-UART
 *   RX: P1.1 (UCA0RXD) - desde el PC
 *   (El programador integrado hace el puente UART <-> USB)
 *
 * Calculo baudrate 9600 bps con SMCLK = 1 MHz (modo sobremuestreo x16):
 *   UCBRx  = 6
 *   UCBRFx = 8
 *   UCBRSx = 0x20
 *
 * Terminal: 9600 bps, 8 bits, sin paridad, 1 stop bit
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* Pines TX (P1.0) y RX (P1.1) como funcion UART */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN0 | GPIO_PIN1,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    /* Parametros UART 9600 bps */
    EUSCI_A_UART_initParam p = {0};
    p.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    p.clockPrescalar    = 6;
    p.firstModReg       = 8;
    p.secondModReg      = 0x20;
    p.parity            = EUSCI_A_UART_NO_PARITY;
    p.msborLsbFirst     = EUSCI_A_UART_LSB_FIRST;
    p.numberofStopBits  = EUSCI_A_UART_ONE_STOP_BIT;
    p.uartMode          = EUSCI_A_UART_MODE;
    p.overSampling      = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    EUSCI_A_UART_init(EUSCI_A0_BASE, &p);
    EUSCI_A_UART_enable(EUSCI_A0_BASE);

    EUSCI_A_UART_clearInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    EUSCI_A_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);

    __bis_SR_register(LPM0_bits | GIE);

    while (1);
    return 0;
}

#pragma vector = USCI_A0_VECTOR
__interrupt void ISR_UART_A0(void)
{
    switch (__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_UART_UCRXIFG:
        {
            uint8_t rxData = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
            EUSCI_A_UART_transmitData(EUSCI_A0_BASE, rxData);   // eco
            break;
        }
        default: break;
    }
}
