/*
 * DEM_G4_P3_i2c_maestro.c
 * Gabinete 4 - Parte 3: I2C maestro con eUSCI_B0 (DEMOSTRATIVO)
 *
 * Maestro I2C a 100 kbps. Protocolo escritura + Repeated Start + lectura:
 *   1. START + dir esclavo (0x48) + WRITE
 *   2. Enviar byte de registro (0x00)
 *   3. REPEATED START + dir esclavo + READ
 *   4. Recibir 1 byte + STOP
 *
 * Usar junto con DEM_G4_P3_i2c_esclavo.c en el segundo kit.
 *
 * Conexion entre kits:
 *   SDA: P5.0 <-> P5.0  (con pull-up 4.7 kohm a VCC)
 *   SCL: P5.1 <-> P5.1  (con pull-up 4.7 kohm a VCC)
 *   GND <-> GND
 *
 * Truco del LaunchPad: usar P1.5 como fuente de VCC para los pull-ups
 * (configurarlo como salida en alto).
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

#define SLAVE_ADDR  0x48

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* P1.5 como VCC simulada para pull-ups */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);

    /* SDA (P5.0) y SCL (P5.1) */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P5,
        GPIO_PIN0 | GPIO_PIN1,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    EUSCI_B_I2C_initMasterParam ip = {0};
    ip.selectClockSource    = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
    ip.i2cClk               = 1000000UL;
    ip.dataRate             = EUSCI_B_I2C_SET_DATA_RATE_100KBPS;
    ip.byteCounterThreshold = 0;
    ip.autoSTOPGeneration   = EUSCI_B_I2C_NO_AUTO_STOP;

    EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &ip);
    EUSCI_B_I2C_enable(EUSCI_B0_BASE);

    uint8_t rxByte = 0;

    while (1)
    {
        /* Fase escritura: enviar registro 0x00 */
        EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE, SLAVE_ADDR);
        EUSCI_B_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
        EUSCI_B_I2C_masterSendStart(EUSCI_B0_BASE);
        EUSCI_B_I2C_masterSendSingleByte(EUSCI_B0_BASE, 0x00);

        /* Fase lectura: Repeated Start + recibir 1 byte */
        EUSCI_B_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_RECEIVE_MODE);
        rxByte = EUSCI_B_I2C_masterReceiveSingleByte(EUSCI_B0_BASE);

        /* Breakpoint aqui: verificar rxByte == 0xAB (respuesta del esclavo) */
        __no_operation();

        __delay_cycles(500000);
    }

    return 0;
}
