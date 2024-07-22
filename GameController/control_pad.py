#import typing  # from typing import Union, Dict # for hints
import inputs
import threading
import command_serializer
import copy
#import time

class Updates:
    
    w = 0
    x = 0
    y = 0
    
    w_updated = False
    x_updated = False
    y_updated = False
    
    def clear(self):
        self.w_updated = False
        self.x_updated = False
        self.y_updated = False

class ControlPad:
    
    # Analogue sticks.
    LEFT_ANALOG_X = 'ABS_X'
    LEFT_ANALOG_Y = 'ABS_Y'
    RIGHT_ANALOG_X = 'ABS_RX'
    RIGHT_ANALOG_Y = 'ABS_RY'
        
    def __init__(self):
        self._running = False
        self._updates = Updates()
                
        self._w_axis = AnalogAxis(self.RIGHT_ANALOG_X, self._create_axis_callback('w', 'w_updated'))
        self._x_axis = AnalogAxis(self.LEFT_ANALOG_Y, self._create_axis_callback('x', 'x_updated'))
        self._y_axis = AnalogAxis(self.LEFT_ANALOG_X, self._create_axis_callback('y', 'y_updated'))
    
    def _create_axis_callback(self, axis, flag):
        def _axis_callback(value):
            setattr(self._updates, axis, value)
            setattr(self._updates, flag, True)
        return _axis_callback
    
    @property
    def has_updates(self) -> bool:
        return self._updates.w_updated or self._updates.x_updated or self._updates.y_updated 
    
    def get_updates(self):
        updates = copy.copy(self._updates)
        self._updates.clear()
        return updates

    def start(self) -> None:
        self._running = True
        self._loop_thread = threading.Thread(target=self._loop, daemon=True)
        self._loop_thread.start()
    
    def stop(self) -> None:
        self._running = False
        self._loop_thread.join()
    
    def _handle_event(self, event):
        self._w_axis.handle_event(event)
        self._x_axis.handle_event(event)
        self._y_axis.handle_event(event)
    
    def _loop(self):
        while self._running:
            events = inputs.get_gamepad()
            for event in events:
                self._handle_event(event)


class AnalogAxis:
    def __init__(self, axis:str, callback):
        self._axis = axis
        self._callback = callback
        self._previous = 0
    
    def _map_range(self, value, from_min, from_max):
        from_span = from_max - from_min
        to_span = 100
        
        scale_value = (value - from_min) / from_span
        mapped_value = scale_value * to_span
        
        return int(round(mapped_value))
    
    def handle_event(self, event:inputs.InputEvent):
        JOYSTICK_MIN = 5000
        JOYSTICK_MAX = 32767   
        
        if event.code == self._axis:
            if abs(event.state) < JOYSTICK_MIN:
                if self._previous == 0:
                    return
                
                self._previous = 0
                self._callback(0)
                return
            
            percentage = self._map_range(abs(event.state), JOYSTICK_MIN, JOYSTICK_MAX)
            if event.state < 0:
                percentage = 0 - percentage
            
            if self._previous == percentage:
                return
            
            self._previous = percentage
            self._callback(percentage)
        
        