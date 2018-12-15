/* Arduino Weather Station
 * 2018, Albert Ratajczak
 */

// TODO: wire ESP and Uno reset
// Error handling for DHT and BMP

#include <SoftwareSerial.h>
#include <Wire.h>
#include <SFE_BMP180.h>
#include "DHT.h"
#include "my_wifi.h"    // file with my WiFi SSID and passward

#define RX 2            // connected to ESP8266 TX pin
#define TX 3            // connected to ESP8266 RX pin
#define DHTTYPE DHT22   // DHT module number
#define DHTPIN 4        // pin for DHT22 signal


SoftwareSerial esp8266(RX, TX); 
DHT dht(DHTPIN, DHTTYPE);
SFE_BMP180 bmp180;

char state;
double temperature_bmp;
double pressure;
double temperature_dht;
double humidity;


void setup() {
  Serial.begin(9600);
  bmp180.begin();
  dht.begin();

  Serial.println("\n\n--- Setting ESP8266 ------------------------------");
  esp8266.begin(9600);
  while (!esp8266) {
    ;  // waiting for connection
  }
  
  Serial.print("\nESP8266: ");
  esp8266.println("AT+CWMODE=1");
  delay(100);
  Serial.println(readESP8266());
  
  Serial.print("\n\nESP8266: ");
  esp8266.println("AT+CWJAP=\""+WIFI_SSID+"\",\""+WIFI_PWD+"\"");
  delay(100);
  Serial.println(readESP8266());
  delay(5000);
  Serial.println(readESP8266());
  
  Serial.println("\n--------------------------------------------------");
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

  // Measuring humidity and temperature with DHT22
  humidity = dht.readHumidity();
  temperature_dht = dht.readTemperature();

  Serial.print("\nBMP180 temperature: ");
  Serial.println(temperature_bmp);  
  Serial.print("DHT22 temperature: ");
  Serial.println(temperature_dht);
  Serial.print("Averaged temperature: ");
  Serial.println(0.5*temperature_bmp+0.5*temperature_dht);
  Serial.print("Pressure: ");
  Serial.println(pressure);
  Serial.print("Humidity: ");
  Serial.println(humidity);

  delay(900000);
}


String readESP8266(){
  char inData[128]; 
  int index =0;
  while(esp8266.available()>0){
    if (index < 127) {
      inData[index] = esp8266.read();
      index++;
      inData[index] = '\0';  // Null terminates the string  
    }
  }
  String str(inData);
  str.trim();
  return str;
}
