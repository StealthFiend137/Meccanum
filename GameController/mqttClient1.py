import paho.mqtt.client as mqtt
import time
import command_serializer

topic = 'test/topic'

def on_connect(client, userdata, flags, result_code, properties):
    print(f'Connected with result code {result_code}')
    mqtt_client.subscribe(topic)
    
def on_disconnect(client, userdata, disconnect_flags, result_code, properties):
    print(f'Disconnected with result code: {result_code}')
    
def on_message(client, userdata, message):
    print(f'Received Message:\nTopic: {message.topic}\n Payload: {str(message.payload)}')
    
def on_publish(client, userdata, mid, reason_code, properties):
    pass
    #print(f'Message id: {mid} published. {reason_code}');
    
mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqtt_client.on_connect = on_connect
mqtt_client.on_disconnect = on_disconnect
mqtt_client.on_message = on_message
mqtt_client.on_publish = on_publish

broker_address = "192.168.0.50"
broker_port = 1883
try:
    mqtt_client.connect(broker_address, broker_port, 60)
except Exception as e:
    print(f'Failed to connect: {e}')

mqtt_client.loop_start()

command = command_serializer.Command()
serializer = command_serializer.Serializer()

try:
    count = 1
    while True:
        
        
        command.add_axis(command_serializer.Axis.x, 20)
        command.add_axis(command_serializer.Axis.w, 10)
        command.add_axis(command_serializer.Axis.y, 30)
        payload = serializer.serialize(command)
        
        
        mqtt_client.publish(topic, payload)
        count = count + 1
        time.sleep(1)
        pass
except KeyboardInterrupt:
    print("exiting")
    mqtt_client.loop_stop()
    mqtt_client.disconnect()