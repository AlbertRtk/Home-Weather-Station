#include <Wire.h>
#include <SFE_BMP180.h>
#include "DHT.h"

#define DHTPIN 7 
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// BMP180 - pressure and temperature
SFE_BMP180 bmp180;
char state;

double temperature_bmp;
double pressure;
double humidity;
double temperature_dht;

void setup() {
  Serial.begin(9600);
  bmp180.begin();
  dht.begin();
}

void loop() {

  // Measureing temperature with BMP180
  bmp180.startTemperature();
  delay(500);
  bmp180.getTemperature(temperature_bmp);

  // Measuring pressure with BMP180
  state = bmp180.startPressure(3);
  delay(state);
  bmp180.getPressure(pressure, temperature_bmp);

  // Measuring humidity with DHT22
  humidity = dht.readHumidity();

  // Measuring temperature with DHT22
  temperature_dht = dht.readTemperature();

  Serial.println("--- BMP180 measurements ---------");
  Serial.print("Temperature: ");
  Serial.println(temperature_bmp);  
  Serial.print("Pressure: ");
  Serial.println(pressure);
  Serial.println("");
  Serial.println("--- DHT22 measurements ----------");
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Temperature: ");
  Serial.println(temperature_dht);
  Serial.println("");
  Serial.println("=================================");
  Serial.println("");
  
  delay(5000);
}
