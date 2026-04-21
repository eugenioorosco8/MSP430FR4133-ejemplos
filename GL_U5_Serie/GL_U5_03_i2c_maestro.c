/*
 * GL_U5_03_i2c_maestro.c
 * Unidad 5 - I2C maestro con eUSCI_B0 (Repeated Start)
 *
 * Configura eUSCI_B0 como maestro I2C a 100 kbps.
 * Protocolo: escritura de un registro + lectura con Repeated Start.
 *   1. START + direccion esclavo (0x48) + WRITE
 *   2. Enviar byte de registro (0x00)
 *   3. REPEATED START + direccion esclavo + READ
 *   4. Leer 1 byte de respuesta + STOP
 *
 * Conexion (FR4133 LaunchPad):
 *   SDA: P5.0  (UCB0SDA)
 *   SCL: P5.1  (UCB0SCL)
 *   Pull-ups de 4.7 kohm a VCC en SDA y SCL (obligatorios)
 *   P1.5 se puede usar como VCC simulada para pull-ups (ver Gabinete 4)
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

#define SLAVE_ADDR   0x48   // direccion del esclavo I2C

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* P1.5 como salida alta = VCC para pull-ups (truco del LaunchPad) */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);

    /* Pines SDA y SCL */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P5,
        GPIO_PIN0 | GPIO_PIN1,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    /* Parametros I2C maestro */
    EUSCI_B_I2C_initMasterParam i2cParam = {0};
    i2cParam.selectClockSource    = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;  // 1 MHz
    i2cParam.i2cClk               = 1000000UL;
    i2cParam.dataRate             = EUSCI_B_I2C_SET_DATA_RATE_100KBPS;
    i2cParam.byteCounterThreshold = 0;
    i2cParam.autoSTOPGeneration   = EUSCI_B_I2C_NO_AUTO_STOP;

    EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &i2cParam);
    EUSCI_B_I2C_enable(EUSCI_B0_BASE);

    uint8_t rxByte = 0;

    while (1)
    {
        /* Fase ESCRITURA: enviar direccion de registro */
        EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE, SLAVE_ADDR);
        EUSCI_B_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
        EUSCI_B_I2C_masterSendStart(EUSCI_B0_BASE);
        EUSCI_B_I2C_masterSendSingleByte(EUSCI_B0_BASE, 0x00);

        /* Fase LECTURA: Repeated Start + leer 1 byte */
        EUSCI_B_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_RECEIVE_MODE);
        rxByte = EUSCI_B_I2C_masterReceiveSingleByte(EUSCI_B0_BASE);

        /* Ver rxByte en Debug > Expressions */
        __no_operation();

        __delay_cycles(500000);
    }

    return 0;
}
