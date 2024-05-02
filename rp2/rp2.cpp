#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>

#define I2C_SLAVE_PORT i2c0
#define RING_BUFFER_COUNT 8

// Telemetry Registers (3)
#define CELL0 0x00
#define CELL1 0x01
#define CELL2 0x02

// Movement Registers (7)
#define XDIR0 0x03
#define XDIR1 0X04
#define YDIR0 0x05
#define YDIR1 0X06
#define WDIR0 0x07
#define WDIR1 0x08
#define MOVET 0x09

// Sound Registers (5)
#define SOUND 0x0A
#define FREQ0 0x0B
#define DUTY0 0x0D
#define FREQ1 0x0D
#define DUTY1 0x0E

// Lighting Registers (2)
#define LIGH0 0x0F
#define LIGH1 0x10

// I2C
static const uint I2C_SLAVE_SDA_PIN = 16; // Green
static const uint I2C_SLAVE_SCL_PIN = 17; // Yellow
static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 100000; // 100 kHz

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

Context ring_buffer[RING_BUFFER_COUNT];
uint8_t ring_buffer_position = 0;
uint8_t next_process_position = 0;

static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    switch (event) {

        case I2C_SLAVE_RECEIVE: // master has initiated a connection
            if (!context.mem_address_written) {
                // reads and writes always start with the memory address
                uint8_t address = i2c_read_byte_raw(i2c);
                context.mem_address = address;
                context.mem_address_written = true;

                //ring_buffer_position = (ring_buffer_position % RING_BUFFER_COUNT);
                // context.bytes_received = 0;
                // context.final_received = false;
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

            // ring_buffer_position = (ring_buffer_position % RING_BUFFER_COUNT);
            // ring_buffer[ring_buffer_position] = context;
            break;

        default:
            break;
    }
}

static void setup_slave() {
    gpio_init(I2C_SLAVE_SDA_PIN);
    gpio_pull_up(I2C_SLAVE_SDA_PIN);
    gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_I2C);

    gpio_init(I2C_SLAVE_SCL_PIN);
    gpio_pull_up(I2C_SLAVE_SCL_PIN);
    gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_I2C);

    i2c_init(i2c0, I2C_BAUDRATE);
    i2c_slave_init(i2c0, I2C_SLAVE_ADDRESS, &i2c_slave_handler);
}

void registers_init()
{
    // Put some pretend values into the telemetry registers.
    // battery cell registers are - 0-255
    registers[CELL0][1] = 255;
    registers[CELL1][1] = 128;
    registers[CELL2][1] = 0;
}

int main()
{
    stdio_init_all();

    registers_init();

    setup_slave();

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
