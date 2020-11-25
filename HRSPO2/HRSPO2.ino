#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor;
uint32_t redBuffer;
uint32_t irBuffer;
int hrBuffer;
String dataToSend;
int sensorHR;
long milis;
void setup() {
  Serial.begin(9600); // initialize serial communication at 115200 bits per second:
  dataToSend.reserve(100);
  sensorHR=0;
  // Initialize sensor
  while (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30102 was not found. Please check wiring/power."));
  }
   byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
}

void loop() {
  while (particleSensor.available() == false) //do we have new data?
      particleSensor.check();
   redBuffer = particleSensor.getRed();
   irBuffer = particleSensor.getIR();
   hrBuffer = analogRead(sensorHR);
   milis=millis();
   dataToSend="HR:";
   dataToSend+=hrBuffer;
   dataToSend+=";ML:";
   dataToSend+=milis;
   dataToSend+=";RED:";
   dataToSend+=redBuffer;
   dataToSend+=";IR:";
   dataToSend+=irBuffer;
   Serial.println(dataToSend);
}
