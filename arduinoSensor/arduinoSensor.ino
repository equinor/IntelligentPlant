#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include "common/PlantData.h"

RF24 radio(7, 8);

const unsigned char rxAddr[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
unsigned long time;

// Device ID (HARDCODED FTW)
unsigned short ID = 1;

PlantData pd;

void setup()
{
  Serial.begin(57600);
  printf_begin();
  printf("Starting transmitter!\n");
  
  radio.begin();
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
  pd.id = ID;
  pd.soilHumidity = 11;
  pd.airTemperature = 3.14f;
  pd.airHumidity = 0.6f;
  pd.light = 66;

  /*printf("pd.id -> %i\n", sizeof(pd.id));
  printf("pd.solidHumidity -> %i\n", sizeof(pd.soilHumidity));
  printf("pd.airTemperature -> %i\n", sizeof(pd.airTemperature));
  printf("pd.airHumidity -> %i\n", sizeof(pd.airHumidity));
  printf("pd.light -> %i\n", sizeof(pd.light));*/
  
  //const char text[] = "Hello World";
  radio.write(&pd, sizeof(pd));
  //radio.write(&time, sizeof(time));
  //printf("Sent: %s, with time; %ul\n", text, time);
  printf("Sent: %i bytes of data\n", sizeof(pd));
  
  delay(1000);
}
