#include<Wire.h>
#include "DHT11.h"

#define moistureSensor 5
#define tempSensor 4
#define DHT11PIN 8   
#define lightSensor 3

dht11 DHT11;

void setup() {
  // Serial Begin so we can see the data from the mosture sensor in our serial input window. 
  Serial.begin(9600);
}

double Thermistor(int RawADC) {
  double temp;
  temp = log(((10240000/RawADC) - 10000)); 
  temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
  temp = temp - 273.15;           
  return temp;
}

void loop() {
  Serial.println("Sensor data");
  Serial.println("--------------------");
  
  // read the input on analog pin 0:
  int moistureValue = analogRead(moistureSensor);
  Serial.print("Moisture: ");
  Serial.println(moistureValue);

  int tempValue=analogRead(tempSensor);
  double temp =  Thermistor(tempValue);
  Serial.print("Temp. : ");
  Serial.print(temp); 
  Serial.println(" Celsius");

  int chk = DHT11.read(DHT11PIN);

  switch (chk) {
    case DHTLIB_OK: 
    Serial.print("Humidity (%): ");
    Serial.println((float)DHT11.humidity, 2);
    Serial.print("Temp. (oC): ");
    Serial.println((float)DHT11.temperature, 2);
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.println("Checksum error"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.println("Time out error"); 
    break;
    default: 
    Serial.println("Unknown error"); 
    break;
  }

  int lightValue = analogRead(lightSensor);
  Serial.print("Light: ");
  Serial.println(lightValue);
  
  delay(1000);        // delay 1 second between reads
}

