#include <EEPROM.h>
char r;
int ap=0;
const int a[15]={1,2,3,4,5,6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

void setup() {
Serial.begin(9600);
}

void loop() {
  if(Serial.available()) r = Serial.read();
 if(r=='l'){
  for(int i=0; i<10; i++){
   Serial.print("ADDR: ");
   Serial.print(i);
   Serial.print(' ');
   Serial.println(EEPROM.read(i), DEC);
  } 
 }
 if(r=='r'){
 for(int i=0; i<10; i++){
  if(EEPROM.read(i)==3) EEPROM.write(i, 0);
 }
 }
if(r=='a'){
  for(int i=0; i<10; i++){
    if(EEPROM.read(i)==0){
      EEPROM.write(i, a[ap]);
    ap++;
    }
  }
  }
  if(r=='c'){
    for (int i = 0 ; i < 10; i++) {
    EEPROM.write(i, 0);
  }

  }
}
