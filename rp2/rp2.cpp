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

uint8_t registers[17][2];

static struct Context
{
    uint8_t mem_address;
    bool mem_address_written = false;
    uint8_t mem[256];
    uint8_t bytes_received = 0;
    bool final_received = false;
    bool processed = true;
} context;

void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    switch (event) {

        case I2C_SLAVE_RECEIVE: // master has initiated a connection
            if (!context.mem_address_written) {
                // reads and writes always start with the memory address
                uint8_t address = i2c_read_byte_raw(i2c);
                context.mem_address = address;
                context.mem_address_written = true;
            } else {
                // master is writing data
                uint8_t data = i2c_read_byte_raw(i2c);
                context.mem[context.bytes_received] = data;
                context.bytes_received++;
            }
            break;

        case I2C_SLAVE_REQUEST: // master is requesting data
            // load from memory
            i2c_write_byte_raw(i2c, registers[context.mem_address][1]);
            context.mem_address++;
            break;

        case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
            context.mem_address_written = false;
            context.final_received = true;
            context.processed = false;
            break;

        default:
            break;
    }
}



void registers_init()
{
    // Put some pretend values into the telemetry registers.
    // battery cell registers are - 0-255
    registers[CELL0][1] = 245;
    registers[CELL1][1] = 128;
    registers[CELL2][1] = 0;
}

I2cCommandReceiver i2cHandler(i2c0);

int main()
{
    stdio_init_all();
    registers_init();
    
    auto commandReceiver = I2cCommandReceiver(I2C_SLAVE_PORT);
    commandReceiver.setup_command_receiver(I2C_SLAVE_SDA_PIN, I2C_SLAVE_SCL_PIN, I2C_SLAVE_BAUDRATE, I2C_SLAVE_ADDRESS, &i2c_slave_handler);

    // Context* current = &ring_buffer[next_process_position];

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
