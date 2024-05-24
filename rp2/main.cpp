#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include "registers.h"
#include "chassis.cpp"
#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include "i2cCommandReceiver.cpp"

#include <hardware/uart.h>
#include "serialComamndReceiver.cpp"

#define I2C_SLAVE_PORT i2c0
#define I2C_MASTER_PORT i2c1
#define LED_STATUS_INDICATOR 25

#define UART_INSTANCE uart0
#define UART_BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// I2C
static const uint I2C_SLAVE_SDA_PIN = 16; // Green
static const uint I2C_SLAVE_SCL_PIN = 17; // Yellow
static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_SLAVE_BAUDRATE = 100000; // 100 kHz

Chassis chassis(500);
I2cCommandReceiver i2cCommandReceiver(I2C_SLAVE_PORT, &chassis);
SerialComamndReceiver serialComamndReceiver(UART_INSTANCE, &chassis);

int movementIntervalStartTime = to_ms_since_boot(get_absolute_time());
int modifiedRegisterCount;

void status_indicator_init()
{
    gpio_init(LED_STATUS_INDICATOR);
    gpio_set_dir(LED_STATUS_INDICATOR, GPIO_OUT);
}

void indicate_status(int ms_since_boot)
{
    //TODO: this is a terrible way of doing it. Make it better later, use an interrupt.
    //TODO: use this LED indicator to provide error codes like motherboard beep codes.
    auto ledOn = (ms_since_boot % 2000) < 1000;
    gpio_put(LED_STATUS_INDICATOR, ledOn);
}

void action_movement(int ms_since_boot)
{
    int elapsed_time = ms_since_boot - movementIntervalStartTime; 
    if(elapsed_time < 10)
    {
        return;
    }

    movementIntervalStartTime = ms_since_boot;

    auto modifiedRegisters = i2cCommandReceiver.GetModifiedRegisters(&modifiedRegisterCount);
    if(0 == modifiedRegisterCount)
    {
        return;
    }

    printf("%d registers modified.\n", modifiedRegisterCount);
    for(int i = 0; i < modifiedRegisterCount; i++)
    {
        uint8_t registerAddress = modifiedRegisters[i];
        uint8_t registerValue = i2cCommandReceiver.GetRegisterValue(registerAddress);

        printf("register %x value changed to %x.\n", registerAddress, registerValue);
    }
}

int main()
{
    stdio_init_all();
    status_indicator_init();
    
    i2cCommandReceiver.setup_command_receiver(I2C_SLAVE_SDA_PIN, I2C_SLAVE_SCL_PIN, I2C_SLAVE_BAUDRATE, I2C_SLAVE_ADDRESS, 500);
    serialComamndReceiver.setup_command_receiver(UART_BAUD_RATE, UART_TX_PIN, UART_RX_PIN, 500);

    while(true)
    {
        auto ms_since_boot = to_ms_since_boot(get_absolute_time());
        indicate_status(ms_since_boot);
        action_movement(ms_since_boot);
    }
}
