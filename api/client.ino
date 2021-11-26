/**
   SETUP BOARD   -> https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/
   SETUP DRIVER  -> https://electronics-project-hub.com/arduino-not-detected-and-driver-issues-solved/
                 -> https://learn.sparkfun.com/tutorials/usb-serial-driver-quick-install-/all
   ARDUINOJSON   -> https://arduinojson.org/
*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecureBearSSL.h>

// Fingerprint for demo URL, expires on June 2, 2021, needs to be updated well before this date
const uint8_t fingerprint[20] = {0x40, 0xaf, 0x00, 0x6b, 0xec, 0x90, 0x22, 0x41, 0x8e, 0xa3, 0xad, 0xfa, 0x1a, 0xe8, 0x25, 0x41, 0x1d, 0x1a, 0x54, 0xb3};

ESP8266WiFiMulti WiFiMulti;

const char *ssid = "Bodo amat";
const char *password = "H2nomor11";

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  wifi_connection();  
}

//////////////// BINDING CONNECTION /////////////////////
void wifi_connection()
{
  Serial.print("\n");
  Serial.printf("WiFi Connecting to :: ");
  Serial.printf(ssid);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  if ((WiFiMulti.run() == WL_CONNECTED)) {
    Serial.printf("[WIFI] Connected...\n");
    app_connection();
  }else{
    Serial.printf("[WIFI] Unable to Connect...\n");
  }
}

void app_connection(){
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;  
  
  Serial.print("[HTTPS] begin...\n");
  
  int sensor1 = random(50, 80);
  int sensor2 = random(200, 300);
  int sensor3 = random(30, 50);
  String(url) = "https://esp-telebot.herokuapp.com/api/sensor/"+String(sensor1)+"/"+String(sensor2)+"/"+String(sensor3);

  if (https.begin(*client, url)) {  // HTTPS
      Serial.print("[HTTPS] Getting Data -> "+url+"\n");
      
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();

          Serial.println("[HTTPS] RESPONSE ... ");
          Serial.println(payload);
          Serial.println("");

          StaticJsonDocument<300> doc;
          DeserializationError error = deserializeJson(doc, payload);

          // Test if parsing succeeds.
          if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            return;
          }
          
          const char *message = doc["message"];
          int status_ = doc["status"];
          int sensor_1 = doc["data"]["sensor_1"];
          int sensor_2 = doc["data"]["sensor_2"];
          int sensor_3 = doc["data"]["sensor_3"];

          get_parsed_data(sensor_1, sensor_2, sensor_3);       
          
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
}


void get_parsed_data(int sensor_1, int sensor_2, int sensor_3){  
   Serial.println("[HTTPS] PARSING....");        
   Serial.print("sensor_1:: ");
   Serial.println(sensor_1);
   Serial.print("sensor_2:: ");
   Serial.println(sensor_2);
   Serial.print("sensor_3:: ");
   Serial.println(sensor_3);
}

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    app_connection();
  }else{
    Serial.printf("[WIFI] Unable to Connect...\n");
  }
  Serial.println("Wait 10s before next round...");
  delay(10000);
}
