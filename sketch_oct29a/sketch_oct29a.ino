char r;
struct ads{
  byte a;
  byte b;
  byte c;
  byte d;
  };
ads hgj(byte in){
  ads data;
  data.a=in;
  data.b=in+1;
  data.c=in+2;
  data.d=in+3;
  return data;
}
void setup() {
 Serial.begin(9600);

}

void loop() {
  ads a = hgj(2);
  if(Serial.available())r=Serial.read();
  if(r=='l'){
    Serial.println(a.a);
    Serial.println(a.b);
    Serial.println(a.c);
    Serial.println(a.d);
  }

}
