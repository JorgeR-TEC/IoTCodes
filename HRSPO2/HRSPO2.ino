#include <Wire.h>
#include <SoftwareSerial.h>//wifi
#include "MAX30105.h"

SoftwareSerial ESPserial(2, 3); //Wifi RX | TX
MAX30105 particleSensor;
char data[100];
uint32_t redBuffer;
uint32_t irBuffer;
int hrBuffer;
String dataToSend;
int sensorHR;
long milis;
String command;
String response;
void setup() {
  Serial.begin(9600); // initialize serial communication at 115200 bits per second:
  dataToSend.reserve(100);
  command.reserve(300);
  response.reserve(200);
  sensorHR=0;
  // Initialize sensor
  while (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30102 was not found. Please check wiring/power."));
  }
  //iniciar wifi
  ESPserial.begin(9600);
  Serial.println("Setting up client mode");
  sendCommand("AT+CWMODE=1\r\n", 1000); 
  sendCommand2("AT+CWJAP=\"Kepler\",\"TychoGabePennyHorse\"\r\n", 4000,1); 
  delay(20000);
  sendCommand2("AT+CIPSTART=\"UDP\",\"172.16.38.42\",1337\r\n", 2000, 1);
  delay(1000);
  //iniciar SPO2
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
  
   //enviar por serial
   //Serial.println(dataToSend);

   //enviar por wifi
    dataToSend+=";\r\n";
    command="AT+CIPSEND=";
    command+=dataToSend.length();
    command+="\r\n";
    sendCommand(command, 50);
    sendData(50);
   
}

void sendData(const int timeout)
{
  while ( ESPserial.available() ) {
    Serial.write( ESPserial.read());
    delay(1000);
  }
  response = "";
  int dataSize = dataToSend.length();
  dataToSend.toCharArray(data,dataSize);
  ESPserial.write(data,dataSize); // 
  long int time = millis();
  while( (time+timeout) > millis())
  {
  while(ESPserial.available())
  {
  // The esp has data so display its output to the serial window
  char c = ESPserial.read(); // read the next character.
  response+=c;
  }
  }
  Serial.print(response);
  //return response;
}

void sendCommand(String command, const int timeout)
{
  while ( ESPserial.available() ) {
    Serial.write( ESPserial.read());
    delay(100);
  }
  response="";
  ESPserial.print(command); // send the read character to the wifi
  long int time = millis();
  while( (time+timeout) > millis())
  {
  while(ESPserial.available())
  {
  // The esp has data so display its output to the serial window
  char c = ESPserial.read(); // read the next character.
  response+=c;
  }
  }
  Serial.print(response);
  //return response;
} 
void sendCommand2(String command, const int timeout, int times)
{
  response="";
  ESPserial.print(command); // send the read character to the wifi
  int i=0;
  while(i<times){
    long int time = millis();
    while( (time+timeout) > millis())
    {
    while(ESPserial.available())
    {
    // The esp has data so display its output to the serial window
    char c = ESPserial.read(); // read the next character.
    response+=c;
    }
    }
    Serial.print(response);
    i+=1;
  }

  //return response;
} 
