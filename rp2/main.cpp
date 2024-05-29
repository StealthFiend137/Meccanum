#include <hardware/i2c.h>
#include <hardware/uart.h>
#include <pico/stdlib.h>
#include <pico/i2c_slave.h>
#include <stdio.h>
#include <string.h>

#include "chassis/chassis.h"
#include "i2cCommandReceiver/i2cCommandReceiver.h"

#define I2C_SLAVE_PORT i2c0
#define I2C_MASTER_PORT i2c1
#define LED_STATUS_INDICATOR 25

#define UART_INSTANCE uart0
#define UART_BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// I2C
static const uint I2C_SLAVE_SDA_PIN = 16; // Green
static const uint I2C_SLAVE_SCL_PIN = 17; // Yellow
static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_SLAVE_BAUDRATE = 100000; // 100 kHz

uint chassis_timeout_ms = 8000; // 8 seconds for testing only.
Chassis chassis(chassis_timeout_ms);
Chassis::Modified _modified = Chassis::Modified::none;

I2cCommandReceiver i2cCommandReceiver(I2C_SLAVE_PORT, &chassis);

int movementIntervalStartTime = to_ms_since_boot(get_absolute_time());
int modifiedRegisterCount;

void status_indicator_init()
{
    gpio_init(LED_STATUS_INDICATOR);
    gpio_set_dir(LED_STATUS_INDICATOR, GPIO_OUT);
};

void indicate_status(int ms_since_boot)
{
    //TODO: this is a terrible way of doing it. Make it better later, use an interrupt.
    //TODO: use this LED indicator to provide error codes like motherboard beep codes.
    auto ledOn = (ms_since_boot % 2000) < 1000;
    gpio_put(LED_STATUS_INDICATOR, ledOn);
};

void update_callback(Chassis::Modified modified)
{
    _modified = _modified | modified;
};

void action_movement(int ms_since_boot)
{
    int elapsed_time = ms_since_boot - movementIntervalStartTime; 
    if(elapsed_time < 10) return;

    movementIntervalStartTime = ms_since_boot;

    if (_modified == Chassis::Modified::none) return;

    if((_modified & Chassis::Modified::wAxisModified) == Chassis::Modified::wAxisModified)
    {
        int velocity = chassis.get_w_axis();
        printf("w Axis Modified to %d\n", velocity);
    }

    if((_modified & Chassis::Modified::xAxisModified) == Chassis::Modified::xAxisModified)
    {
        int velocity = chassis.get_x_axis();
        printf("x Axis Modified to %d\n", velocity);
    }

    if((_modified & Chassis::Modified::yAxisModified) == Chassis::Modified::yAxisModified)
    {
        int velocity = chassis.get_y_axis();
        printf("y Axis Modified to %d\n", velocity);
    }

    _modified = Chassis::Modified::none;
};

int main()
{
    stdio_init_all();
    status_indicator_init();
    chassis.register_callback(update_callback);
    
    i2cCommandReceiver.command_receiver_init(I2C_SLAVE_SDA_PIN, I2C_SLAVE_SCL_PIN, I2C_SLAVE_BAUDRATE, I2C_SLAVE_ADDRESS);

    printf("\n==========================\nReady\n==========================\n");

    while(true)
    {
        auto ms_since_boot = to_ms_since_boot(get_absolute_time());
        indicate_status(ms_since_boot);
        action_movement(ms_since_boot);
    }
};
