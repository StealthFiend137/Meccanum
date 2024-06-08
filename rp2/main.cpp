#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <hardware/i2c.h>
#include <hardware/uart.h>
#include <pico/stdlib.h>
#include <pico/i2c_slave.h>

#include "chassis/chassis.h"
#include "motors/motor.h"
#include "motors/openLoop.h"
#include "meccanum.h"
#include "i2cCommandReceiver/i2cCommandReceiver.h"
#include "i2cMultiplexer/i2cMultiplexer.h"

#include "ioExtenders/mcp23017.h"
#include "ioExtenders/mcp23017_ControlPin.h"

#include <time.h>

#define I2C_SLAVE_PORT i2c0
#define I2C_MASTER_PORT i2c1
#define LED_STATUS_INDICATOR 25

#define UART_INSTANCE uart0
#define UART_BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define MOTOR_0_PWM_GPIO 4
#define MOTOR_1_PWM_GPIO 5
#define MOTOR_2_PWM_GPIO 8
#define MOTOR_3_PWM_GPIO 9


int movementIntervalStartTime = to_ms_since_boot(get_absolute_time());


// Slave I2c (for communication with an external contoller)
static const uint I2C_SLAVE_SDA_PIN = 16; // Green
static const uint I2C_SLAVE_SCL_PIN = 17; // Yellow
static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_SLAVE_BAUDRATE = 100000; // 100 kHz

// Will need to fine-tune this value to see what works best from the i2c host.
//TODO: just to make things even more fun, consider making this timeout configurable.
uint chassis_timeout_ms = 200;  
Chassis chassis(chassis_timeout_ms);


// I2C Master channel setup
// ============================================================================

#define I2C_MASTER_DATA_PIN 2
#define IO_EXTENDER_ENABLE_PIN 14

I2cMultiplexer i2cMultiplexer(I2C_MASTER_PORT, I2C_MASTER_DATA_PIN, IO_EXTENDER_ENABLE_PIN);


// Motor rotation sensors
// ============================================================================

#define I2C_MASTER_MOTOR_0_CLOCK_PIN 3
#define I2C_MASTER_MOTOR_1_CLOCK_PIN 7
#define I2C_MASTER_MOTOR_2_CLOCK_PIN 11
#define I2C_MASTER_MOTOR_3_CLOCK_PIN 15

I2cMultiplexedChannel* _motors[4]
{
    i2cMultiplexer.create_channel(I2C_MASTER_MOTOR_0_CLOCK_PIN),
    i2cMultiplexer.create_channel(I2C_MASTER_MOTOR_1_CLOCK_PIN),
    i2cMultiplexer.create_channel(I2C_MASTER_MOTOR_2_CLOCK_PIN),
    i2cMultiplexer.create_channel(I2C_MASTER_MOTOR_3_CLOCK_PIN),
};


// Auxiliary i2c master
// ============================================================================

#define I2C_MASTER_AUX_CLOCK_PIN 19

I2cMultiplexedChannel* auxiliary_i2c_channel = i2cMultiplexer.create_channel(I2C_MASTER_AUX_CLOCK_PIN);
IoExtenders::Mcp23017 mcp23017(auxiliary_i2c_channel, 0x20);


// Motor driver control pins
// ============================================================================

ControlPins::DigitalControlPin* motor_0_cw = new ControlPins::Mcp23017_ControlPin(&mcp23017, IoExtenders::Mcp23017::Bank::A, 1);
ControlPins::DigitalControlPin* motor_0_ccw = new ControlPins::Mcp23017_ControlPin(&mcp23017, IoExtenders::Mcp23017::Bank::A, 2);
ControlPins::DigitalControlPin* motor_1_cw = new ControlPins::Mcp23017_ControlPin(&mcp23017, IoExtenders::Mcp23017::Bank::A, 3);
ControlPins::DigitalControlPin* motor_1_ccw = new ControlPins::Mcp23017_ControlPin(&mcp23017, IoExtenders::Mcp23017::Bank::A, 4);
ControlPins::DigitalControlPin* motor_2_cw = new ControlPins::Mcp23017_ControlPin(&mcp23017, IoExtenders::Mcp23017::Bank::A, 5);
ControlPins::DigitalControlPin* motor_2_ccw = new ControlPins::Mcp23017_ControlPin(&mcp23017, IoExtenders::Mcp23017::Bank::A, 6);
ControlPins::DigitalControlPin* motor_3_cw = new ControlPins::Mcp23017_ControlPin(&mcp23017, IoExtenders::Mcp23017::Bank::A, 7);
ControlPins::DigitalControlPin* motor_3_ccw = new ControlPins::Mcp23017_ControlPin(&mcp23017, IoExtenders::Mcp23017::Bank::A, 8);

ControlPins::DigitalControlPin* motor_driver_0_enable = new ControlPins::Mcp23017_ControlPin(&mcp23017, IoExtenders::Mcp23017::Bank::B, 1);
ControlPins::DigitalControlPin* motor_driver_1_enable = new ControlPins::Mcp23017_ControlPin(&mcp23017, IoExtenders::Mcp23017::Bank::B, 2);


// Open loop motor controllers
// ============================================================================

Motors::Motor* frontLeft = new Motors::OpenLoop(MOTOR_0_PWM_GPIO, motor_0_cw, motor_0_ccw);
Motors::Motor* frontRight = new Motors::OpenLoop(MOTOR_1_PWM_GPIO, motor_1_cw, motor_1_ccw);
Motors::Motor* rearLeft = new Motors::OpenLoop(MOTOR_2_PWM_GPIO, motor_2_cw, motor_2_ccw);
Motors::Motor* rearRight = new Motors::OpenLoop(MOTOR_3_PWM_GPIO, motor_3_cw, motor_3_ccw);
Meccanum drivetrain(&chassis, frontLeft, frontRight, rearLeft, rearRight);


// Slave i2c (for receiving communcation and instructions)
// ============================================================================
I2cCommandReceiver i2cCommandReceiver(I2C_SLAVE_PORT, &chassis);


// 
// ============================================================================

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


    motor_0_ccw->set_pin_state(ControlPins::DigitalControlPin::PinState::High);

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

    // Delete of objects ommited as this program has no exit condition.
};
