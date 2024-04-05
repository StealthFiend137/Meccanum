from machine import mem32,Pin

IO_BANK0_BASE = 0x40014000

for gpio_number in range (0, 29):
    value = mem32[IO_BANK0_BASE | ((8 * gpio_number) + 4)]
    print(f"GPIO {gpio_number} : {value} : {bin(value)}")


#mem32[IO_BANK0_BASE | ((8 * 4) + 4)] = 3
#mem32[IO_BANK0_BASE | ((8 * 5) + 4)] = 3

#gpio0 = mem32[IO_BANK0_BASE | 4]
#gpio1 = mem32[IO_BANK0_BASE | (8 + 4)]
#print(bin(int(gpio0)))
#print(bin(int(gpio1)))