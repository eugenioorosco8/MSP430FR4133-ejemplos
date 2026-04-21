/*
 * GL_U5_02_spi_maestro.c
 * Unidad 5 - SPI maestro con eUSCI_B0
 *
 * Configura eUSCI_B0 como maestro SPI a 500 kHz.
 * Transmite el byte 0xA5 en forma continua y lee la respuesta del esclavo.
 *
 * Conexion (FR4133 LaunchPad):
 *   MOSI: P5.0  (UCB0SIMO)
 *   MISO: P5.1  (UCB0SOMI)
 *   SCK:  P5.2  (UCB0CLK)
 *   CS:   P5.3  (GPIO, control manual)
 *
 * Configuracion SPI:
 *   CPOL = 0, CPHA = 0 (modo 0,0)
 *   MSB primero
 *   3 pines (sin CS automatico)
 *   SCK = SMCLK / 2 = 500 kHz (con SMCLK = 1 MHz)
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* Pines SPI */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P5,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    /* CS como GPIO salida, activo bajo */
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN3);   // CS inactivo

    /* Parametros SPI maestro */
    EUSCI_B_SPI_initMasterParam spiParam = {0};
    spiParam.selectClockSource  = EUSCI_B_SPI_CLOCKSOURCE_SMCLK;  // 1 MHz
    spiParam.clockSourceFrequency = 1000000UL;
    spiParam.desiredSpiClock    = 500000UL;                         // 500 kHz
    spiParam.msbFirst           = EUSCI_B_SPI_MSB_FIRST;
    spiParam.clockPhase         = EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
    spiParam.clockPolarity      = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
    spiParam.spiMode            = EUSCI_B_SPI_3PIN;

    EUSCI_B_SPI_initMaster(EUSCI_B0_BASE, &spiParam);
    EUSCI_B_SPI_enable(EUSCI_B0_BASE);

    uint8_t rxData = 0;

    while (1)
    {
        /* Activar CS (bajo) */
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN3);

        /* Transmitir y esperar a que termine */
        EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, 0xA5);
        while (EUSCI_B_SPI_isBusy(EUSCI_B0_BASE));

        /* Leer dato recibido del esclavo */
        rxData = EUSCI_B_SPI_receiveData(EUSCI_B0_BASE);

        /* Desactivar CS (alto) */
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN3);

        /* Ver rxData en Debug > Expressions */
        __no_operation();

        __delay_cycles(100000);   // pausa entre transmisiones
    }

    return 0;
}
