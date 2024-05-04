from smbus import SMBus
import gpiod
import time

# Telemetry Registers
CELL0 = 0x00
CELL1 = 0x01
CELL2 = 0x02

# Movement Registers
XDIR0 = 0x03
XDIR1 = 0x04
XDIRT = 0x05
YDIR0 = 0x06
YDIR1 = 0x07
YDIRT = 0x08
WDIR0 = 0x09
WDIR1 = 0X0A
WDIRT = 0x0B

# Sound Registers
SOUND = 0x0C
FREQ0 = 0x0D
DUTY0 = 0x0E
FREQ1 = 0x0F
DUTY1 = 0x10

# Lighting Registers
LIGH0 = 0x11
LIGH1 = 0x12

#chip = gpiod.Chip('gpiochip4')
#scl = chip.get_line(2);
#sda = chip.get_line(3);
#print("I2C bus reset")


bus = SMBus(1)
chassis_address = 0x17;

#bus.write_byte_data(chassis_address, 0x01, 0x02)

#b = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19] # there are 19
#bus.write_i2c_block_data(chassis_address, 0x00, b)

xUpdate = [0, 0, 128]
bus.write_i2c_block_data(chassis_address, XDIR0, xUpdate)

time.sleep(0.01)
adr = bus.read_i2c_block_data(chassis_address, 0x00, 19)
print(adr)

print("sent")