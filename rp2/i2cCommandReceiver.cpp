#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include "registers.h"
#include <math.h>

#define REGISTER_COUNT 19

class I2cCommandReceiver
{

public:

    struct MemoryRegister
    {
        uint8_t value;
        bool externallyModifiable;
        bool modified;
    };

private:

    inline static MemoryRegister memoryRegisters[REGISTER_COUNT];
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

    bool GetAnyRegisterChanged()
    {
        //return registerChanged;
        return true;
    }

    void ClearAllChangedRegisters()
    {
        registerChanged = false;
    }

    MemoryRegister fakeMemreg = MemoryRegister();

    int GetChangedArrays(I2cCommandReceiver::MemoryRegister** changedRegisters)
    {
        int changedRegistersCount = 0;

        for(MemoryRegister memRegister : memoryRegisters)
        {
            if(memRegister.modified || memRegister.externallyModifiable)
            {
                MemoryRegister* memRegisterPtr = &memRegister;
                
                //delete changedRegisters[changedRegistersCount];
                changedRegisters[changedRegistersCount] = memRegisterPtr;
                changedRegistersCount++;

                // changedRegisters[changedRegistersCount] = &memRegister;
                // changedRegistersCount++;
            }
        }

        return changedRegistersCount;
    }

private:

    i2c_inst_t* i2c_instance;

    static void Register_Change(uint8_t address, uint8_t value)
    {
        if(!memoryRegisters[address].externallyModifiable) 
        {
            return;
        }

        memoryRegisters[address].value = value;
        memoryRegisters[address].modified = true;
        registerChanged = true;
    }

    static uint8_t Register_Read(uint8_t address)
    {
        return memoryRegisters[address].value;
    }

    static void Register_Set(uint8_t address, uint8_t value, bool externallyModifiable)
    {
        memoryRegisters[address].value = value;
        memoryRegisters[address].externallyModifiable = externallyModifiable;
        memoryRegisters[address].modified = false;
    }

    void registers_init()
    {
        // Telemetry Registers
        Register_Set(CELL0, 1, false);
        Register_Set(CELL1, 2, false);
        Register_Set(CELL2, 3, false);

        // Movement Registers
        Register_Set(XDIR0, 4, true);
        Register_Set(XDIR1, 5, true);
        Register_Set(XDIRT, 6, false);
        Register_Set(YDIR0, 7, true);
        Register_Set(YDIR1, 8, true);
        Register_Set(YDIRT, 9, false);
        Register_Set(WDIR0, 10, true);
        Register_Set(WDIR1, 0, true);
        Register_Set(WDIRT, 0, false);

        // Sound Registers
        Register_Set(SOUND, 0, true);
        Register_Set(FREQ0, 0, true);
        Register_Set(DUTY0, 0, true);
        Register_Set(FREQ1, 0, true);
        Register_Set(DUTY1, 0, true);

        // Lighting Registers
        Register_Set(LIGH0, 0, true);
        Register_Set(LIGH1, 0, true);
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
