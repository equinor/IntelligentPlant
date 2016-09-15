/*
2015-04-06 : Johan Boeckx - Arduino/RPi(2) nRF24L01+ : Raspberry Pi (2) code
  Tested on Arduino UNO R3 and Raspberry Pi B Rev. 2.0 and Raspberry Pi 2 B

 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

 03/17/2013 : Charles-Henri Hallard (http://hallard.me)
              Modified to use with Arduipi board http://hallard.me/arduipi
                          Changed to use modified bcm2835 and RF24 library
TMRh20 2014 - Updated to work with optimized RF24 Arduino library

*/

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <RF24/RF24.h>
#include <time.h>
#include <fstream>
#include <algorithm>

using namespace std;
//
// Hardware configuration
// Configure the appropriate pins for your connections

/****************** Raspberry Pi ***********************/

// Radio CE Pin, CSN Pin, SPI Speed
// See http://www.airspayce.com/mikem/bcm2835/group__constants.html#ga63c029bd6500167152db4e57736d0939 and the related enumerations for pin information.

// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 4Mhz
//RF24 radio(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ);

// NEW: Setup for RPi B+
//RF24 radio(RPI_BPLUS_GPIO_J8_15,RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ);

// Setup for GPIO 15 CE and CE0 CSN with SPI Speed @ 8Mhz
//RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

// RPi generic:
RF24 radio(22,0);

/*** RPi Alternate ***/
//Note: Specify SPI BUS 0 or 1 instead of CS pin number.
// See http://tmrh20.github.io/RF24/RPi.html for more information on usage

//RPi Alternate, with MRAA
//RF24 radio(15,0);

//RPi Alternate, with SPIDEV - Note: Edit RF24/arch/BBB/spi.cpp and  set 'this->device = "/dev/spidev0.0";;' or as listed in /dev
//RF24 radio(22,0);


/****************** Linux (BBB,x86,etc) ***********************/

// See http://tmrh20.github.io/RF24/pages.html for more information on usage
// See http://iotdk.intel.com/docs/master/mraa/ for more information on MRAA
// See https://www.kernel.org/doc/Documentation/spi/spidev for more information on SPIDEV

// Setup for ARM(Linux) devices like BBB using spidev (default is "/dev/spidev1.0" )
//RF24 radio(115,0);

//BBB Alternate, with mraa
// CE pin = (Header P9, Pin 13) = 59 = 13 + 46
//Note: Specify SPI BUS 0 or 1 instead of CS pin number.
//RF24 radio(59,0);

/********** User Config *********/
// Assign a unique identifier for this node, 0 or 1
// 0 Rx / 1 Tx
bool radioNumber = 0;
unsigned long timeoutPeriod = 3000;     // Set a user-defined timeout period. With auto-retransmit set to (15,15) retransmission will take up to 60ms and as little as 7.5ms with it set to (1,15).
/********************************/

// Radio pipe addresses for the 2 nodes to communicate.
// const uint8_t pipes[][6] = {"1Node","2Node"};
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // Radio pipe addresses for the 2 nodes to communicate.
const unsigned char rxAdress[6] = {0x00,0x00,0x00,0x00,0x00,0x01};
char data[32] = {"_A message from RPi w/ NRF24L+!"};            //Data buffer


//variables for managing time
time_t rawtime;
struct tm * timeinfo;



void showData(void)
{
      printf("Data: ");
      for(int i=0; i<12; i++){
           printf("%c", data[i]);
      }
      printf("\n\r");
     //printf("Time: %ul\n", receivedTime);
}

struct PlantData {
	unsigned short id;
	unsigned short soilHumidity;
	float airTemperature;
	float airHumidity;
	unsigned short light;
};


int main(int argc, char** argv){

  const int role_rx=0, role_tx=1;
  int role=role_rx;

/***********************************/
  // Setup and configure rf radio
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  // Set the channel
  radio.setChannel(109);
  // Set the data rate
  //radio.setDataRate(RF24_2MBPS);
  radio.setDataRate(RF24_250KBPS);
  //radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MAX);
  //radio.setPALevel(RF24_PA_MIN);
  //radio.setPALevel(RF24_PA_HIGH);

  //opening pipe for reading
   radio.openReadingPipe(1,rxAdress);
   memset(&data,'\0',sizeof(data));
   radio.startListening();

    radio.printDetails();
    printf("Start loop:\n");
    PlantData pd;
    //file management
   ofstream sensorDataFile;
   sensorDataFile.open("sensorData.txt");
   std::string message;

    // forever loop
    while (1)
    {
        // just showing we are looping
	// printf("x");

	//
        // Receive each packet, dump it
        //
	
            if(radio.available()){
		// printf("Listening on radio\n");
                // Read any available payloads for analysis
                radio.read(&pd, sizeof(pd));
		if (pd.id > 0) {
		   time (&rawtime);
		   timeinfo = localtime(&rawtime);
		   fprintf(stdout, "Data: id %d SH %d T %f AH %f L %d received at %s", pd.id, pd.soilHumidity, pd.airTemperature, pd.airHumidity, pd.light, asctime(timeinfo));
		   char string[500];
                   char * timeInfo2 = asctime(timeinfo);
		   * remove(timeInfo2, timeInfo2+strlen(timeInfo2), '\n')='\0';

		   sprintf(string, "Data: id %d SH %d T %f AH %f L %d received at %s", pd.id, pd.soilHumidity, pd.airTemperature, pd.airHumidity, pd.light, timeInfo2);	
		   message = string;
		   sensorDataFile << message << endl;
		   //sensorDataFile.write(string);	
		   // printf("Written to file");
                   // Dump the printable data of the payload
                   //showData();
                   fflush(stdout);
		} //end of pd.id>0
            } 
        delay(5);
    } // forever loop
  
  sensorDataFile.close();
  return 0;
  
}

