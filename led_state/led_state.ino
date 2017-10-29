int state=0;

void setup() {
 pinMode(13, OUTPUT);
 pinMode(6, INPUT);

}

void loop() {
if(digitalRead(6)==HIGH){
  if(state==0){
    state=1;
  }else if(state==1){
    state=0;
  }
}
if(state==1){
digitalWrite(13, HIGH);
}else if(state==0){
  digitalWrite(13, LOW);
}
delay(200);
}

