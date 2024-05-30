#include <hardware/i2c.h>
#include <hardware/uart.h>
#include <pico/stdlib.h>
#include <pico/i2c_slave.h>
#include <stdio.h>
#include <string.h>

#include "chassis/chassis.h"
#include "meccanum.h"
#include "i2cCommandReceiver/i2cCommandReceiver.h"

#define I2C_SLAVE_PORT i2c0
#define I2C_MASTER_PORT i2c1
#define LED_STATUS_INDICATOR 25

#define UART_INSTANCE uart0
#define UART_BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define motor_0_pwm_gpio 4
#define motor_0_direction_gpio 6
#define motor_1_pwm_gpio 5
#define motor_1_direction_gpio 10
#define motor_2_pwm_gpio 8
#define motor_2_direction_gpio 12
#define motor_3_pwm_gpio 9
#define motor_3_direction_gpio 13

// I2C
static const uint I2C_SLAVE_SDA_PIN = 16; // Green
static const uint I2C_SLAVE_SCL_PIN = 17; // Yellow
static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_SLAVE_BAUDRATE = 100000; // 100 kHz

// Will need to fine-tune this value to see what works best from the i2c host.
//TODO: just to make things even more fun, consider making this timeout configurable.
uint chassis_timeout_ms = 200;  

Chassis chassis(chassis_timeout_ms);
Meccanum drivetrain(&chassis,
    motor_0_pwm_gpio, motor_0_direction_gpio,
    motor_1_pwm_gpio, motor_1_direction_gpio,
    motor_2_pwm_gpio, motor_2_direction_gpio,
    motor_3_pwm_gpio, motor_3_direction_gpio);

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

void action_movement(int ms_since_boot)
{
    int elapsed_time = ms_since_boot - movementIntervalStartTime; 
    if(elapsed_time < 50) return;
    movementIntervalStartTime = ms_since_boot;
    drivetrain.action_updates();
};

int main()
{
    stdio_init_all();
    status_indicator_init();

    i2cCommandReceiver.command_receiver_init(I2C_SLAVE_SDA_PIN, I2C_SLAVE_SCL_PIN, I2C_SLAVE_BAUDRATE, I2C_SLAVE_ADDRESS);

    // uart_init(uart0, 115200);
    // gpio_set_function(0, GPIO_FUNC_UART);
    // gpio_set_function(1, GPIO_FUNC_UART);

    printf("\n==========================\nReady\n==========================\n");

    while(true)
    {
        auto ms_since_boot = to_ms_since_boot(get_absolute_time());
        indicate_status(ms_since_boot);
        action_movement(ms_since_boot);

        // if(uart_is_readable(uart0))
        // {
        //     char c = uart_getc(uart0);
        //     uart_putc(uart0, c);
        // }
    }
};
