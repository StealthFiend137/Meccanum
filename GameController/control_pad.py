#import typing  # from typing import Union, Dict # for hints
import inputs
import threading
import command_serializer
#import time

class Updates:
    w = 0
    x = 0
    y = 0
    
    

class ControlPad:
    
    # Analogue sticks.
    LEFT_ANALOG_X = 'ABS_X'
    LEFT_ANALOG_Y = 'ABS_Y'
    RIGHT_ANALOG_X = 'ABS_RX'
    RIGHT_ANALOG_Y = 'ABS_RY'
        
    def __init__(self):
        self._running = False
        self._updates = Updates()
        self._has_updates = False
                
        self._w_axis = AnalogAxis(self.LEFT_ANALOG_X, self._create_axis_callback('w'))
        self._x_axis = AnalogAxis(self.RIGHT_ANALOG_X, self._create_axis_callback('x'))
        self._y_axis = AnalogAxis(self.RIGHT_ANALOG_Y, self._create_axis_callback('y'))
    
    def _create_axis_callback(self, axis):
        def _axis_callback(value):
            setattr(self._updates, axis, value)          
            self._has_updates = True
        return _axis_callback
    
    @property
    def has_updates(self) -> bool:
        return self._has_updates
    
    def get_updates(self):
        self._has_updates = False
        return self._updates;

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
    
    def handle_event(self, event:inputs.InputEvent):
        if event.code == self._axis:
            self._callback(event.state)
        