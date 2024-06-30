#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

#include "i2cMultiplexer/i2cMultiplexer.h"
#include "ioExtenders/mcp23017.h"
#include "ioExtenders/mcp23017_ControlPin.h"

#define DRIVER0_ENABLE_PIN IoExtenders::Mcp23017::IoPin::GPB0

#define MOTOR0_PWM_PIN 4
#define MOTOR0_FORWARD_PIN IoExtenders::Mcp23017::IoPin::GPA0
#define MOTOR0_REVERSE_PIN IoExtenders::Mcp23017::IoPin::GPA1

#define MOTOR1_PWM_PIN 5
#define MOTOR1_FORWARD_PIN IoExtenders::Mcp23017::IoPin::GPA2
#define MOTOR1_REVERSE_PIN IoExtenders::Mcp23017::IoPin::GPA3

// #define MOTOR2_PWM_PIN 20
// #define MOTOR2_FORWARD_PIN IoExtenders::Mcp23017::IoPin::GPA4
// #define MOTOR2_REVERSE_PIN IoExtenders::Mcp23017::IoPin::GPA5

// #define MOTOR3_PWM_PIN 21
// #define MOTOR3_FORWARD_PIN IoExtenders::Mcp23017::IoPin::GPA6
// #define MOTOR3_REVERSE_PIN IoExtenders::Mcp23017::IoPin::GPA7

#define IOEXTENDER_RESET 6 

#define I2C_MASTER_PORT i2c1
#define I2C_MASTER_DATA_PIN 2
#define IO_EXTENDER_ENABLE_PIN 14
#define I2C_MASTER_AUX_CLOCK_PIN 3

I2cMultiplexer i2cMultiplexer(I2C_MASTER_PORT, I2C_MASTER_DATA_PIN, IO_EXTENDER_ENABLE_PIN);
I2cMultiplexedChannel* auxiliary_i2c_channel = i2cMultiplexer.create_channel(I2C_MASTER_AUX_CLOCK_PIN);
IoExtenders::Mcp23017 mcp23017(auxiliary_i2c_channel, 0x20);

void on_pwm_wrap_isr()
{
    static int fade = 0;
    static bool going_up = true;

    pwm_clear_irq(pwm_gpio_to_slice_num(MOTOR0_PWM_PIN));

    if(going_up)
    {
        ++fade;
        if (fade > 255)
        {
            fade = 255;
            going_up = false;
        }
    }
    else
    {
        --fade;
        if(fade < 0)
        {
            fade = 0;
            going_up = true;
        }
    }

    pwm_set_gpio_level(MOTOR0_PWM_PIN, fade*fade);
    pwm_set_gpio_level(MOTOR1_PWM_PIN, fade*fade);
}

void enable_ioextender()
{
    gpio_init(IOEXTENDER_RESET);
    gpio_set_dir(IOEXTENDER_RESET, GPIO_OUT);
    gpio_put(IOEXTENDER_RESET, 1);
    mcp23017.set_pin_as_output(DRIVER0_ENABLE_PIN);
    mcp23017.set_pin_as_output(MOTOR0_FORWARD_PIN);
    mcp23017.set_pin_as_output(MOTOR0_REVERSE_PIN);
    mcp23017.set_pin_as_output(MOTOR1_FORWARD_PIN);
    mcp23017.set_pin_as_output(MOTOR1_REVERSE_PIN);
}

void enable_pwm()
{
    gpio_set_function(MOTOR0_PWM_PIN, GPIO_FUNC_PWM);
    gpio_set_function(MOTOR1_PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(MOTOR0_PWM_PIN);

    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap_isr);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.f);
    pwm_init(slice_num, &config, true);
}


void enable_motor_driver(int motor_drive_number, bool enabled)
{
    mcp23017.set_pin_state(IoExtenders::Mcp23017::IoPin::GPB0, IoExtenders::Mcp23017::IoPinOutputState::high);
}

enum class Direction : int
{
    ShortBrake = 0,
    Stop = 1,
    Forwards = 2,
    Reverse = 3,
};

enum class Motor : int
{
    Motor0 = 0,
    Motor1 = 1, 
    Motor2 = 2,
    Motor3 = 3
};

void test_direction(Motor motor, Direction direction)
{
    IoExtenders::Mcp23017::IoPin forwards;
    IoExtenders::Mcp23017::IoPin reverse;
    
    switch(motor)
    {
        case Motor::Motor0:
            forwards = MOTOR0_FORWARD_PIN;
            reverse = MOTOR0_REVERSE_PIN;
            break;

        case Motor::Motor1:
            forwards = MOTOR1_FORWARD_PIN;
            reverse = MOTOR1_REVERSE_PIN;
            break;
    }


    const IoExtenders::Mcp23017::IoPinOutputState HIGH = IoExtenders::Mcp23017::IoPinOutputState::high;
    const IoExtenders::Mcp23017::IoPinOutputState LOW = IoExtenders::Mcp23017::IoPinOutputState::low;
    switch(direction)
    {
        case(Direction::Forwards):
            mcp23017.set_pin_state(forwards, HIGH);
            mcp23017.set_pin_state(reverse, LOW);
            break;

        case(Direction::Reverse):
            mcp23017.set_pin_state(forwards, LOW);
            mcp23017.set_pin_state(reverse, HIGH);
            break;

        case (Direction::ShortBrake):
            mcp23017.set_pin_state(forwards, HIGH);
            mcp23017.set_pin_state(reverse, HIGH);
            break;

        case (Direction::Stop):
            mcp23017.set_pin_state(forwards, LOW);
            mcp23017.set_pin_state(reverse, LOW);
            break;
    }
}

int main()
{
    stdio_init_all();

    enable_ioextender();
    enable_pwm();


    enable_motor_driver(1, true);
    sleep_ms(500);

    test_direction(Motor::Motor0, Direction::Forwards);
    sleep_ms(1000);
    test_direction(Motor::Motor0, Direction::Reverse);
    sleep_ms(1000);
    test_direction(Motor::Motor0, Direction::ShortBrake);
    sleep_ms(1000);
    test_direction(Motor::Motor0, Direction::Stop);

    sleep_ms(2000);

    test_direction(Motor::Motor1, Direction::Forwards);
    sleep_ms(1000);
    test_direction(Motor::Motor1, Direction::Reverse);
    sleep_ms(1000);
    test_direction(Motor::Motor1, Direction::ShortBrake);
    sleep_ms(1000);
    test_direction(Motor::Motor1, Direction::Stop);




    while(true)
    {
        tight_loop_contents();
    }
}
