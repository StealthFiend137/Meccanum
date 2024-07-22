from typing import Union, Dict # for hints
from enum import Enum
import struct

class Axis(Enum):
    w = 1
    x = 2
    y = 3

class Command:

    def __init__(self):
        self.axes:Dict[Axis, int] = {}
    
    def add_axis(self, axis:Axis, value:int) -> None:
        self._Guard.check_axis_type(axis)
        self._Guard.check_value_type(value)
        self._Guard.check_value_range(value)
        
        self.axes[axis] = value
        
    def get_axes(self) -> Dict[Axis, int]:
        sorted_dict = dict(sorted(self.axes.items(), key=lambda item:item[0].value))
        return sorted_dict
        
    class _Guard:
        @staticmethod
        def check_axis_type(axis):
            if not isinstance(axis, Axis):
                raise ValueError('The axis parameter must be instance of the Axis class.')
            
        @staticmethod
        def check_value_type(value):
            if not isinstance(value, int):
                raise ValueError('The value parameter must be an integer.')
            
        @staticmethod
        def check_value_range(value):
            if value < -100 or value > 100:
                raise ValueError('The value parameter represents percentage and must be in the range -100 to 100')
            
class Serializer:
    
    def __init__(self):
        pass
    
    def serialize(self, command:Command) -> str:
        byte_array = bytearray([0])
        axes = command.get_axes()
        
        loop_count = 1
        for axis, value in axes.items():
            byte_array[0] |= 1 << axis.value - 1
            
            value_as_byte = value.to_bytes(1, signed=True)
            byte_array.extend(value_as_byte)
        
        return bytes(byte_array)
    
    def deserialize(self, serialized:str) -> Command:
        pass
    
    class _Guard:
        pass


# serialization map:
# byte 1 indictes which axes are being modified.
# byte1: 0, 1, 2, 3, 4, 5, 6, 7
#        w, x, y
# those axes are then represented by the following bytes in the w, x, y order.

