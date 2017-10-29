int led= 13;
int interruptPin = 2;
volatile byte state = LOW;

void setup() {
 pinMode(interruptPin, INPUT);
 pinMode(led, OUTPUT);
 attachInterrupt(digitalPinToInterrupt(2), toggle, FALLING);

}

void toggle(){
  state = !state;
}

void loop() {
 digitalWrite(led, state);
delay(100);
}
