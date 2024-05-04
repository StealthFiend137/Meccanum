#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include "registers.h"

class I2cCommandReceiver
{

public:

    I2cCommandReceiver(i2c_inst_t* i2c)
    {
        i2c_instance = i2c;
    }

    void setup_command_receiver(uint sda_pin, uint scl_pin, uint baudrate, uint address)
    {
        gpio_init(sda_pin);
        gpio_pull_up(sda_pin);
        gpio_set_function(sda_pin, GPIO_FUNC_I2C);

        gpio_init(scl_pin);
        gpio_pull_up(scl_pin);
        gpio_set_function(scl_pin, GPIO_FUNC_I2C);
        i2c_init(i2c0, baudrate);
        i2c_slave_init(i2c_instance, address, i2c_slave_isr);

        registers_init();
    }


private:

    i2c_inst_t* i2c_instance;

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


    void registers_init()
    {
        // Put some pretend values into the telemetry registers.
        // battery cell registers are - 0-255
        registers[CELL0][1] = 245;
        registers[CELL1][1] = 128;
        registers[CELL2][1] = 0;
    }

    static void i2c_slave_isr(i2c_inst_t *i2c, i2c_slave_event_t event)
    {
        switch (event) {

            case I2C_SLAVE_RECEIVE: // master has initiated a connection
                 if (!context.mem_address_written) {
                     // reads and writes always start with the memory address
        //             uint8_t address = i2c_read_byte_raw(i2c);
        //             context.mem_address = address;
        //             context.mem_address_written = true;
                 } else {
                     // master is writing data
        //             uint8_t data = i2c_read_byte_raw(i2c);
        //             context.mem[context.bytes_received] = data;
        //             context.bytes_received++;
                 }
                break;

            case I2C_SLAVE_REQUEST: // master is requesting data
            
                // load from memory
                // i2c_write_byte_raw(i2c, instance->registers[instance->context.mem_address][1]);
                // instance->context.mem_address++;
                break;

            case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
        //         context.mem_address_written = false;
        //         context.final_received = true;
        //         context.processed = false;
                break;

             default:
                 break;
        }
    }
};
