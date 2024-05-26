#include <hardware/gpio.h>
#include <pico/time.h>

#include "i2cCommandReceiver.h"
#include "../registers.h"

#define REGISTER_COUNT 16

uint8_t I2cCommandReceiver::modifiedRegisters[REGISTER_COUNT];
MemoryRegister* I2cCommandReceiver::memoryRegisters[REGISTER_COUNT];

void I2cCommandReceiver::Register_Change(uint8_t address, uint8_t value)
{
    MemoryRegister *reg = memoryRegisters[address];

    if(!reg->externallyModifiable) 
    {
        return;
    }

    reg->value = value;

    if(reg->decayTimer)
    {
        reg->decayStartTimestamp = to_ms_since_boot(get_absolute_time());
        reg->decayStarted = true;
    };

    reg->modified = true;
};

uint8_t I2cCommandReceiver::Register_External_Read(uint8_t address)
{
    return memoryRegisters[address]->value;
};

void I2cCommandReceiver::Register_Initialize(uint8_t address, uint8_t value, bool externallyModifiable)
{
    memoryRegisters[address] = new MemoryRegister(address, value, externallyModifiable);
};

void I2cCommandReceiver::Register_Initialize(uint8_t address, uint8_t value, bool externallyModifiable, int decayTime_ms)
{
    memoryRegisters[address] = new MemoryRegister(address, value, externallyModifiable, decayTime_ms);
};

void I2cCommandReceiver::registers_init(int movementTimeout_ms)
{
    // Telemetry Registers
    Register_Initialize(CELL0, 1, false);
    Register_Initialize(CELL1, 2, false);
    Register_Initialize(CELL2, 3, false);

    // Movement Registers
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
};

void I2cCommandReceiver::i2c_slave_isr(i2c_inst_t *i2c, i2c_slave_event_t event)
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
};

bool I2cCommandReceiver::movement_prune_callback(struct repeating_timer *t)
{
    int ms_sinceBoot = to_ms_since_boot(get_absolute_time());

    for(MemoryRegister* memRegister : memoryRegisters)
    {
        if(!memRegister->decayTimer || !memRegister->decayStarted)
        {
            continue;
        };

        if(ms_sinceBoot - memRegister->decayStartTimestamp > memRegister->decayPeriod_ms)
        {
            memRegister->value = 0;
            memRegister->decayStarted = false;
            memRegister->modified = true;
        };
    };

    return true;
};

// I2cCommandReceiver::I2cCommandReceiver(i2c_inst_t* i2c, Chassis* chassis)
// {
//     i2c_instance = i2c;
//     chassis_instance = chassis;

//     const int interval_ms = 20;
//     add_repeating_timer_ms(interval_ms, movement_prune_callback, NULL, &decay_timer);
// };

void I2cCommandReceiver::setup_command_receiver(uint sda_pin, uint scl_pin, uint baudrate, uint address, int movementTimeout_ms)
{
    registers_init(movementTimeout_ms);

    gpio_init(sda_pin);
    gpio_pull_up(sda_pin);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);

    gpio_init(scl_pin);
    gpio_pull_up(scl_pin);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    
    i2c_init(i2c_instance, baudrate);
    i2c_slave_init(i2c_instance, address, i2c_slave_isr);
};

uint8_t* I2cCommandReceiver::GetModifiedRegisters(int* count)
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
};

uint8_t I2cCommandReceiver::GetRegisterValue(uint8_t address)
{
    memoryRegisters[address]->modified = false;
    return memoryRegisters[address]->value;
};
