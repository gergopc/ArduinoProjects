#include <SoftwareSerial.h>

SoftwareSerial Bluetooth(3, 4);
int val;   //data value buffer

int lamp = 5; //lamp pin

int btn1 = 0;
int btn2 = 0;

void setup() {

pinMode(lamp, OUTPUT);  
Bluetooth.begin(9600);

}

void loop() {
  if(Bluetooth.available()){
    val = Bluetooth.read();
  }

if(val == 1){
  digitalWrite(lamp, HIGH);  //felgyujt
}else if(val == 2){
  digitalWrite(lamp, LOW); //leolt
}


}
