char r;
void setup() {
  Serial.begin(9600);

}

void loop() {
  if(Serial.available())
  if(Serial.read() != null) r=Serial.read();
  Serial.println(r);
  delay(500);
}
