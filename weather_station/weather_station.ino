/* Arduino Weather Station
 * 2019, Albert Ratajczak
 */

// Print ESP8266 response for debuging
const boolean DEBUG = true;

#include <SoftwareSerial.h>
#include <Wire.h>
#include <SFE_BMP180.h>
#include "DHT.h"
#include "secret.h"    // file with my WiFi SSID and passward and Google Form ID

#define RX 2            // connected to ESP8266 TX pin
#define TX 3            // connected to ESP8266 RX pin
#define DHTTYPE DHT22   // DHT module number
#define DHTPIN 4        // pin for DHT22 signal

// Setting ESP8266, DHN22 and BMP180
SoftwareSerial esp(RX, TX); 
DHT dht(DHTPIN, DHTTYPE);
SFE_BMP180 bmp180;

// Host abd port number for SSL connection
const String HOST = "docs.google.com";
const int PORT = 443;

// Maximal awaiting time for response from ESP8266
int rtime = 5000;  

// Variables for measurements of weather conditions
char state;
double temperature_bmp;
double pressure;
double temperature_dht;
double humidity;


void setup() {
  Serial.begin(9600);
  esp.begin(9600);
  while (!esp) {
    ;  // waiting for connection
  }
  sendToESP("AT+RST", rtime, "GOT IP");  // WiFi is saved in ESP8266
  bmp180.begin();
  dht.begin();
  delay(1000);
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

  // Creating string with request to send to ESP8266
  String request = "GET /forms/d/e/" + FORM_ID + "/formResponse?" + 
                   "entry.1105778885=" + String(temperature_bmp) +
                   "&entry.1094475896=" + String(temperature_dht) + 
                   "&entry.896020747=" + String(pressure) +
                   "&entry.1330320364=" + String(humidity) +
                   "&submit=Submit HTTP/1.1\r\n" +
                   "Host: " + HOST + "\r\n" +
                   "User-Agent: ESP8266\r\n" +
                   "Connection: close\r\n\r\n";

  // Setting SSL Buffer size - has to be large, 2048-4096
  sendToESP("AT+CIPSSLSIZE=4096", rtime, "OK");
  
  // Starting connection
  sendToESP("AT+CIPSTART=\"SSL\",\"" + HOST + "\"," + String(PORT), rtime, "OK");
  
  // Sending data to ESP8266 - setting the data length and passing request
  sendToESP("AT+CIPSEND="+String(request.length()+2), rtime, "OK");
  sendToESP(request, rtime, "CLOSED");

  delay(900000);  // 15 min
}


boolean find(String string, String pattern){
  /* Checks if string contains pattern
   * Return: boolean, true or false
  */
  if(string.indexOf(pattern) >= 0)
    return true;
  return false;
}


String sendToESP(String command, const int timeout, String answer){
  /* Sends command to ESP8266 
   * Waits for timeout ms for response containing answer
   * Returns: String, response from ESP8266
  */
  if(DEBUG){
    Serial.println("\n>> ESP8266 request:");
    Serial.println(command);
    Serial.println();
  }
  esp.println(command);
  return readESP(timeout, answer);
}


String readESP(const int timeout, String tail){
  /* Reads response from ESP8266
   * Waits for response for timeout ms
   * Reads untim tail is found in the response
   * Return: String, response from ESP8266
  */
  char c;
  String message; 
  long int t = millis();
  while((t+timeout)>=millis()){
    while(esp.available()){  // ESP8266 sends data
      c = esp.read();        // read byte
      message += c;          // compose response  
      if(DEBUG){
        Serial.print(c);
      }
    }
    if(find(message, tail)){
      break;  // break while-loop if message contain tail
    }
  }
  return message;
}
