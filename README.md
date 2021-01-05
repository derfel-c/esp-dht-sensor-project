# ESP-DHT-Sensor-Project
## Introduction
The goal of this project is to create a temperature/humidity tracking system while learning and experimenting with different technologies. In the following list the subsytems and their respective goals are described:
- The goal of the hardware side is to create an ESP8266 and DHT22 based sensor system which sends the recorded temperature/humidity data to the backend and displays it on a small OLED display. 
- The backend should save the received sensor data to a MongoDB databse and offer a basic REST API so that the frontend can request the sensor data.
- The frontend should display the sensor data using graphs and/or tables

## Hardware
- ESP8266
- DHT22 sensor
- SSD1306 OLED display
- Pushbutton

## Technologies
### ESP
- Arduino C++
- Arduino OTA library
- Arduino ESP8266 libraries
- Adafruit SSD1308 libraries
- DHTesp library

### Backend
- node.js
- express
- body-parser express middleware
- cors express middleware
- mongoose

### Frontend
- ...