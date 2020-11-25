
long HR;
void setup() {
  Serial.begin(9600);

}

void loop() {
 HR=analogRead(0);
 Serial.println(HR);

}
