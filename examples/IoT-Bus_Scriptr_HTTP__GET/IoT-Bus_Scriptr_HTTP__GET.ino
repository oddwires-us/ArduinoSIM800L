/****************************************************/
/* This example shows how to perform a scriptr GET  */
/*                                                  */
/* Using IoT-BUs SIM800 GSM module by oddWires      */
/****************************************************/

#include <Http.h>

unsigned long lastRunTime = 0;
unsigned long waitForRunTime = 0;

#define RST_PIN 33
HTTP http(9600, RST_PIN);

const char contentType[] = "text/plain";
const char header[] = "Authorization: bearer S0I0QzgyMEM1NjpzY3JpcHRyOkQxMTQwNjBERUQwN0FEREQ5NTBGOTk0MEU2ODNCQTk3";

// the setup routine runs once when you press reset:
void setup() {
  
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Starting!");
  doGet();
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

void doGet(){
  http.preInit();
  char response[1024];
  Result result;

  result = http.disconnect(); // may get error on connect if currently connected
  print(F("Configure bearer: "), http.configureBearer("pwg"));
  result = http.connect();
  print(F("HTTP connect: "), result);

  http.setContentType(contentType);
  http.setHeader(header);

  result = http.get("https://api.scriptrapps.io/test", response);
  print(F("HTTP GET: "), result);
  if (result == SUCCESS) {
    Serial.println(response);
  }
  
  print(F("HTTP disconnect: "), http.disconnect());
}
