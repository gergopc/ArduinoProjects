

/*
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

#define ADD_CARD_TASK 1
#define REMOVE_CARD_TASK 2
#define MAX_CARDS 10
#define CARD_EXISTS 0xFF
#define CARD_NOT_EXISTS 0x00

struct UID{
  byte b0;
  byte b1;
  byte b2;
  byte b3;
  byte er;
  };

int led = 13;
char r;
boolean adminmode;
byte task;
UID adminuid = {0xE6, 0x7F, 0xA5, 0x59};
constexpr uint8_t RST_PIN = 8;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 9;    // Configurable, see typical pin layout above
boolean door = false;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance


void setup() {
  //servo.attach(6);
  pinMode(led, OUTPUT);
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
	if(Serial.available())
  r = Serial.read();
   if(r=='a'){ //add rfid card command
    if(adminmode){
      task = ADD_CARD_TASK;
      Serial.println("Touch the card");
      Serial.println("setTSK" + task);
    }else Serial.println("Use the admin card");
   }
   if(r=='r'){ //remove rfid card command
    if(adminmode){
      task = REMOVE_CARD_TASK; 
      Serial.println("setTSK" + task);
      Serial.println("Touch the card");
    }else Serial.println("Use the admin card");
   }
     if(r=='l'){ //list eeprom values command
  for(int i=0; i<MAX_CARDS; i++){
   Serial.print("ADDR: ");
   Serial.print(i);
   Serial.print(" \n");
  for(int j=0; j<5; j++){
    Serial.println(EEPROM.read(i*5+j), HEX);
  }
  }
     }
     if(r=='c'){ //clear eeprom command
     for(int i=0; i<MAX_CARDS; i++){
  for(int j=0; j<5; j++){
    EEPROM.write(i*5+j, 0);
  }
  }
     }
 
	if(mfrc522.PICC_IsNewCardPresent()) {
  UID carduid = getID();
  if(carduid.er != 1){
    Serial.print("Card detected, UID: ");
    Serial.print(carduid.b0, HEX);
    Serial.print(carduid.b1, HEX);
    Serial.print(carduid.b2, HEX);
    Serial.println(carduid.b3, HEX);
    Serial.println(getCards(carduid));
    Serial.println("TASK" + task);
    if(task==ADD_CARD_TASK){
     addCard(carduid);
    }else if(task==REMOVE_CARD_TASK){
     removeCard(carduid);
    }
 if(getCards(carduid)==0){
  adminmode = false;
  Serial.println("NOT OK");
 }else if(getCards(carduid)==1){
  adminmode = false;
  Serial.println("OK");
  if(task==0) openDoor();
 }else if(getCards(carduid)==2){
  adminmode = !adminmode;
  if(task==0) openDoor();
  Serial.println("Admin card detected");
  }
  if(adminmode)Serial.println("Admin mode enabled");
    else Serial.println("Admin mode disabled");
task=0;
}
}
}
UID getID(){
    UID uid;
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    uid.er=1;
  }
  uid.b0 = mfrc522.uid.uidByte[0];
  uid.b1 = mfrc522.uid.uidByte[1];
  uid.b2 = mfrc522.uid.uidByte[2];
  uid.b3 = mfrc522.uid.uidByte[3];
  uid.er = 0;
  mfrc522.PICC_HaltA(); // Stop reading
  return uid;
}
void addCard(UID uid){
 if(!adminCheck(uid)){
  for(int i=0; i<MAX_CARDS; i++){
    if(EEPROM.read(i*5)==CARD_NOT_EXISTS){
      EEPROM.write(i*5, CARD_EXISTS);
      EEPROM.write(i*5+1, uid.b0);
      EEPROM.write(i*5+2, uid.b1);
      EEPROM.write(i*5+3, uid.b2);
      EEPROM.write(i*5+4, uid.b3);
      break;
    }
         Serial.println("Added");
  }
 }
}
void removeCard(UID uid){
  int check = 0;
for(int i; i<MAX_CARDS; i++){
  if(EEPROM.read(i*5)==CARD_EXISTS) check++;
  if(EEPROM.read(i*5+1)==uid.b0) check++;
  if(EEPROM.read(i*5+2)==uid.b1) check++;
  if(EEPROM.read(i*5+3)==uid.b2) check++;
  if(EEPROM.read(i*5+4)==uid.b3) check++; 
  if(check==5){ EEPROM.write(i*5, CARD_NOT_EXISTS);
  Serial.println("Removed");
  }
  }
}
void openDoor(){
  digitalWrite(led, !digitalRead(led));
}
int getCards(UID uid){
 int ret;
 int check = 0;
for(int i; i<MAX_CARDS; i++){
  if(EEPROM.read(i*5)==CARD_EXISTS) check++;
  if(EEPROM.read(i*5+1)==uid.b0) check++;
  if(EEPROM.read(i*5+2)==uid.b1) check++;
  if(EEPROM.read(i*5+3)==uid.b2) check++;
  if(EEPROM.read(i*5+4)==uid.b3) check++; 
  if(check==5) ret = 1;
  else ret = 0;
  }
  if(adminCheck(uid)) ret = 2;
 return ret;
}
boolean adminCheck(UID uid){
  int adcheck;
  if(uid.b0==adminuid.b0) adcheck++;
  if(uid.b1==adminuid.b1) adcheck++;
  if(uid.b2==adminuid.b2) adcheck++;
  if(uid.b3==adminuid.b3) adcheck++;
  if(adcheck==4) return true;
  else return false;
}

