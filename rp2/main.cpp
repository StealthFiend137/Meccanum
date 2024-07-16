#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <hardware/pwm.h>

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

#define HIGH ControlPins::DigitalControlPin::PinState::High
#define LOW ControlPins::DigitalControlPin::PinState::Low


#define DRIVER0_ENABLE_PIN IoExtenders::Mcp23017::IoPin::GPB0
#define DRIVER1_ENABLE_PIN IoExtenders::Mcp23017::IoPin::GPB1

#define MOTOR0_PWM_PIN 4
#define MOTOR0_FORWARD_PIN IoExtenders::Mcp23017::IoPin::GPA0
#define MOTOR0_REVERSE_PIN IoExtenders::Mcp23017::IoPin::GPA1

#define MOTOR1_PWM_PIN 5
#define MOTOR1_FORWARD_PIN IoExtenders::Mcp23017::IoPin::GPA2
#define MOTOR1_REVERSE_PIN IoExtenders::Mcp23017::IoPin::GPA3

#define MOTOR2_PWM_PIN 20
#define MOTOR2_FORWARD_PIN IoExtenders::Mcp23017::IoPin::GPA4
#define MOTOR2_REVERSE_PIN IoExtenders::Mcp23017::IoPin::GPA5

#define MOTOR3_PWM_PIN 21
#define MOTOR3_FORWARD_PIN IoExtenders::Mcp23017::IoPin::GPA6
#define MOTOR3_REVERSE_PIN IoExtenders::Mcp23017::IoPin::GPA7

#define IOEXTENDER_RESET 6 
#define LED_STATUS_INDICATOR PICO_DEFAULT_LED_PIN

// Master I2C (for communication with onboard IO extender and multiplexed absolute rotation sensors.)
// ============================================================================
#define I2C_MASTER_PORT i2c1
#define I2C_MASTER_DATA_PIN 2
#define I2C_MASTER_AUX_CLOCK_PIN 3

// Slave I2C (for communication with an external contoller)
// ============================================================================
#define I2C_SLAVE_PORT i2c0
#define I2C_SLAVE_SDA_PIN 16
#define I2C_SLAVE_SCL_PIN 17
static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_SLAVE_BAUDRATE = 100000; // 100 kHz

// Will need to fine-tune this value to see what works best from the i2c host.
//TODO: just to make things even more fun, consider making this timeout configurable.
uint chassis_timeout_ms = 500;  
Chassis chassis(chassis_timeout_ms);

// Io extender multiplexer position.
// ============================================================================
I2cMultiplexer i2cMultiplexer(I2C_MASTER_PORT, I2C_MASTER_DATA_PIN, IOEXTENDER_RESET);
I2cMultiplexedChannel* auxiliary_i2c_channel = i2cMultiplexer.create_channel(I2C_MASTER_AUX_CLOCK_PIN);
IoExtenders::Mcp23017 mcp23017(auxiliary_i2c_channel, 0x20, IOEXTENDER_RESET);

// Motor direction control pins
// ============================================================================
ControlPins::DigitalControlPin* motor_0_cw = mcp23017.get_DigitalOutputControlPin(MOTOR0_FORWARD_PIN, LOW);
ControlPins::DigitalControlPin* motor_0_ccw = mcp23017.get_DigitalOutputControlPin(MOTOR0_REVERSE_PIN, LOW);
ControlPins::DigitalControlPin* motor_1_cw = mcp23017.get_DigitalOutputControlPin(MOTOR1_FORWARD_PIN, LOW);
ControlPins::DigitalControlPin* motor_1_ccw = mcp23017.get_DigitalOutputControlPin(MOTOR1_REVERSE_PIN, LOW);
ControlPins::DigitalControlPin* motor_2_cw = mcp23017.get_DigitalOutputControlPin(MOTOR2_FORWARD_PIN, LOW);
ControlPins::DigitalControlPin* motor_2_ccw = mcp23017.get_DigitalOutputControlPin(MOTOR2_REVERSE_PIN, LOW);
ControlPins::DigitalControlPin* motor_3_cw = mcp23017.get_DigitalOutputControlPin(MOTOR3_FORWARD_PIN, LOW);
ControlPins::DigitalControlPin* motor_3_ccw = mcp23017.get_DigitalOutputControlPin(MOTOR3_REVERSE_PIN, LOW);

// // Motor driver control pins
// // ============================================================================
ControlPins::DigitalControlPin* motor_driver_0_enable = mcp23017.get_DigitalOutputControlPin(DRIVER0_ENABLE_PIN, HIGH);
ControlPins::DigitalControlPin* motor_driver_1_enable = mcp23017.get_DigitalOutputControlPin(DRIVER1_ENABLE_PIN, HIGH);

// // Open loop motor controllers
// // ============================================================================
Motors::Motor* frontLeft = new Motors::OpenLoop(MOTOR0_PWM_PIN, motor_0_cw, motor_0_ccw);
Motors::Motor* frontRight = new Motors::OpenLoop(MOTOR1_PWM_PIN, motor_1_cw, motor_1_ccw);
Motors::Motor* rearLeft = new Motors::OpenLoop(MOTOR2_PWM_PIN, motor_2_cw, motor_2_ccw);
Motors::Motor* rearRight = new Motors::OpenLoop(MOTOR3_PWM_PIN, motor_3_cw, motor_3_ccw);
Meccanum drivetrain(&chassis, frontLeft, frontRight, rearLeft, rearRight);

// Slave i2c (for receiving communcation and instructions)
// ============================================================================
I2cCommandReceiver i2cCommandReceiver(I2C_SLAVE_PORT, &chassis);


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

int movementIntervalStartTime = to_ms_since_boot(get_absolute_time());
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

    // Delete of objects ommited as this program has no exit condition.
};
