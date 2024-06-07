from smbus import SMBus
import time

bus = SMBus(1)
chassis_address = 0x20;

A = 0x00
B = 0x01

IODIR =   0X00 # IO Direction
IPOL =    0x02 # Invert polarity
GPINTEN = 0x04
DEFVAL =  0x06
INTCON =  0x08
IOCON =   0x0A # IO Configuration
GPPU =    0x0C
INTF =    0x0E
INTCAP =  0x10 # Interrupt Capture
GPIO =    0x12 # Data Ports (read the value on the port (pin) input)
OLAT =    0x14


#bus.write_byte_data(chassis_address, IODIR+A, 0xff)
#bus.write_byte_data(chassis_address, IODIR+B, 0xff)



#bus.write_byte_data(chassis_address, OLAT+A, 0xff)
#bus.write_byte_data(chassis_address, OLAT+B, 0xff)


adr = bus.read_i2c_block_data(chassis_address, IODIR+A, 22)
print(adr)