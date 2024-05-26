#include <hardware/gpio.h>
#include <pico/time.h>

#include "i2cCommandReceiver.h"
#include "registers.h"

uint8_t I2cCommandReceiver::modifiedRegisters[REGISTER_COUNT];
MemoryRegister* I2cCommandReceiver::memoryRegisters[REGISTER_COUNT];

I2cCommandReceiver::I2cCommandReceiver(i2c_inst_t* i2c, Chassis* chassis)
{
    this->i2c_instance = i2c;
    I2cCommandReceiver::chassis_instance = chassis;
};

void I2cCommandReceiver::command_receiver_init(uint sda_pin, uint scl_pin, uint baudrate, uint addresss)
{
    gpio_init(sda_pin);
    gpio_pull_up(sda_pin);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);

    gpio_init(scl_pin);
    gpio_pull_up(scl_pin);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    
    i2c_init(i2c_instance, baudrate);
    i2c_slave_init(i2c_instance, address, i2c_slave_isr);
};

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
            I2cCommandReceiver::chassis_instance->wAxisModified;
            break;

        default:
            break;
    }
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
