#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUDP.h>

#define un "gergopc"
#define pw "kecske123"

const char* ssid = "gergopc";
const char* password = "5.8.9.3.2";

ESP8266WebServer server(80);
WiFiUDP UDP;
IPAddress c_ip(192,168,1,255);

const int led = 13;
boolean state = false;
byte macaddr[] = {0xD0,0x50,0x99,0x07,0x05,0x28};

void initWOL(){
  String uname;
  String pwd;
  bool access=false;
  if(server.args()==2){
    if(server.argName(0)=="username")
      uname=server.arg(0);
     if(server.argName(1)=="password")
      pwd=server.arg(1);
  }
  if(uname==un && pwd==pw)
    access=true;
    if(access)
      sendWOL(c_ip, macaddr);
    else
      server.send(200, "text/plain", "Invalid Credentials!"); 
  //server.send(200, "text/plain", "u: " + uname + "\npw: "+ pwd);
}
void sendWOL(const IPAddress ip, const byte mac[]) {
  byte preamble[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  UDP.beginPacket(ip, 9);
  UDP.write(preamble, 6);
  for (int i = 0; i < 16; i++) {
    UDP.write(mac, 6);
  }
  UDP.endPacket();
  delay(100);
  server.send(200, "text/plain", "magic packet sent");
}
void handleRoot() {
 if(state)
 server.send(200, "text/plain", "status on");
 else
 server.send(200, "text/plain", "status off");
 
  digitalWrite(led, 0);
}
void turnON(){
  server.send(200, "text/plain", "ON");
    digitalWrite(D1, HIGH);
    state=true;
}
void turnOFF(){
  server.send(200, "text/plain", "OFF");
    digitalWrite(D1, LOW);
    state=false;
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
  server.on("/wol", initWOL);
  server.on("/status", [](){
    server.send(200, "text/plain", "this works as well");
  });
  
  server.on("/on", turnON);
  server.on("/off", turnOFF);
  server.onNotFound(handleNotFound);

  server.begin();
//  Serial.println("HTTP server started");
}

void loop(void){
  
  server.handleClient();
}
