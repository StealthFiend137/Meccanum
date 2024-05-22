#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include "i2cCommandReceiver.cpp"
#include "registers.h"

#define I2C_SLAVE_PORT i2c0
#define I2C_MASTER_PORT i2c1
#define LED_STATUS_INDICATOR 25

// I2C
static const uint I2C_SLAVE_SDA_PIN = 16; // Green
static const uint I2C_SLAVE_SCL_PIN = 17; // Yellow
static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_SLAVE_BAUDRATE = 100000; // 100 kHz

I2cCommandReceiver commandReceiver(I2C_SLAVE_PORT);

int movementIntervalStartTime = to_ms_since_boot(get_absolute_time());
int modifiedRegisterCount;

void status_indicator_init()
{
    gpio_init(LED_STATUS_INDICATOR);
    gpio_set_dir(LED_STATUS_INDICATOR, GPIO_OUT);
}

void indicateActive()
{
    //TODO: this is a terrible way of doing it. Make it better later, use an interrupt.
    //TODO: use this LED indicator to provide error codes like motherboard beep codes.
    auto ledOn = (to_ms_since_boot(get_absolute_time()) % 2000) < 1000;
    gpio_put(LED_STATUS_INDICATOR, ledOn);
}

void actionMovement()
{
    int elapsed_time = to_ms_since_boot(get_absolute_time()) - movementIntervalStartTime; 
    if(elapsed_time < 10)
    {
        return;
    }

    movementIntervalStartTime = to_ms_since_boot(get_absolute_time());

    auto modifiedRegisters = commandReceiver.GetModifiedRegisters(&modifiedRegisterCount);
    if(modifiedRegisterCount > 0)
    {
        printf("%d registers modified.\n", modifiedRegisterCount);
        for(int i = 0; i < modifiedRegisterCount; i++)
        {
            uint8_t registerAddress = modifiedRegisters[i];
            uint8_t registerValue = commandReceiver.GetRegisterValue(registerAddress);

            printf("register %x value changed to %x.\n", registerAddress, registerValue);
        }
    }
}

int main()
{
    stdio_init_all();
    status_indicator_init();
    
    commandReceiver.setup_command_receiver(I2C_SLAVE_SDA_PIN, I2C_SLAVE_SCL_PIN, I2C_SLAVE_BAUDRATE, I2C_SLAVE_ADDRESS, 500);

    while(true)
    {
        indicateActive();
        actionMovement();
    }
}
