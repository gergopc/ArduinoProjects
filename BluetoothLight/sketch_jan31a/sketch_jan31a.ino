int dt = 11;
int clk = 12;
char val;

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("initialized");
}

void loop() {
 if(Serial.available()){
  val = Serial.read();
 }
if(val == 'a'){
  shiftOut(dt, clk,MSBFIRST, 0b00000001);
  Serial.println("received a");
}else if (val=='s'){
  shiftOut(dt, clk,MSBFIRST, 0b00000010);
  Serial.println("received s");
}else if (val=='d'){
  shiftOut(dt, clk,MSBFIRST, 0b00000100);
  Serial.println("received d");
}
}
