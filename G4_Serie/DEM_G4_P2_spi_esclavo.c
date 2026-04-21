/*
 * DEM_G4_P2_spi_esclavo.c
 * Gabinete 4 - Parte 2: SPI esclavo con eUSCI_B0 (DEMOSTRATIVO)
 *
 * Recibe bytes del maestro SPI y los retransmite (eco SPI).
 * Usar junto con DEM_G4_P2_spi_maestro.c en el kit maestro.
 *
 * Conexion: ver DEM_G4_P2_spi_maestro.c
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

volatile uint8_t rxData = 0;

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* Pines MOSI, MISO, SCK, CS */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P5,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    EUSCI_B_SPI_initSlaveParam sp = {0};
    sp.msbFirst      = EUSCI_B_SPI_MSB_FIRST;
    sp.clockPhase    = EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
    sp.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
    sp.spiMode       = EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_LOW;

    EUSCI_B_SPI_initSlave(EUSCI_B0_BASE, &sp);
    EUSCI_B_SPI_enable(EUSCI_B0_BASE);

    EUSCI_B_SPI_clearInterrupt(EUSCI_B0_BASE,
                               EUSCI_B_SPI_RECEIVE_INTERRUPT);
    EUSCI_B_SPI_enableInterrupt(EUSCI_B0_BASE,
                                EUSCI_B_SPI_RECEIVE_INTERRUPT);

    __bis_SR_register(LPM0_bits | GIE);

    while (1);
    return 0;
}

#pragma vector = USCI_B0_VECTOR
__interrupt void ISR_SPI_B0(void)
{
    switch (__even_in_range(UCB0IV, USCI_SPI_UCTXIFG))
    {
        case USCI_SPI_UCRXIFG:
            rxData = EUSCI_B_SPI_receiveData(EUSCI_B0_BASE);
            /* Eco: retransmitir el byte recibido */
            EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, rxData);
            break;
        default: break;
    }
}
