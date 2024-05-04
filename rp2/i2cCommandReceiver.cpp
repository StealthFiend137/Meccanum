#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include "registers.h"
#include <math.h>

class I2cCommandReceiver
{

private:

    struct MemoryRegister
    {
        uint8_t value;
        bool modified;
    };

    inline static MemoryRegister memoryRegisters[17];
    inline static bool registerChanged = false;
    inline static uint8_t address = 0;
    inline static bool addressWritten = false;

public:

    I2cCommandReceiver(i2c_inst_t* i2c)
    {
        i2c_instance = i2c;
        I2cCommandReceiver::registerChanged = false;
    }

    void setup_command_receiver(uint sda_pin, uint scl_pin, uint baudrate, uint address)
    {
        registers_init();

        gpio_init(sda_pin);
        gpio_pull_up(sda_pin);
        gpio_set_function(sda_pin, GPIO_FUNC_I2C);

        gpio_init(scl_pin);
        gpio_pull_up(scl_pin);
        gpio_set_function(scl_pin, GPIO_FUNC_I2C);
        
        i2c_init(i2c_instance, baudrate);
        i2c_slave_init(i2c_instance, address, i2c_slave_isr);
    }

private:

    i2c_inst_t* i2c_instance;

    static void Register_Change(uint8_t address, uint8_t value)
    {
        Register_Modify(address, value);
        Register_NotifyChange(address, value);
    }

    static void Register_Modify(uint8_t address, uint8_t value)
    {
        memoryRegisters[address].value = value;
    }

    static void Register_NotifyChange(uint8_t address, uint8_t value)
    {
        memoryRegisters[address].modified = true;
        registerChanged = true;
    }

    static uint8_t Register_Read(uint8_t address)
    {
        return memoryRegisters[address].value;
    }

    void registers_init()
    {
        // Put some pretend values into the telemetry registers.
        // battery cell registers are - 0-255
        Register_Modify(CELL0, 255);
        Register_Modify(CELL1, 128);
        Register_Modify(CELL2, 0);
    }

    static void i2c_slave_isr(i2c_inst_t *i2c, i2c_slave_event_t event)
    {
        switch (event) {

            case I2C_SLAVE_RECEIVE: // master has initiated a connection
                if(!addressWritten)
                {
                     // reads and writes always start with the memory address
                    address = i2c_read_byte_raw(i2c);
                    addressWritten = true;
                } else {
                     // master is writing data
                    uint8_t data = i2c_read_byte_raw(i2c);
                    Register_Change(address, data);
                    address++;
                 }
                break;

            case I2C_SLAVE_REQUEST: // master is requesting data
                i2c_write_byte_raw(i2c, Register_Read(address));
                address++;
                break;

            case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
                addressWritten = false;
                break;

             default:
                 break;
        }
    }
};
