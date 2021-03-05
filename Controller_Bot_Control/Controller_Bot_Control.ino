/*

  Controller Bot Control
  Justin C Kirk 2020

  info@makerinchief.com
  https://www.makerinchief.com/
  https://github.com/makerinchief/ControllerBot

  Controller Bot  Parts:

  2 x Joystick Modules
  1 x Arduino Nano
  1 x Nano Breakout Board with headers for NRF2401 module
  1 x NRF2401 2.4gHz Radio Transceiver
  1 x 5 AA Battery Holder
  5 x 1.2V NiMh AA Batteries
  1 x Breadboard Half Size

*/

//NRFLite is used to make communication easy
//https://github.com/dparson55/NRFLite
#include <SPI.h>
#include <NRFLite.h>

const static uint8_t CONTROLLER_ID = 1;
const static uint8_t TANK_BOT_ID = 0;

const static uint8_t RADIO_CE = 10;
const static uint8_t RADIO_CSN = 9;

NRFLite remoteRadio;


struct ControllerPacket {

  int16_t left_xAxis;
  int16_t left_yAxis;
  int16_t left_button;

  int16_t right_xAxis;
  int16_t right_yAxis;
  int16_t right_button;

};

ControllerPacket controllerData;

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

unsigned long remoteCurrent = 0;
unsigned long remotePrevious = 0;
long remoteInterval = 10;

void setup() {

  Serial.begin(115200);

  remoteRadio.init(CONTROLLER_ID, RADIO_CE, RADIO_CSN);

  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);

}

void loop() {

    remoteCurrent = millis();
  
    if (remoteCurrent - remotePrevious > remoteInterval) {
  
      //TestController();
      SendController();
      PrintData();
  
      remotePrevious = remoteCurrent;
    }

  //TestController();
  //SendController();
  //PrintData();



    

  
}


void SendController() {

  leftXVal = analogRead(leftXPin);
  leftYVal = analogRead(leftYPin);
  leftButtonVal = digitalRead(leftButtonPin);
  rightXVal = analogRead(rightXPin);
  rightYVal = analogRead(rightYPin);
  rightButtonVal = digitalRead(rightButtonPin);

  controllerData.left_xAxis = leftXVal;
  controllerData.left_yAxis = leftYVal;
  controllerData.left_button = leftButtonVal;
  controllerData.right_xAxis = rightXVal;
  controllerData.right_yAxis = rightYVal;
  controllerData.right_button = rightButtonVal;

  remoteRadio.send(TANK_BOT_ID, &controllerData, sizeof(controllerData));

}


void TestController() {

  leftXVal = analogRead(leftXPin);
  leftYVal = analogRead(leftYPin);
  leftButtonVal = digitalRead(leftButtonPin);
  rightXVal = analogRead(rightXPin);
  rightYVal = analogRead(rightYPin);
  rightButtonVal = digitalRead(rightButtonPin);

  controllerData.left_xAxis = leftXVal;
  controllerData.left_yAxis = leftYVal;
  controllerData.left_button = leftButtonVal;
  controllerData.right_xAxis = rightXVal;
  controllerData.right_yAxis = rightYVal;
  controllerData.right_button = rightButtonVal;

}

void PrintData() {

  Serial.print(controllerData.left_xAxis);
  Serial.print("  :  ");
  Serial.print(controllerData.left_yAxis);
  Serial.print("  :  ");
  Serial.print(controllerData.left_button);
  Serial.print("  :  ");
  Serial.print(controllerData.right_xAxis);
  Serial.print("  :  ");
  Serial.print(controllerData.right_yAxis);
  Serial.print("  :  ");
  Serial.println(controllerData.right_button);

}
