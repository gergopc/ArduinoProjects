void setup() {
 Serial.begin(9600);

}

void loop() {

  Serial.print("Enter a mobile number: ");
  char remoteNum[20];  // telephone number to send sms
  readSerial(remoteNum);
  if(remoteNum > 15){
    Serial.println("Invalid number!");
     remoteNum = 0;
     
  }else{
  Serial.println(remoteNum);
  }
}
int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (inChar == '\n') {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}
