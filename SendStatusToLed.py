'''
Created on 14. sep. 2016

@author: STEFFSO
'''
from sense_hat import SenseHat
import paho.mqtt.client as mqtt
import paho.mqtt.subscribe as subscribe

sense = SenseHat()
Broker = "10.12.73.64"

def on_connect(client, userdata, flags, rc):
    client.subscribe('$SYS/broker/clients/active')
    print("Connected with result code "+str(rc))
    
def on_message(mosq, obj, msg):
 print "Topic: " + str(msg.topic)
 print "QoS: " + str(msg.qos)
 print "Payload: " + str(msg.payload)
 sense.show_letter(msg.payload,text_colour=[255, 0, 0])
    
    
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(Broker, 1883, 60)
client.loop_forever()