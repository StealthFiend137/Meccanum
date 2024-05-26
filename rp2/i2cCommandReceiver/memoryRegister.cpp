#include "memoryRegister.h"

MemoryRegister::MemoryRegister(
    uint8_t address, uint8_t value, bool externallyModifiable)
    : address(address), value(value), externallyModifiable(externallyModifiable)
{
    modified = false;

    decayTimer = false;
    decayPeriod_ms = 0;
    decayStartTimestamp = 0;
    decayStarted = false;
};

MemoryRegister::MemoryRegister(
    uint8_t address, uint8_t value, bool externallyModifiable, int decayPeriod_ms)
    : address(address), value(value), externallyModifiable(externallyModifiable), decayPeriod_ms(decayPeriod_ms)
{
    modified = false;

    decayTimer = true;
    decayStartTimestamp = 0;
    decayStarted = false;
};
