# Home Weather Station

Arduino based DIY weather station which measures temperature, pressure and humidity in define time intervals and sends the data to Google Sheet. Later, the data can be read by an app on your computer or phone (in development).

## Weather station - main components
* Arduino Uno
* ESP-01 - Wi-Fi ESP8266 module
* BMP180
* DHT22 module
* Step-down power module or other source of 3.3 V to power ESP-01 (Arduino cannot handle it)

## Communication with ESP-01
* Connect ESP-01 and Arduino Uno as in the picture below to communicate with ESP-01 module through your Arduino board

[![Connection scheme](https://raw.githubusercontent.com/AlbertRtk/home_weather_station/master/sketch/sketch_esp_communication.jpg "Connection scheme")](https://raw.githubusercontent.com/AlbertRtk/home_weather_station/master/sketch/sketch_esp_communication.jpg "Connection scheme")

* Power ESP-01 and connect Arduino board via USB cable to your computer
* Run Arduino IDE and open Tools > Serial Monitor
* Choose "Both NL & CL" and Baud Rate 115200
* Type command `AT`. You should get `OK` response
* Change ESP-01 Baud Rate
```sh
AT+UART_DEF=9600,8,1,0,0
```
* Switch Serial Monitor to 9600 baud
* Set connection with your Wi-Fi. Connection will be saved in ESP8266 memory and set automatically after restart of the module
```sh
AT+CWMODE=3
AT+CWJAP="your wifi ssid","your wifi password"
```
* You can check if setting are saved by restarting the module. Connection to Wi-Fi should be established automatically now
```sh
AT+RST
```

## Weather station

[![Weather station scheme](https://raw.githubusercontent.com/AlbertRtk/home-weather-station/master/sketch/sketch_station.jpg "Weather station scheme")](https://raw.githubusercontent.com/AlbertRtk/home-weather-station/master/sketch/sketch_station.jpg "Weather station scheme")
