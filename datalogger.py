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

sense = SenseHat()
Broker = "steffsopi"
pub_topic_temperature = "sensor/temperature"       # send messages to this topic
pub_topic_humidity = "sensor/humidity"
pub_topic_pressure = "sensor/pressure"

############### sensehat inputs ##################

def read_temp():
    t = sense.get_temperature()
    t = round(t)
    return t

def read_humidity():
    h = sense.get_humidity()
    h = round(h)
    return h

def read_pressure():
    p = sense.get_pressure()
    p = round(p)
    return p

def display_sensehat(message):
    sense.show_message(message)
    time.sleep(10)

############### MQTT section ##################

# when connecting to mqtt do this;

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

def on_publish(mosq, obj, mid):
    print("mid: " + str(mid))


client = mqtt.Client()
client.on_connect = on_connect
client.connect(Broker, 1883, 60)
client.loop_start()

while True:
    now = datetime.now()
    temp_data = {
                 'name' : 'temperature_sensor',
                 'value' : read_temp(),
                 'timestamp' : now.strftime("%Y-%m-%d %H:%M:%S.%f")
                 }
    json_str_temp = json.dumps(temp_data)
    
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


    ##sensor_data = [read_temp(), read_humidity(), read_pressure(), now.strftime("%Y-%m-%d %H:%M")]
    ##str(sensor_data)
    
    client.publish(pub_topic_temperature, json_str_temp)
    print("publishing:" + pub_topic_temperature + json_str_temp)
    client.publish(pub_topic_humidity, json_str_hum)
    print("publishing:" + pub_topic_humidity + json_str_hum)
    client.publish(pub_topic_pressure, json_str_press)
    print("publishing:" + pub_topic_pressure + json_str_press)
    
    time.sleep(1*10)
