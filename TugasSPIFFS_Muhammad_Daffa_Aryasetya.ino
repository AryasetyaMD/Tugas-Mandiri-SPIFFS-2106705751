#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

const char* ssid = "Embedded Wifi";
const char* password = "pingpooo5";

const int ledPin = 3;
String ledState;

AsyncWebServer server(80);

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  return String();
}

void setup(){
  Serial.begin(115200);
  Serial.println("Muhammad Daffa Aryasetya - 2106705751");
  Serial.println("Tugas Web Server SPIFFS");
  pinMode(ledPin, OUTPUT);

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // ESP32 yang saya miliki memiliki state LED yang terbalik (normally closed / menyala), sehingga string ON perlu melakukan write LOW
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // ESP32 yang saya miliki memiliki state LED yang terbalik (normally closed / menyala), sehingga string OFF perlu melakukan write HIGH
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.begin();
}

void loop(){
}
