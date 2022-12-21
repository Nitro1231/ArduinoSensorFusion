#include "Key.h"
#include "API.h"
#include <MPU9250.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>


unsigned int count = 0;
API api = API(API_SERVER_URL);


void setup() {
  Serial.begin(115200);

  WiFi.begin(WiFi_SSID, WiFi_PASSWORD);
  Serial.println("Connecting to the WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    String data = "{\"data\": \"test data from NodeMCU " + String(count) + "\"}";
    api.post("test", data);
    count++;
  }
  delay(500);
}