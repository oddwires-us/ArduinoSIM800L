#include <ArduinoJson.h>
#include <Http.h>

//#define serialSIM800 Serial2

unsigned long lastRunTime = 0;
unsigned long waitForRunTime = 0;

#define RST_PIN 33
HTTP http(9600, RST_PIN, true); // in debug mode

const char header[] = "Authorization: bearer S0I0QzgyMEM1NjpzY3JpcHRyOkQxMTQwNjBERUQwN0FEREQ5NTBGOTk0MEU2ODNCQTk3";
const char contentType[] = "application/x-www-form-urlencoded";

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Starting!");
  doPost();
}

// the loop routine runs over and over again forever:
void loop() {
  
}

void print(const __FlashStringHelper *message, int code = -1){
  if (code != -1){
    Serial.print(message);
    Serial.println(code);
  }
  else {
    Serial.println(message);
  }
}

void doPost(){
  http.powerDownNow();
  http.preInit();
  char response[2048];
  char body[] = "myName=ian";
  Result result;

  result = http.disconnect(); // will get error on connect if currently connected
  print(F("Configure bearer: "), http.configureBearer("pwg"));
  result = http.connect();
  print(F("HTTP connect: "), result);

  http.setContentType(contentType);
  http.setHeader(header);

  result = http.post("https://api.scriptrapps.io/HelloDevice", body, response);
  print(F("HTTP POST: "), result);
  if (result == SUCCESS) {
    Serial.println(response);
    StaticJsonBuffer<1024> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response);
    lastRunTime = millis();
    waitForRunTime = root["waitForRunTime"];
    
    print(F("Last run time: "), lastRunTime);
    print(F("Next post in: "), waitForRunTime);
  }
  
  print(F("HTTP disconnect: "), http.disconnect());
}
