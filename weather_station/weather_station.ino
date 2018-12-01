#include <LiquidCrystal.h>
#include <Wire.h>
#include <SFE_BMP180.h>

#define LM35 A0

//LCD interface pin
const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal LCD(RS, EN, D4, D5, D6, D7);

// BMP180 - pressure and temperature
SFE_BMP180 BMP180;
char state;

float temperature_in;
double temperature_out;
double pressure;

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
  LCD.begin(16, 2);
  BMP180.begin();
}

void loop() {
  
  // Measuring tmepreature inside with LM35
  temperature_in = 0;
  for (int i=0; i<10; i++) {
    temperature_in += analogRead(LM35) / 9.31 / 10;
    delay(100);
  }
  Serial.println(temperature_in);

  // Measureing temperature outside
  BMP180.startTemperature();
  delay(500);
  BMP180.getTemperature(temperature_out);
  Serial.println(temperature_out);

  // Measuring pressure
  state = BMP180.startPressure(3);
  delay(state);
  BMP180.getPressure(pressure, temperature_out);
  Serial.println(pressure);
  
  // LCD - temperature inside
  LCD.setCursor(0, 0);
  LCD.print(" in ");
  LCD.setCursor(4, 0);
  LCD.print(temperature_in, 0);
  LCD.setCursor(6, 0);
  LCD.print((char)223);
  LCD.print("C");

  // LCD - status
  LCD.setCursor(9, 0);
  LCD.print("     OK");

  // LCD temperature outside
  LCD.setCursor(0, 1);
  LCD.print("out ");
  LCD.setCursor(4, 1);
  LCD.print(temperature_out, 0);
  LCD.setCursor(6, 1);
  LCD.print((char)223);
  LCD.print("C");

  // LCD - pressure
  LCD.setCursor(9, 1);
  LCD.print(pressure, 0);
  LCD.setCursor(13, 1);
  LCD.print("hPa");
  
  delay(1000);
}
