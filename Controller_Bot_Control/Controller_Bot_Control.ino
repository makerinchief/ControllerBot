/*
  Controller Bot Control
  Justin C Kirk 2020

  info@makerinchief.com
  https://www.makerinchief.com/
  https://github.com/makerinchief/ControllerBot

  Controller Bot  Parts:

  2 x Joystick Modules
  1 x Arduino Nano
  1 x Nano Breakout Board with headers for NRF
  1 x NRF2401 2.4gHz Radio Transceiver
  1 x 5 AA Battery Holder
  5 x 1.2V NiMh AA Batteries
  1 x Breadboard Half Size


*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 remoteRadio(10, 9);  //CE, CSN

const byte tankBotAddress[6] = {"00001"};

int leftXPin = 17; //A3
int leftYPin = 16; //A2
int rightXPin = 15; //A1
int rightYPin = 14;  //A0
int leftButtonPin = 4;
int rightButtonPin = 5;

int leftXVal = 0;
int leftYVal = 0;
int rightXVal = 0;
int rightYVal = 0;
int leftButtonVal = 0;
int rightButtonVal = 0;

struct SEND_DATA {

  int16_t left_xAxis;
  int16_t left_yAxis;
  int16_t left_button;

  int16_t right_xAxis;
  int16_t right_yAxis;
  int16_t right_button;
};

SEND_DATA dataSent;

const int dataCount = 6;

unsigned long remoteCurrent = 0;
unsigned long remotePrevious = 0;
unsigned long remoteReadPrevious = 0;
const long remoteInterval = 100;


void setup() {

  Serial.begin(9600);
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);

  remoteRadio.begin();

  remoteRadio.openWritingPipe(tankBotAddress);
  remoteRadio.setPALevel(RF24_PA_HIGH);
  remoteRadio.stopListening();

}


void loop() {

  //testRadio_Send(100);
  sendController();

}

void testRadio_Send(int delayTime) {

  const char sendText[] = "Hello Bot";
  remoteRadio.write(&sendText, sizeof(sendText));
  Serial.print("Controller Sends: ");
  Serial.println(sendText);
  delay(delayTime);

}




void sendController() {

  remoteCurrent = millis();

  if (remoteCurrent - remotePrevious >= remoteInterval) {

    remotePrevious  = remoteCurrent;

    leftXVal = analogRead(leftXPin);
    leftYVal = analogRead(leftYPin);
    leftButtonVal = digitalRead(leftButtonPin);
    rightXVal = analogRead(rightXPin);
    rightYVal = analogRead(rightYPin);
    rightButtonVal = digitalRead(rightButtonPin);

    dataSent.left_xAxis = leftXVal;
    dataSent.left_yAxis = leftYVal;
    dataSent.left_button = leftButtonVal;
    dataSent.right_xAxis = rightXVal;
    dataSent.right_yAxis = rightYVal;
    dataSent.right_button = rightButtonVal;

    remoteRadio.write(&dataSent, sizeof(dataSent));

   // Serial.print("VALS: ");
   // printStuff();
  }

}



void printStuff() {

  Serial.print(dataSent.left_xAxis);
  Serial.print("  :  ");
  Serial.print(dataSent.left_yAxis);
  Serial.print("  :  ");
  Serial.print(dataSent.left_button);
  Serial.print("  :  ");
  Serial.print(dataSent.right_xAxis);
  Serial.print("  :  ");
  Serial.print(dataSent.right_yAxis);
  Serial.print("  :  ");
  Serial.println(dataSent.right_button);

}
