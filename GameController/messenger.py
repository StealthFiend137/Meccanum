import paho.mqtt.client as mqtt

class Messenger:
    def __init__(self):
        self._topic = 'test/topic'
        pass
    
    def _on_connect(self, client, userdata, flags, result_code, properties):
        print(f'Connected with result code {result_code}')
        self._mqtt_client.subscribe(self._topic)

    def connect(self, broker_address, broker_port):
        try:
            self._mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
            self._mqtt_client.on_connect = self._on_connect
            self._mqtt_client.connect(broker_address, broker_port, 2)
            self._mqtt_client.loop_start()
        except Exception as e:
            print(f'Failed to connect: {e}')
            
    def send(self, payload):
        self._mqtt_client.publish(self._topic, payload)
