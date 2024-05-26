#pragma once
#include <hardware/i2c.h>
#include <pico/i2c_slave.h>

#include "memoryRegister.h"
#include "../chassis/chassis.h"

#define REGISTER_COUNT 13

class I2cCommandReceiver
{
private:

    i2c_inst_t* i2c_instance;
    Chassis* chassis_instance;

    struct repeating_timer decay_timer;

    static uint8_t modifiedRegisters[REGISTER_COUNT];
    static MemoryRegister* memoryRegisters[REGISTER_COUNT];

    static inline uint8_t address; // = 0;
    static inline bool addressWritten; // = false;
    
    static void Register_Change(uint8_t address, uint8_t value);
    static uint8_t Register_External_Read(uint8_t address);

    static void Register_Initialize(uint8_t address, uint8_t value, bool externallyModifiable);
    static void Register_Initialize(uint8_t address, uint8_t value, bool externallyModifiable, int decayTime_ms);

    void registers_init(int movementTimeout_ms);
    static void i2c_slave_isr(i2c_inst_t *i2c, i2c_slave_event_t event);
    static bool movement_prune_callback(struct repeating_timer *t);

public:

    //I2cCommandReceiver(i2c_inst_t* i2c, Chassis* chassis);

    void setup_command_receiver(uint sda_pin, uint scl_pin, uint baudrate, uint address, int movementTimeout_ms);
    uint8_t* GetModifiedRegisters(int* count);
    uint8_t GetRegisterValue(uint8_t address);
};
