from smbus import SMBus
import gpiod
import time

# Movement Registers
WDIR = 0x00
XDIR = 0x01
YDIR = 0X02

# Sound Registers
SOUND = 0x03
FREQ0 = 0x04
DUTY0 = 0x05
FREQ1 = 0x06
DUTY1 = 0x07

# Lighting Registers
LIGH0 = 0x08
LIGH1 = 0x09

# Telemetry Registers
CELL0 = 0x0A
CELL1 = 0x0B
CELL2 = 0x0C

#chip = gpiod.Chip('gpiochip4')
#scl = chip.get_line(2);
#sda = chip.get_line(3);
#print("I2C bus reset")


bus = SMBus(1)
chassis_address = 0x17;

#bus.write_byte_data(chassis_address, 0x01, 0x02)

#b = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19] # there are 19
#bus.write_i2c_block_data(chassis_address, 0x00, b)

def set_speed(num):
    xUpdate = [0, 0, num]
    bus.write_i2c_block_data(chassis_address, WDIR, xUpdate)
    time.sleep(0.2)

#while True:
set_speed(100);


#set_speed(75);
#set_speed(50);
#set_speed(25);
#set_speed(100);

#time.sleep(0.01)
#adr = bus.read_i2c_block_data(chassis_address, DUTY0, 20)
#print(adr)

print("sent")