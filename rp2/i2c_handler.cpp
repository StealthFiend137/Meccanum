class I2cCommandReceiver
{
public:
    I2cCommandReceiver(i2c_inst_t* i2c)
    {
        i2c_instance = i2c;
    }

private:
    i2c_inst_t* i2c_instance;
};
