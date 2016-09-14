#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"

// TRANSMITTER

RF24 radio(7, 8);

#define palevel RF24_PA_MAX

#define dataRate RF24_250KBPS

#define crcLength RF24_CRC_8

//const byte rxAddr[6] = "00001";
const unsigned char rxAddr[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
unsigned long time;

//struct {
//};

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
}

void loop()
{
  //time = millis();
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  //radio.write(&time, sizeof(time));
  //printf("Sent: %s, with time; %ul\n", text, time);
  printf("Sent: %s\n", text);
  
  delay(1000);
}
