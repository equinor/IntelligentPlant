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

#define MAGIC 0xBEEF

RF24 radio(22,0);
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // Radio pipe addresses for the 2 nodes to communicate.
const unsigned char rxAdress[6] = {0x00,0x00,0x00,0x00,0x00,0x01};
const unsigned char txAdress[6] = {0x00,0x00,0x00,0x00,0x00,0x33};
char data[32] = {"_A message from RPi w/ NRF24L+!"};            //Data buffer

//variables for managing time
time_t rawtime;
struct tm * timeinfo;

struct PlantData {
	unsigned short id;
	unsigned short soilHumidity;
	float airTemperature;
	float airHumidity;
	unsigned short light;
};

struct Cmd {
	unsigned short command;
	unsigned short magic;
	short value;
};


int main(int argc, char** argv){

  const int role_rx=0, role_tx=1;
  int role=role_rx;

/***********************************/
  // Setup and configure rf radio
  radio.begin();

  radio.setRetries(15,15);

  radio.setChannel(109);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);

  //opening pipe for reading
   radio.openWritingPipe(txAdress);
   radio.openReadingPipe(1,rxAdress);
   memset(&data,'\0',sizeof(data));
   radio.startListening();

    radio.printDetails();
    printf("Start loop:\n");
    PlantData pd;
    Cmd cmd;
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
		printf("Listening on radio\n");
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
            } else { //end of radio available
		radio.stopListening();
		cmd.command = 78;
		cmd.value = 12;
		cmd.magic = MAGIC;
		printf("Sending cmd: %d, value: %d to receivers...: \n", cmd.command, cmd.value);
		radio.write(&cmd, sizeof(cmd));
		printf("Done \n");
		radio.startListening();
		delay(1200);
	    }; //end of sending 
        delay(100);
    } // forever loop
  
  sensorDataFile.close();
  return 0;
  
}

