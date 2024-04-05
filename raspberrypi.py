import smbus

channel = 1
bus = smbus.SMBus(channel)

address = 0x52

#bus.write_byte(address,12)
#bus.write_byte(address,15)
#bus.write_byte(address,-1)
#bus.write_byte(address,0)

values = [0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x04]
bus.write_i2c_block_data(address, 0x03, values)

print("sent")