void setup() {
 Serial.begin(9600);
analogReference(INTERNAL1V1);
}

void loop() {
  float temp = analogRead(A0)/9.31;
   Serial.print("Temperature: " /*+ temp*/);
   Serial.println(temp);
   Serial.print("Signal: "/* + analogRead(A0)*/);
   Serial.println(analogRead(A0));
   delay(500);
}
