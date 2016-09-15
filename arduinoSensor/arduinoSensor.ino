#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT11.h>
#include "printf.h"
#include "common/PlantData.h"

RF24 radio(7, 8);

const unsigned char rxAddr[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
unsigned long time;

// Device ID (HARDCODED FTW)
const unsigned short ID = 2;

const unsigned int moistureSensor = 0;
const unsigned int tempSensor = 1;
const unsigned int lightSensor = 2;
const unsigned int humiditySensor = 9;

dht11 dht11;
PlantData pd;

int soilMoisture(int sensorPin) {
  return analogRead(sensorPin);
}

double airTemperature(int sensorPin) {
  double temp;
  int rawADC = analogRead(sensorPin);
  temp = log(((10240000/rawADC) - 10000));
  temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
  temp = temp - 273.15;
  return temp;
}

float airHumidity(int sensorPin) {
  float values[3];
  int chk = dht11.read(sensorPin);
  values[0] = (float)chk;
  values[1] = (float)dht11.humidity;
  values[1] = (float)dht11.temperature;
  return dht11.humidity;
}

int light(int sensorPin) {
  return analogRead(sensorPin);
}

void printSensorValues(int soilMoisture, float airTemp, float humidityValue, int light) {
  Serial.println("Sensor data");
  Serial.println("-------------------------");
  Serial.print("Moisture: ");
  Serial.println(soilMoisture);
  Serial.print("Temp. : ");
  Serial.print(airTemp);
  Serial.println(" Celcius");
  Serial.print("Humidity (%): ");
  Serial.println(humidityValue);
  Serial.print("Light: ");
  Serial.println(light);
  Serial.println("-------------------------");
  Serial.println();
}

void setup()
{
  Serial.begin(57600);
  printf_begin();
  printf("Starting transmitter!\n");
  radio.begin();
  radio.setChannel(109);
  radio.setPALevel(palevel);

  radio.setDataRate(dataRate);
  //radio.setCRCLength(crcLength);
  //radio.write_register(DYNPD,0);

  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);

  radio.stopListening();

  uint8_t payloadSize = radio.getPayloadSize();
  printf("payload size: %d\n", payloadSize);
}

void loop()
{

  // Reading the sensor values
  int moistureValue = soilMoisture(moistureSensor);
  double tempValue =  airTemperature(tempSensor);
  // Humidity sensor provides humidity (values[0]) and temperature (values[1])
  float humidityValue = airHumidity(humiditySensor);
  int lightValue = light(lightSensor);

  printSensorValues(moistureValue, tempValue, humidityValue, lightValue);

  pd.id = ID;
  pd.soilHumidity = moistureValue;
  pd.airTemperature = tempValue;
  pd.airHumidity = humidityValue;
  pd.light = lightValue;

  radio.write(&pd, sizeof(pd));

  printf("Sent: %i bytes of data\n", sizeof(pd));

  delay(1000);
}
