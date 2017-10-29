

char val;


void setup() {
for(int i=4; i<8; i++){
  pinMode(i, OUTPUT);
}
Serial.begin(9600);
}

void loop() {
  
  if(Serial.available()){
    val=Serial.read();
  }

if(val=='j'){
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
}else if(val=='k'){
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
}else if(val=='l'){
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
}else if(val=='h'){
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
}else{
 for(int i=4; i<8; i++){
  digitalWrite(i, LOW);
} 
}
if(!val == 0){
  Serial.println(val);
}
delay(500);

}
