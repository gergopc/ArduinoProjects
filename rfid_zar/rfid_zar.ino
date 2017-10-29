

/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 * 
 * @license Released into the public domain.
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

char r;
boolean adminmode;
int task;
int adminuid = 4294944089;
constexpr uint8_t RST_PIN = 8;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 9;    // Configurable, see typical pin layout above
boolean door = false;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
//Servo servo;

void setup() {
  //servo.attach(6);
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
   if(r=='a'){
    if(adminmode){
      task = ADD_CARD_TASK;
      Serial.println("Touch the card");
    }else Serial.println("Use the admin card");
   }
   if(r=='r'){
    if(adminmode){
      task = REMOVE_CARD_TASK; 
      Serial.println("Touch the card");
    }else Serial.println("Use the admin card");
   }
	if(mfrc522.PICC_IsNewCardPresent()) {
  unsigned long uid = getID();
  if(uid != -1){
    Serial.print("Card detected, UID: "); Serial.println(uid);
    Serial.println(getCards(uid));
    
    if(task==ADD_CARD_TASK){
      task = 0;
      Serial.println("Added");
      addCard(uid);
    }else if(task==REMOVE_CARD_TASK){
      task = 0;
      Serial.println("Removed");
    }
 if(getCards(uid)==0){
  adminmode = false;
  Serial.println("NOT OK");
 }else if(getCards(uid)==1){
  adminmode = false;
  Serial.println("OK");
 }else if(getCards(uid)==2){
  adminmode = !adminmode;
  Serial.println("Admin card detected");
  }
  if(adminmode)Serial.println("Admin mode enabled");
    else Serial.println("Admin mode disabled");
}
}
}
void addCard(int uid){
  if(uid != adminuid){
  int addr = EEPROM.length() + 1;
  EEPROM.write(addr, uid);
  }
}
void removeCard(int uid){
 
}
void openDoor(){
  
}
int getCards(int uid){
  int ret;
  for (int i=0; i<EEPROM.length(); i++){
    if(uid == EEPROM.read(i)) ret=1;
    else ret=0;
  }
  if(uid==adminuid){
    ret=2;
  }
  return ret;
}
unsigned long getID(){
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return -1;
  }
  unsigned long hex_num;
  hex_num =  mfrc522.uid.uidByte[0] << 24;
  hex_num += mfrc522.uid.uidByte[1] << 16;
  hex_num += mfrc522.uid.uidByte[2] <<  8;
  hex_num += mfrc522.uid.uidByte[3];
  mfrc522.PICC_HaltA(); // Stop reading
  return hex_num;
}
