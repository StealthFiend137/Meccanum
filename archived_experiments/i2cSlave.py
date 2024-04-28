from machine import mem32,Pin

class i2c_slave:
    I2C0_BASE = 0x40044000
    I2C1_BASE = 0x40048000
    IO_BANK0_BASE = 0x40014000
    
    # Registers
    IC_CON = 0x0
    IC_TAR = 0x4
    
    IC_SAR_OFFSET = 0x08
    
    IC_DATA_CMD = 0x10 
    IC_RX_TL = 0x38
    IC_TX_TL = 0x3C
    IC_CLR_INTR = 0x40
    
    IC_ENABLE_OFFSET = 0x6c
    IC_ENABLE_ENABLE_BIT = 0
    
    IC_STATUS = 0x70
        
    def set_bit(self, value, bit):
        return value | (1<<bit)
    
    def clear_bit(self, value, bit):
        return value & ~(1<<bit)
    
    def disable_DW_apb_i2c(self):
        REGISTER_ADDRESS = self.i2c_base | self.IC_ENABLE_OFFSET
        
        current_value = mem32[REGISTER_ADDRESS]
        new_value = self.clear_bit(current_value, self.IC_ENABLE_ENABLE_BIT)
        mem32[REGISTER_ADDRESS] = new_value
    
    def enable_DW_apb_i2c(self):
        REGISTER_ADDRESS = self.i2c_base | self.IC_ENABLE_OFFSET
        
        current_value = mem32[REGISTER_ADDRESS]
        new_value = self.set_bit(current_value, self.IC_ENABLE_ENABLE_BIT)
        mem32[REGISTER_ADDRESS] = new_value
        
    def set_slave_address(self, slave_address):
        REGISTER_ADDRESS = self.i2c_base | self.IC_SAR_OFFSET
        MASK = 0x1FF
        
        current_value = mem32[REGISTER_ADDRESS]
        masked_value = ~MASK & current_value
        new_value = masked_value | slave_address
        mem32[REGISTER_ADDRESS] = new_value
        
    def set_as_slave(self):
        REGISTER_ADDRESS = self.i2c_base | self.IC_CON
        IC_10BITADDR_SLAVE_BIT = 3
        SLAVE_DISABLE_BIT = 6
        MASTER_MODE_BIT = 0
        
        value = mem32[REGISTER_ADDRESS]
        value = self.clear_bit(value, IC_10BITADDR_SLAVE_BIT)
        value = self.clear_bit(value, SLAVE_DISABLE_BIT)
        value = self.clear_bit(value, MASTER_MODE_BIT)
        mem32[REGISTER_ADDRESS] = value

    def get_base_address(self, i2cID):
        if 0 == i2cID:
            return self.I2C0_BASE
        else:
            return self.I2C1_BASE
    
    def clear_bus_0(self, sda, scl):
        FIRST_SDA = 0
        LAST_SDA = 28
        for pair in range(FIRST_SDA, LAST_SDA+1, 4):
            for offset in range (pair, pair + 2):
                if(3 == mem32[self.IO_BANK0_BASE | ((8 * offset) + 4)]):
                    mem32[self.IO_BANK0_BASE | ((8 * offset) + 4)] = 31
    
    def clear_bus_1(self, sda, scl):
        FIRST_SDA = 2
        LAST_SDA = 26
        for pair in range(FIRST_SDA, LAST_SDA+1, 4):
            for offset in range (pair, pair + 2):
                if(3 == mem32[self.IO_BANK0_BASE | ((8 * offset) + 4)]):
                    mem32[self.IO_BANK0_BASE | ((8 * offset) + 4)] = 31
    
    def set_pins(self, i2c_bus, sda, scl):
        if 0 == i2c_bus:
            self.clear_bus_0(sda, scl)
        else:
            self.clear_bus_1(sda, scl)
            
        mem32[self.IO_BANK0_BASE | ((8 * scl) + 4)] = 3
        mem32[self.IO_BANK0_BASE | ((8 * sda) + 4)] = 3
        
        # find any gpios for the selected i2c bus (0 or 1) that = 3 and
        # overwrite with value of 31 to null them.
        # update the requested scl and sda pins with the value 3.

        # i2c0
        # sda, scl
        # 0, 1
        # 4, 5
        # 8, 9
        # 12, 13
        # 16, 17
        # 20, 21
        # 24, 25
        # 28, 29
        
        # i2c1
        # sda, scl
        # 2, 3
        # 6, 7
        # 10, 11
        # 14, 15
        # 18, 19
        # 22, 23
        # 26, 27
    
    def __init__(self, i2c_bus = 0, sda=0, scl=1,slave_address=0x41):
        self.i2c_base = self.get_base_address(i2c_bus)
        
        self.disable_DW_apb_i2c()
        
        self.set_slave_address(slave_address)
        self.set_as_slave()
        self.set_pins(i2c_bus, scl, sda)
        
        self.enable_DW_apb_i2c()

    def any(self):
        # get IC_STATUS
        status = mem32[ self.i2c_base | self.IC_STATUS]
        # check RFNE receive fifio not empty
        if (status &  8) :
            return True
        return False
    
    def get(self):
        while not self.any():
            pass
        return mem32[ self.i2c_base | self.IC_DATA_CMD] & 0xff

        