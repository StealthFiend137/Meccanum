#include <hardware/i2c.h>

class I2cCommandReceiver
{
public:

    I2cCommandReceiver(i2c_inst_t* i2c)
    {
        i2c_instance = i2c;
    }
    
    void setup_command_receiver(uint sda_pin, uint scl_pin, uint baudrate, uint address, i2c_slave_handler_t handler)
    {
        gpio_init(sda_pin);
        gpio_pull_up(sda_pin);
        gpio_set_function(sda_pin, GPIO_FUNC_I2C);

        gpio_init(scl_pin);
        gpio_pull_up(scl_pin);
        gpio_set_function(scl_pin, GPIO_FUNC_I2C);

        i2c_init(i2c0, baudrate);
        i2c_slave_init(i2c0, address, handler);
    }

private:

    i2c_inst_t* i2c_instance;
};
