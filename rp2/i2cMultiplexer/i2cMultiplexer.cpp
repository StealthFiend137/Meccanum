#include "i2cMultiplexer.h"
#include "i2cMultiplexedChannel.h"
#include "pico/stdlib.h"

I2cMultiplexer::I2cMultiplexer(i2c_inst_t* i2c_instance, int data_pin, int enable_pin):
    _i2c_instance(i2c_instance), _data_pin(data_pin)
{

    i2c_init(this->_i2c_instance, 100000);
    
    gpio_set_function(data_pin, GPIO_FUNC_I2C);
    gpio_pull_up(data_pin);

    // Reset the MCP23017 on startup to get is into a known state.
    gpio_init(enable_pin);
    gpio_set_dir(enable_pin, GPIO_OUT);
    gpio_set_outover(enable_pin, GPIO_OVERRIDE_NORMAL);
    gpio_put(enable_pin, 0);
    sleep_us(1); // Param #30 
    gpio_put(enable_pin, 1);
};

void I2cMultiplexer::switch_clock_pin(int clock_pin)
{
    if(clock_pin == this->_current_clock_pin) return;

    if(this->_current_clock_pin != -1)
    {
        gpio_set_function(this->_current_clock_pin, GPIO_FUNC_NULL);
    }

    gpio_set_function(clock_pin, GPIO_FUNC_I2C);
    gpio_pull_up(clock_pin);
    
    this->_current_clock_pin = clock_pin;
};

I2cMultiplexedChannel* I2cMultiplexer::create_channel(int clock_pin)
{
    I2cMultiplexedChannel* channel = new I2cMultiplexedChannel(this, clock_pin);
    return channel;
};

int I2cMultiplexer::i2c_write_blocking(int clock_pin, uint8_t addr, const uint8_t *src, size_t len, bool nostop)
{
    this->switch_clock_pin(clock_pin);
    return ::i2c_write_blocking(this->_i2c_instance, addr, src, len, nostop);
};

int I2cMultiplexer::i2c_read_blocking(int clock_pin, uint8_t addr, uint8_t *dst, size_t len, bool nostop)
{
    this->switch_clock_pin(clock_pin);
    return ::i2c_read_blocking(this->_i2c_instance, addr, dst, len, nostop);
};