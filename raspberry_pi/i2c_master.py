from smbus import SMBus
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
DUTY0 = 0x0D
FREQ1 = 0x0D
DUTY1 = 0x0E

# Lighting Registers
LIGH0 = 0x0F
LIGH1 = 0x10

bus = SMBus(1)
chassis_address = 0x17;

#bus.write_byte_data(chassis_address, 0x01, 0x02)

#b = [1, 2, 3, 4, 5, 6, 8] # there are 8
#bus.write_i2c_block_data(chassis_address, 0x01, b)

time.sleep(0.01)
adr = bus.read_i2c_block_data(chassis_address, CELL0, 3)

print(adr)

print("sent")