import paho.mqtt.client as mqtt
from command_serializer import Command, Serializer, Axis
import threading
import time

topic = 'test/topic'

class Cache:
    def __init__(self):
        self.w = 0
        self.x = 0
        self.y = 0
    
    def push_w_to_cache(self, value):
        self.w = value
        
    def push_x_to_cache(self, value):
        self.x = value
        
    def push_y_to_cache(self, value):
        self.y = value

class Chassis:
    
    CHASSIS_ADDRESS = 0x17;
    
    WDIR = 0x00
    XDIR = 0x01
    YDIR = 0X02

    def __init__(self):
        #self._bus = SMBus(1)
        self._running = False
        self._cache = Cache()
        
    def update(self, command):
        axes = command.get_axes()
        for axis, value in axes.items():
            if Axis.w == axis:
                self._cache.push_w_to_cache(value)
            elif Axis.x == axis:
                self._cache.push_x_to_cache(value)
            elif Axis.y == axis:
                self._cache.push_y_to_cache(value)
    
    def start(self):
        self._running = True
        self._loop_thread = threading.Thread(target=self._loop, daemon=True)
        self._loop_thread.start()
        
    def stop(self) -> None:
        self._running = False
        self._loop_thread.join()
        
    def _loop(self):
         while self._running:
             time.sleep(0.1)
             self._update_chassis(self._cache)
             
             
    def _update_chassis(self, cache):
        update = [cache.w, cache.x, cache.y]
        #self._bus.write_i2c_block_data(self.CHASSIS_ADDRESS, WDIR, update)
        print(f'{self.CHASSIS_ADDRESS}, {update}')
        

def on_connect(client, userdata, flags, result_code, properties):
    print(f'Connected with result code {result_code}')
    mqtt_client.subscribe(topic)

def on_message(client, userdata, message):
    command = Serializer.deserialize(message.payload)
    chassis.update(command)

mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

broker_address = "192.168.0.50"
broker_port = 1883
try:
    mqtt_client.connect(broker_address, broker_port, 60)
except Exception as e:
    print(f'Failed to connect: {e}')

chassis = Chassis()
chassis.start()
mqtt_client.loop_start()
