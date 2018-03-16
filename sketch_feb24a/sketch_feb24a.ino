void setup() {
  Serial.begin(9600);
  pinMode(4, OUTPUT);
}

void loop() {
char c;
if(Serial.available()){ //ha kap adatot a geptol
  c=Serial.read();
}
  if(c == 'n'){
    digitalWrite(4, HIGH);
    Serial.println("LED felkapcsolva");
  }else if(c == 'f'){
    digitalWrite(4, LOW);
    Serial.println("LED lekapcsolva");
  }
}
