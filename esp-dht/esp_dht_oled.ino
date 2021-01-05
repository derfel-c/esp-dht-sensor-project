#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoOTA.h>


// Modifiable parameters:
#define DHTPIN 16
#define BUTTONPIN 0
#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128

// WIFI Connection
const char* ssid = "*";
const char* password = "*";
// API that handles the POST-Request
const String apiPath = "*";
// Sensor Location
const String location = "*";
// Frequency of sensor readings, display updates and POST-requests
const long updateTime = 3600000;

long lastMsg = 0;
float curTemp;
float curHum;
int airQuality;
DHTesp dht;

// -1 because the disply doesn't have a reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  dht.setup(DHTPIN, DHTesp::DHT22);
  pinMode(BUTTONPIN, INPUT);
  
  // Connect to Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Initialize Display with the I2C address 0x3C
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(200);
  display.clearDisplay();
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(5);
  
  // Arduino OTA Setup
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    ArduinoOTA.setHostname("esp8266-rMax-temp");

    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void postData() {
  HTTPClient http;
  String curTempStr = String(curTemp);
  String curHumStr = String(curHum);
  String data = "temp=" + curTempStr + "&hum=" + curHumStr + "&location=" + location;
  Serial.println("Sending HTTP-POST with current sensor readings!");
  http.begin(apiPath);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int responseCode = http.POST(data);
  Serial.println("HTTP-Responsecode: " + String(responseCode));
  http.end();
}

void displayData() {
  Serial.println("Display on");
  display.ssd1306_command(SSD1306_DISPLAYON);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("T: ");
  display.print(curTemp);
  display.println(" C");
  display.println("");
  display.print("H: ");
  display.print(curHum);
  display.print(" %");
  display.display();
}

void reconnectWifi() {
  int retry = 0;
  while(WiFi.status() != WL_CONNECTED && retry < 5) {
    retry++;
    Serial.println("Reconnecting Wifi...");
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    delay(1000);
  }
  if(retry >= 5) {
    ESP.restart();
  }
}
void loop() {
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi connection lost!");
    reconnectWifi();
  }
  ArduinoOTA.handle();
  int buttonState = digitalRead(BUTTONPIN);
  if(buttonState == LOW) {
    displayData();
  }
  else {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
 }
  long now = millis();
  if (lastMsg == 0 || now - lastMsg > updateTime) {
    curTemp = dht.getTemperature();
    curHum = dht.getHumidity();
    Serial.println("Measured Temperature: " + String(curTemp));
    Serial.println("Measured Humidity: " + String(curHum));
    postData();
    lastMsg = now;
  }
}
