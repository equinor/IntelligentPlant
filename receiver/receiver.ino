#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"

// RECEIVER

RF24 radio(7, 8);

//const byte rxAddr[6] = "00001";
const unsigned char rxAddr[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
unsigned long time;

void setup()
{

  Serial.begin(57600);
  printf_begin();
  printf("Starting receiver!\n");
  radio.setChannel(109);
  radio.begin();
  printf("radio begin\n");
  radio.openReadingPipe(0, rxAddr);
  printf("radio open pipe\n");

  radio.startListening();
  printf("radio start listening\n");
}

void loop()
{
  if (radio.available())
  {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    //delay(50);
    //radio.read(&time, sizeof(time));

    if(text[0] != 0) {
      printf(text);
      //printf(" - ");
      //printf("%ul", time);
      printf("\n");
    } else {
      printf("something rubbish..\n");
    }
  }
}
