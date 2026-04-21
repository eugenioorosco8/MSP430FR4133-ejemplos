/*
 * DEM_G4_P2_spi_maestro.c
 * Gabinete 4 - Parte 2: SPI maestro con eUSCI_B0 (DEMOSTRATIVO)
 *
 * Transmite el byte 0xA5 al esclavo SPI en forma continua y lee la respuesta.
 * Usar junto con DEM_G4_P2_spi_esclavo.c en un segundo kit FR4133.
 *
 * Conexion entre los dos kits (cable directo):
 *   Maestro P5.0 (MOSI) -> Esclavo P5.0 (MOSI)
 *   Maestro P5.1 (MISO) -> Esclavo P5.1 (MISO)
 *   Maestro P5.2 (SCK)  -> Esclavo P5.2 (SCK)
 *   Maestro P5.3 (CS)   -> Esclavo P5.3 (CS)
 *   GND -> GND (obligatorio)
 *
 * Configuracion: CPOL=0, CPHA=0, MSB first, 3 pines + CS manual
 * SCK = SMCLK/2 = 500 kHz con SMCLK = 1 MHz
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* Pines MOSI, MISO, SCK */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P5,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    /* CS (P5.3): GPIO salida, activo bajo */
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN3);

    EUSCI_B_SPI_initMasterParam sp = {0};
    sp.selectClockSource      = EUSCI_B_SPI_CLOCKSOURCE_SMCLK;
    sp.clockSourceFrequency   = 1000000UL;
    sp.desiredSpiClock        = 500000UL;
    sp.msbFirst               = EUSCI_B_SPI_MSB_FIRST;
    sp.clockPhase             = EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
    sp.clockPolarity          = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
    sp.spiMode                = EUSCI_B_SPI_3PIN;

    EUSCI_B_SPI_initMaster(EUSCI_B0_BASE, &sp);
    EUSCI_B_SPI_enable(EUSCI_B0_BASE);

    uint8_t rxData = 0;

    while (1)
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN3);   // CS activo

        EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, 0xA5);
        while (EUSCI_B_SPI_isBusy(EUSCI_B0_BASE));
        rxData = EUSCI_B_SPI_receiveData(EUSCI_B0_BASE);

        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN3);  // CS inactivo

        /* Poner breakpoint aqui y verificar rxData == 0xA5 (eco del esclavo) */
        __no_operation();

        __delay_cycles(100000);
    }

    return 0;
}
