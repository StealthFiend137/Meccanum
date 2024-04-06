import machine
import utime

# Define the pin number for the passive buzzer
speed_pin = machine.Pin(4)


horn0 = machine.PWM(machine.Pin(17))
horn0.freq(3_000)
horn1 = machine.PWM(machine.Pin(21))
horn1.freq(500)

# Function to make the buzzer buzz for a certain duration
def buzz(duration_ms):
    horn0.duty_u16(32767)
    horn1.duty_u16(32767)  # Set duty cycle to maximum (range: 0-65535)
    utime.sleep_ms(duration_ms)
    horn0.duty_u16(0)
    horn1.duty_u16(0)

# Example usage
while True:
    buzz(1000)  # Buzz for 1 second
    utime.sleep(1)  # Wait for 1 second before buzzing again