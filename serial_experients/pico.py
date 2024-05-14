import machine
import time
import sys

# Configure serial communication over USB
uart = machine.UART(0, baudrate=115200)

# Function to receive and process incoming messages
def process_serial_data():
    if uart.any():
        received_data = uart.readline().strip()
        print("Received:", received_data.decode())

# Main loop to continuously check for incoming data
while True:
    process_serial_data()
    time.sleep(0.1)  # Adjust sleep time as needed
