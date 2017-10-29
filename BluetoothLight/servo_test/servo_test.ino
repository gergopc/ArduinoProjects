#include <Servo.h>

Servo servo;

void setup() {
  Serial.begin(9600);
  servo.attach(9);

}

void loop() {
 char val;
 if(Serial.available()){
  val=Serial.read();
 }
 if(val == 'b') servo.write(180);
 if(val == 'k') servo.write(0);

}
