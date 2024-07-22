from command_serializer import Command, Serializer
from messenger import Messenger
from control_pad import ControlPad
import time

pad = ControlPad()
pad.start()

while True:
    
    
    if pad.has_updates:
        
        updates = pad.get_updates()
        print(f'updates: w {updates.w}, x {updates.x}, y {updates.y}')
        continue
        
    time.sleep(0.01)