import inputs
import DpadHandler



def old_handle_event(event):
    
    if event.ev_type == 'Key':
        if event.state == 1:
            print(f"key {event.code} pressed")
        else:
            print(f"key {event.code} released")
        return
    
    if event.ev_type == 'Absolute':
        if "ABS_HAT" in event.code:
            print(f"Hat: {event.code} moved to {event.state}")
            return    
        
        #if abs(event.state) < 3000:
        #    return
        #if event.code == 'ABS_RX':
        #    return
        #if event.code == 'ABS_RY':
        #    return
        #if event.code == 'ABS_X':
        #    return
        #if event.code == 'ABS_Y':
        #    return
        print(f"Axis: {event.code} moved to {event.state}")

# buttons# pressed 1, released 0
LEFT_BUMPER = 'BTL_TL'
RIGHT_BUMPER = 'BTN_TR'
SELECT = 'BTN_SELECT'
MODE = 'BTN_MODE'
START = 'BTN_START'
A = 'BTN_SOUTH'
B = 'BTN_EAST'
X = 'BTN_NORTH'
Y = 'BTN_WEST'
LEFT_THUMB = 'BTN_THUMBL'
RIGHT_THUMB = 'BTN_THUMBR'

# D Pad
# left/up -1
# down/right 1
# centre 0
DPAD_X = 'ABS_HAT0X'
DPAD_Y = 'ABS_HAT0Y'

# Triggers
# min 0 (released), max 255 (fully depressed)
LEFT_TRIGGER = 'ABS_Z' 
RIGHT_TRIGGER = 'ABS_RZ'

# Analogue sticks.
LEFT_ANALOG_X = 'ABS_X'
LEFT_ANALOG_Y = 'ABS_Y'
RIGHT_ANALOG_X = 'ABS_RX'
RIGHT_ANALOG_Y = 'ABS_RY'


def callback(result):
    print(f'{result} result received')

def handle_event(event):
    dpad_handler.handle_event(event)

dpadx_handler = DpadHandler.dpad_axis_handler('x', DPAD_X)
dpady_handler = DpadHandler.dpad_axis_handler('y', DPAD_Y)
dpad_handler = DpadHandler.dpad_handler(dpadx_handler, dpady_handler, callback)

try:
    print('ready')
    
    while True:    
        events = inputs.get_gamepad()
        for event in events:
            handle_event(event)
            
except KeyboardInterrupt:
    print("quit")