from command_serializer import Command, Serializer, Axis
from messenger import Messenger
from control_pad import ControlPad
import time

pad = ControlPad()
pad.start()

messenger = Messenger()
messenger.connect('192.168.0.50', 1883)

command = Command()
serializer = Serializer()

def keep_alive():
    return bytes([0b10000000])

def invert_axis(value):
    return 0-value

while True:
    
    time.sleep(0.1)
    
    if pad.has_updates:
        updates = pad.get_updates()
        if updates.w_updated:
            command.add_axis(Axis.w, updates.w)
        if updates.x_updated:
            command.add_axis(Axis.x, invert_axis(updates.x))
        if updates.y_updated:
            command.add_axis(Axis.y, updates.y)
            
        payload = serializer.serialize(command)
        messenger.send(payload)

        continue
    
    messenger.send(keep_alive())
