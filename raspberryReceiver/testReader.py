import time
import json
import sys  	##to read from pipe
from datetime import datetime
import tailhead
## http://www.switchdoc.com/2016/02/tutorial-installing-and-testing-mosquitto-mqtt-on-raspberry-pi/
## sensorData= open("sensorData.txt", r)		
for line in tailhead.follow_path("sensorData.txt"):
	if line:
		print("Data received" + line[19:29])
	else:
		time.sleep(0.05)

