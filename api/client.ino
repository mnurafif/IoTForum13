//http://arduino.esp8266.com/stable/package_esp8266com_index.json
//https://dl.espressif.com/dl/package_esp32_index.json
// ArduinoJSON can be downloaded through sketch

//////////////////// LIBRARIES /////////////////////
#include <Arduino.h>
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

//////////////////// GLOBAL DEFINE /////////////////////
WiFiClient client;

const char *ssid = "Bodo amat";
const char *password = "H2nomor11";
char path[] = "/";
char host[]= "https://esp-telebot.herokuapp.com"; //WLAN IP (SERVER)
int port = 80;




////////////////// LOOP AND SETUP ///////////////////////
void setup(){
    #if defined(ESP8266)
        Serial.begin(115200);
    #else
        Serial.begin(38400);
    #endif

    Serial.setDebugOutput(true);
    for (uint8_t t = 4; t > 0; t--)
    {
        Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
        Serial.flush();
        delay(500);
    }    
    wifi_connection();
}





void loop()
{
  get_json();
  
  delay(10000);
}




//////////////// BINDING CONNECTION /////////////////////
void wifi_connection()
{
    Serial.print("\n");
    Serial.printf("WiFi Connecting to :: ");
    Serial.printf(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.printf(".");
    }

    Serial.print("\n");
    Serial.printf("WiFi connected");
    Serial.printf("IP address: ");
    Serial.print(WiFi.localIP());

    app_connection();
}





void app_connection()
{
    Serial.print("\n");
    Serial.printf("App Connecting to :: ");
    Serial.printf(host);

    if (client.connect(host, port))
    {
        Serial.printf("\n\nApp Server Connected\n\n");
    }
    else
    {
        Serial.printf("\nApp Server Connection failed\n & Trying...");
        app_connection();
    }

    Serial.print("");
}




///////////////////////////////////////////////////////////////

void get_json(){
  Serial.println("getting data from... ");
  HTTPClient http;  //Declare an object of class HTTPClient
     
  http.begin("https://esp-telebot.herokuapp.com/api/sensor/123/65/78");  //Specify request destination
  int httpCode = http.GET(); //Send the request
     
  if (httpCode > 0) { //Check the returning code
    String payload = http.getString();   //Get the request response payload
    Serial.println(payload);//JSON DATA

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);

    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    const char* messgae = doc["messgae"];
    
    int sensor_1 = doc["sensor_1"];
    int sensor_2 = doc["sensor_2"];
    int sensor_3 = doc["sensor_3"];
    
    Serial.println(sensor_1);
    Serial.println(sensor_2);
    Serial.println(sensor_3);

  }else{
    Serial.println("CANT GET DATA");
  }
  
  http.end();
}
