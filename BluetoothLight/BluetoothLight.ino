#include <SoftwareSerial.h>


SoftwareSerial Bluetooth(3, 4); //RX, TX
char val;
int lamp = 0;
int btn0 = 1;
int btn1 = 2;

void setup() { //setup

pinMode(lamp, OUTPUT);
pinMode(btn0, INPUT);
pinMode(btn1, INPUT);
pinMode(3, INPUT);
 
Bluetooth.begin(9600);
} //end setup

void loop() {

if(Bluetooth.available()){ //if
  val = Bluetooth.read();
}//end if

  if(val=='n'){
    digitalWrite(lamp, HIGH);
  }else if(val=='f'){
    digitalWrite(lamp, LOW);
  }

  
if(digitalRead(btn0)==HIGH){
  Bluetooth.flush();
  val = 'f';
} else if(digitalRead(btn1)==HIGH){
  Bluetooth.flush();
  val = 'n';
}

} //end loop

