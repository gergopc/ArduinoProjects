#include <IRremote.h>

int btn = 2;

void setup() {
 pinMode(btn, INPUT);

}
IRsend is;
void loop() {
 if(digitalRead(btn)==HIGH){
  is.sendShuzu(0x46546545, 32);
 }

}
