#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include "i2cCommandReceiver.cpp"
#include "registers.h"

#define I2C_SLAVE_PORT i2c0
#define I2C_MASTER_PORT i2c1

// I2C
static const uint I2C_SLAVE_SDA_PIN = 16; // Green
static const uint I2C_SLAVE_SCL_PIN = 17; // Yellow
static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_SLAVE_BAUDRATE = 100000; // 100 kHz

I2cCommandReceiver commandReceiver(I2C_SLAVE_PORT);

int main()
{
    stdio_init_all();
    
    commandReceiver.setup_command_receiver(I2C_SLAVE_SDA_PIN, I2C_SLAVE_SCL_PIN, I2C_SLAVE_BAUDRATE, I2C_SLAVE_ADDRESS);

    int startTime = to_ms_since_boot(get_absolute_time());
    int modifiedRegisterCount;

    while(1)
    {
        int elapsed_time = to_ms_since_boot(get_absolute_time()) - startTime; 

        if(elapsed_time > 10)
        {
            startTime = to_ms_since_boot(get_absolute_time());

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
        
        // do other things.
    }
}
