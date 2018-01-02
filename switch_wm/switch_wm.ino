
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "gergopc";
const char* password = "5.8.9.3.2";

ESP8266WebServer server(80);

const int led = 13;
int address = 0;
byte val = 0b00000000;
boolean state = false;

void handleRoot() {
 if(state)
 server.send(200, "text/plain", "status on");
 else
 server.send(200, "text/plain", "status off");
 
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
void start_switch(){
  digitalWrite(D1, LOW);
}
void kapcsol(){
  static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > 200) 
 {
  if(state)
  digitalWrite(D1, LOW);
  else
  digitalWrite(D1, HIGH);
  }
  state=!state;
 last_interrupt_time = interrupt_time;
}
void setup(void){
  pinMode(led, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D3, INPUT_PULLUP);
  attachInterrupt(D3, kapcsol, RISING);
  digitalWrite(led, 0);
//  Serial.begin(115200);
  WiFi.begin(ssid, password);
//  Serial.println("");


start_switch();
  // Wait for connection
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("");
//  Serial.print("Connected to ");
//  Serial.println(ssid);
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());
//
//  if (MDNS.begin("esp8266")) {
//    Serial.println("MDNS responder started");
//  }

  server.on("/", handleRoot);

  server.on("/status", [](){
    server.send(200, "text/plain", "this works as well");
  });
  
  server.on("/on", [](){
    server.send(200, "text/plain", "ON");
    digitalWrite(D1, HIGH);
    state=true;
  });

  server.on("/off", [](){
    server.send(200, "text/plain", "OFF");
    digitalWrite(D1, LOW);
    state=false;
  });
  server.onNotFound(handleNotFound);

  server.begin();
//  Serial.println("HTTP server started");
}

void loop(void){
  
  server.handleClient();
}
