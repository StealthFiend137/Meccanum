import paho.mqtt.client as mqtt

topic = 'test/topic'

def on_connect(client, userdata, flags, result_code, properties):
    print(f'Connected with result code {result_code}')
    mqtt_client.subscribe(topic)

def on_message(client, userdata, message):
    print(f'Received Message:\nTopic: {message.topic}\n Payload: {str(message.payload)}')

mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

broker_address = "192.168.0.50"
broker_port = 1883
try:
    mqtt_client.connect(broker_address, broker_port, 60)
except Exception as e:
    print(f'Failed to connect: {e}')

mqtt_client.loop_start()