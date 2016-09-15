'''
Created on 28. aug. 2016

@author: ssorenes
'''
from sense_hat import SenseHat
import time
import json
from datetime import datetime
import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import tailhead


Broker = "192.168.1.100"
pub_topic_soiltemperature = "plant/01/sensor/soiltemperature"       # send messages to this topic 
pub_topic_soilhumidity = "plant/01/sensor/soilhumidity"
pub_topic_atmpressure = "plant/01/sensor/atmosphericpressure"
pub_topic_airtemperature = "plant/01/sensor/airtemperature"

# when connecting to mqtt do this;

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

def on_publish(mosq, obj, mid):
    print("mid: " + str(mid))


client = mqtt.Client()
client.on_connect = on_connect
##client.connect(Broker, 1883, 60)
client.connect(Broker)
client.loop_start()

for line in tailhead.follow_path("sensorData.txt"):
	now = datetime.now()
	if line:
		print("Data received" + line)
		temp_data = {
                	'name' : 'air_temperature_sensor',
                	'value' : line[19:29],
                	'timestamp' : now.strftime("%Y-%m-%d %H:%M:%S.%f")
                }
    		json_str_temp = json.dumps(temp_data)
		client.publish(pub_topic_airtemperature, json_str_temp)
    		print("publishing:" + pub_topic_airtemperature + json_str_temp)
		time.sleep(1*10)
	else:
		time.sleep(0.05)
  
  
"""  
    hum_data = {
                 'name' : 'humidity_sensor',
                 'value' : read_humidity(),
                 'timestamp' : now.strftime("%Y-%m-%d %H:%M:%S.%f")
                 }
    json_str_hum = json.dumps(hum_data)

    press_data = {
                 'name' : 'pressure_sensor',
                 'value' : read_pressure(),
                 'timestamp' : now.strftime("%Y-%m-%d %H:%M:%S.%f")
                 }
    json_str_press = json.dumps(press_data)
"""

    ##sensor_data = [read_temp(), read_humidity(), read_pressure(), now.strftime("%Y-%m-%d %H:%M")]
    ##str(sensor_data)
    
 ##   client.publish(pub_topic_temperature, json_str_temp)
 ##   print("publishing:" + pub_topic_temperature + json_str_temp)
##    client.publish(pub_topic_humidity, json_str_hum)
 ##   print("publishing:" + pub_topic_humidity + json_str_hum)
 ##   client.publish(pub_topic_pressure, json_str_press)
 ##   print("publishing:" + pub_topic_pressure + json_str_press)
    
    
