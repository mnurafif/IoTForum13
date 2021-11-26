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

const char *ssid = "ZenZen";
const char *password = "12345678";
char path[] = "/";
char host[]= "172.20.10.3"; //WLAN IP (SERVER)
int port = 3000;




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






const char* source_key = "N0";
const char* destination_key = "N0";
int value = 4; //khusus N10

void get_json(){
  Serial.println("getting data from... ");
  HTTPClient http;  //Declare an object of class HTTPClient
     
  http.begin("http://172.20.10.3:3000/api/sensor_transactions");  //Specify request destination
  int httpCode = http.GET();                                                                  //Send the request
     
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
    const char* dest_key = doc["destination_key"];
    
    int value = doc["value"];//selain N10

    //DOING SOMETHING HERE
    if(String(dest_key) == String(source_key)){
        post_json(value);
        Serial.println("UPDATING DATA...");
        
        //khusus N8
        if(String(dest_key) == "N8"){//end point
            ///function LED
            Serial.println("END POINT REACHED");
        }
    }
  }else{
    Serial.println("CANT GET DATA");
  }
  
  http.end();
}







void post_json(int val){
    StaticJsonDocument<800> doc;   //Declaring static JSON buffer  
    doc["source_key"] = source_key;
    doc["value"] = val;
    doc["destination_key"] = destination_key;

    char data_json[800];
    serializeJson(doc, data_json);

    HTTPClient http;    //Declare object of class HTTPClient
   
    http.begin("http://172.20.10.3:3000/api/sensor_transaction");      //Specify request destination
    http.addHeader("Content-Type", "application/json");
    
    int httpCode = http.POST(data_json);   //Send the request
    String payload = http.getString();                  //Get the response payload
//    Serial.println(payload);
   
    http.end();  //Close connection
}

