from smbus import SMBus
import gpiod
import time

# Telemetry Registers
CELL0 = 0x00
CELL1 = 0x01
CELL2 = 0x02

# Movement Registers
XDIR0 = 0x03
XDIR1 = 0X04
YDIR0 = 0x05
YDIR1 = 0x06
WDIR0 = 0x07
WDIR1 = 0X08
MOVET = 0x09

# Sound Registers
SOUND = 0x0A
FREQ0 = 0x0B
DUTY0 = 0x0C
FREQ1 = 0x0D
DUTY1 = 0x0E

# Lighting Registers
LIGH0 = 0x0F
LIGH1 = 0x10

#chip = gpiod.Chip('gpiochip4')
#scl = chip.get_line(2);
#sda = chip.get_line(3);
#print("I2C bus reset")


bus = SMBus(1)
chassis_address = 0x17;

#bus.write_byte_data(chassis_address, 0x01, 0x02)

b = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17] # there are 17
bus.write_i2c_block_data(chassis_address, 0x00, b)

time.sleep(0.01)
adr = bus.read_i2c_block_data(chassis_address, CELL0, 17)
print(adr)

print("sent")