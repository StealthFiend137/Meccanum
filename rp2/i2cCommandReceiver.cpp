#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include "registers.h"
#include <math.h>

#define REGISTER_COUNT 19

class I2cCommandReceiver
{
private:

    struct MemoryRegister
    {
        uint8_t address;
        uint8_t value;
        bool externallyModifiable;
        bool modified = false;

        MemoryRegister(uint8_t address, uint8_t value, bool externallyModifiable)
            : address(address), value(value), externallyModifiable(externallyModifiable)
            {}
    };

    i2c_inst_t* i2c_instance;

    inline static uint8_t modifiedRegisters[REGISTER_COUNT];
    inline static uint8_t address = 0;
    inline static bool addressWritten = false;

    inline static MemoryRegister* memoryRegisters[REGISTER_COUNT];

    static void Register_Change(uint8_t address, uint8_t value)
    {
        if(!memoryRegisters[address]->externallyModifiable) 
        {
            return;
        }

        memoryRegisters[address]->modified = true;
        memoryRegisters[address]->value = value;
    }

    static uint8_t Register_External_Read(uint8_t address)
    {
        return memoryRegisters[address]->value;
    }

    static void Register_Initialize(uint8_t address, uint8_t value, bool externallyModifiable)
    {
        // NB: No need to delete as there is no exit condition for the program.
        memoryRegisters[address] = new MemoryRegister(address, value, externallyModifiable);
    }

    void registers_init()
    {
        // Telemetry Registers
        Register_Initialize(CELL0, 1, false);
        Register_Initialize(CELL1, 2, false);
        Register_Initialize(CELL2, 3, false);

        // Movement Registers
        Register_Initialize(XDIR0, 4, true);
        Register_Initialize(XDIR1, 5, true);
        Register_Initialize(XDIRT, 6, false);
        Register_Initialize(YDIR0, 7, true);
        Register_Initialize(YDIR1, 8, true);
        Register_Initialize(YDIRT, 9, false);
        Register_Initialize(WDIR0, 10, true);
        Register_Initialize(WDIR1, 0, true);
        Register_Initialize(WDIRT, 0, false);

        // Sound Registers
        Register_Initialize(SOUND, 0, true);
        Register_Initialize(FREQ0, 0, true);
        Register_Initialize(DUTY0, 0, true);
        Register_Initialize(FREQ1, 0, true);
        Register_Initialize(DUTY1, 0, true);

        // Lighting Registers
        Register_Initialize(LIGH0, 0, true);
        Register_Initialize(LIGH1, 0, true);
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
                i2c_write_byte_raw(i2c, Register_External_Read(address));
                address++;
                break;

            case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
                addressWritten = false;
                break;

             default:
                 break;
        }
    }

public:

    I2cCommandReceiver(i2c_inst_t* i2c)
    {
        i2c_instance = i2c;
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

    uint8_t* GetModifiedRegisters(int* count)
    {
        int modifiedRegisterCount = 0;
        
        for(MemoryRegister* memRegister : memoryRegisters)
        {
            if(memRegister->modified)
            {
                modifiedRegisters[modifiedRegisterCount] = memRegister->address;
                modifiedRegisterCount++;
            }
        }

        *count = modifiedRegisterCount;
        return modifiedRegisters;
    }

    uint8_t GetRegisterValue(uint8_t address)
    {
        memoryRegisters[address]->modified = false;
        return memoryRegisters[address]->value;
    }    
};
