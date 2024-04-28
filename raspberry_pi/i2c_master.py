from smbus import SMBus
import time

bus = SMBus(1)
chassis_address = 0x17;

bus.write_byte_data(chassis_address, 0x00, 0x00)