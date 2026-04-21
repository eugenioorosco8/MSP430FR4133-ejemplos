/*
 * DEM_G4_P3_i2c_esclavo.c
 * Gabinete 4 - Parte 3: I2C esclavo con eUSCI_B0 (DEMOSTRATIVO)
 *
 * Esclavo I2C con direccion 0x48.
 * Cuando el maestro escribe: guarda el byte en rxByte.
 * Cuando el maestro lee: responde con txByte = 0xAB.
 *
 * Usar junto con DEM_G4_P3_i2c_maestro.c en el kit maestro.
 * Conexion: ver DEM_G4_P3_i2c_maestro.c
 *
 * Target: MSP430FR4133
 * Proyecto base: emptyProject con driverlib
 */

#include <driverlib.h>

#define MY_ADDR  0x48

volatile uint8_t rxByte = 0;
volatile uint8_t txByte = 0xAB;   // dato que retorna al maestro

int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    /* SDA (P5.0) y SCL (P5.1) */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P5,
        GPIO_PIN0 | GPIO_PIN1,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    EUSCI_B_I2C_initSlaveParam sp = {0};
    sp.slaveAddress      = MY_ADDR;
    sp.slaveAddressOffset = EUSCI_B_I2C_OWN_ADDRESS_OFFSET0;
    sp.slaveOwnAddressEnable = EUSCI_B_I2C_OWN_ADDRESS_ENABLE;

    EUSCI_B_I2C_initSlave(EUSCI_B0_BASE, &sp);
    EUSCI_B_I2C_enable(EUSCI_B0_BASE);

    EUSCI_B_I2C_clearInterrupt(EUSCI_B0_BASE,
        EUSCI_B_I2C_RECEIVE_INTERRUPT0 | EUSCI_B_I2C_TRANSMIT_INTERRUPT0);
    EUSCI_B_I2C_enableInterrupt(EUSCI_B0_BASE,
        EUSCI_B_I2C_RECEIVE_INTERRUPT0 | EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    __bis_SR_register(LPM0_bits | GIE);

    while (1);
    return 0;
}

#pragma vector = USCI_B0_VECTOR
__interrupt void ISR_I2C_esclavo(void)
{
    switch (__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
    {
        case USCI_I2C_UCRXIFG0:
            rxByte = EUSCI_B_I2C_slaveGetData(EUSCI_B0_BASE);
            break;

        case USCI_I2C_UCTXIFG0:
            EUSCI_B_I2C_slavePutData(EUSCI_B0_BASE, txByte);
            break;

        default: break;
    }
}
