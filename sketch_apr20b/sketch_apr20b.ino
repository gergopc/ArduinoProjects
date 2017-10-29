char val;

void setup() {
 pinMode(13, OUTPUT);
Serial.begin(9600);
}

void loop() {
  if(Serial.available()){
    val = Serial.read();
  }
if(val=='n'){
  digitalWrite(13, HIGH);
}else{
digitalWrite(13, LOW);
}
}
