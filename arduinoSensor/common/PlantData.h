
// Configuration for the NRF24L01+ radio link
#define palevel RF24_PA_MAX
#define dataRate RF24_250KBPS
#define crcLength RF24_CRC_8

// Data struts
struct PlantData {
  unsigned short id;
  unsigned int soilHumidity;
  float airTemperature;
  float airHumidity;
  unsigned int light;
};
