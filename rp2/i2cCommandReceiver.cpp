#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include "registers.h"
#include <math.h>
#include <pico/time.h>

#define REGISTER_COUNT 16

class I2cCommandReceiver
{
private:

    struct MemoryRegister
    {
        uint8_t address;
        uint8_t value;
        bool externallyModifiable;
        bool modified;

        bool decayTimer;
        int default_decayTime_ms;
        int remaining_decayTime_ms;

        MemoryRegister(
            uint8_t address, uint8_t value, bool externallyModifiable)
            : address(address), value(value), externallyModifiable(externallyModifiable)
            {
                modified = false;

                decayTimer = false;
                default_decayTime_ms = 0;
                remaining_decayTime_ms = 0;
            }

        MemoryRegister(
            uint8_t address, uint8_t value, bool externallyModifiable, int default_decayTime_ms)
            : address(address), value(value), externallyModifiable(externallyModifiable), default_decayTime_ms(default_decayTime_ms)
            {
                modified = false;

                decayTimer = true;
                remaining_decayTime_ms = 0;
            }
    };

    i2c_inst_t* i2c_instance;

    inline static uint8_t modifiedRegisters[REGISTER_COUNT];
    inline static uint8_t address = 0;
    inline static bool addressWritten = false;

    inline static MemoryRegister* memoryRegisters[REGISTER_COUNT];

    static void Register_Change(uint8_t address, uint8_t value)
    {
        MemoryRegister *reg = memoryRegisters[address];

        if(!reg->externallyModifiable) 
        {
            return;
        }

        reg->value = value;

        if(reg->decayTimer)
        {
            reg->remaining_decayTime_ms = reg->default_decayTime_ms;
        };

        memoryRegisters[address]->modified = true;
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

    static void Register_Initialize(uint8_t address, uint8_t value, bool externallyModifiable, int decayTime_ms)
    {
        // NB: No need to delete as there is no exit condition for the program.
        memoryRegisters[address] = new MemoryRegister(address, value, externallyModifiable, decayTime_ms);
    }

    void registers_init()
    {
        // Telemetry Registers
        Register_Initialize(CELL0, 1, false);
        Register_Initialize(CELL1, 2, false);
        Register_Initialize(CELL2, 3, false);

        // Movement Registers
        int movementTimeout_ms = 2000;
        Register_Initialize(XDIR0, 4, true, movementTimeout_ms);
        Register_Initialize(XDIR1, 5, true, movementTimeout_ms);
        Register_Initialize(YDIR0, 6, true, movementTimeout_ms);
        Register_Initialize(YDIR1, 7, true, movementTimeout_ms);
        Register_Initialize(WDIR0, 8, true, movementTimeout_ms);
        Register_Initialize(WDIR1, 9, true, movementTimeout_ms);

        // Sound Registers
        Register_Initialize(SOUND, 10, true);
        Register_Initialize(FREQ0, 11, true);
        Register_Initialize(DUTY0, 12, true);
        Register_Initialize(FREQ1, 13, true);
        Register_Initialize(DUTY1, 14, true);

        // Lighting Registers
        Register_Initialize(LIGH0, 15, true);
        Register_Initialize(LIGH1, 16, true);
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
    
    struct repeating_timer timer;

    static bool movement_prune_callback(struct repeating_timer *t)
    {
        printf("repeating callback called\n");
        return true;
    };

public:

    I2cCommandReceiver(i2c_inst_t* i2c)
    {
        i2c_instance = i2c;
        add_repeating_timer_ms(2000, movement_prune_callback, NULL, &timer);
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
