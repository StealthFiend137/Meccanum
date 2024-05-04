#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include "i2cCommandReceiver.cpp"
#include "registers.h"

#define I2C_SLAVE_PORT i2c0
#define I2C_MASTER_PORT i2c1
#define RING_BUFFER_COUNT 8

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

    while(1)
    {
        // while(!current->processed)
        // {
        //     current->processed = true;
        //     current->final_received = false;
        //     next_process_position = (next_process_position++ % RING_BUFFER_COUNT);
        //     printf("Written %02X bytes, at address starting %02X\n", current->bytes_received, current->mem_address);
        //     Context* current = &ring_buffer[next_process_position];
        // }


        //sleep_ms(3000);
        /*
        if(context.final_received)
        {
            // printf("Write at 0x%02X: '%s'\n", mem_address, msg);
            printf("Written %02X bytes, at address starting %02X\n", context.bytes_received, context.mem_address);
            context.final_received = false;
        }
        */
    }
}
