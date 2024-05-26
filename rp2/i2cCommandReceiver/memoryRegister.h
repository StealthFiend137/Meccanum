#pragma once
#include <cstdint>

struct MemoryRegister
{
    uint8_t address;
    uint8_t value;
    bool externallyModifiable;
    bool modified;

    bool decayTimer;
    int decayPeriod_ms;
    int decayStartTimestamp;
    bool decayStarted;

    MemoryRegister(uint8_t address, uint8_t value, bool externallyModifiable);

    MemoryRegister(uint8_t address, uint8_t value, bool externallyModifiable, int decayPeriod_ms);
};
