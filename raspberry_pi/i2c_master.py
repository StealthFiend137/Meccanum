from smbus import SMBus
import time

bus = SMBus(1)
chassis_address = 0x17;

#bus.write_byte_data(chassis_address, 0x01, 0x02)

b = [1, 2, 3, 4, 5, 6, 8] # there are 8
bus.write_i2c_block_data(chassis_address, 0x01, b)

#time.sleep(0.01)

b = [1, 2, 3, 4, 5, 6, 8, 9] # there are 8
bus.write_i2c_block_data(chassis_address, 0x02, b)

time.sleep(0.01)

b = [1, 2, 3, 4, 5, 6, 8, 9, 10] # there are 8
bus.write_i2c_block_data(chassis_address, 0x03, b)

#time.sleep(0.01)

b = [1, 2, 3, 4, 5, 6, 8, 9, 10, 11] # there are 8
bus.write_i2c_block_data(chassis_address, 0x04, b)

print("sent")