/*
  Controller Bot Basic
  Justin C Kirk 2020

  justin.c.kirk@gmail.com
  https://www.makerinchief.com/
  https://github.com/makerinchief/ControllerBot

  Controller Bot  uses:

  2 Joystick Breakout Modules
  1 Arduino Nano with Expansion board
  1 NRF24l01 2.4GHz RF Transceiver
  1 Breadboard
  1 6 volt LiPo Battery

  The joystick modules use analog and digital pins.
  The X and Y axis of both are connected to analog pins.
  The joysticks can be pressed down to act as button inputs which are
  connected to digital pins.

  Analog Pins 0 and 1 are used to for the

  Digital Pins 5 and 6 are used to control front wheel motors

  Digital Pins 7 and 8 are used to control rear wheel motors

  Digital Pins 9 - 13 are used by the RF Transceiver

  This leaves 7 pins unused.

  Analog Pins 2 and 3 can be used with other sensors or as extra output pins.

  Analog Pins 4 and 5 can be used for I2C communication to control multiple PWM values using PCA9685 breakbout boards.

  Digital Pins 2, 3 and 4 can be used with shift registers to expand the number of digital outputs.

  The Arduino's 5 Volt output is wired to a rail of the breadboard for use by any additional sensors.

  The motor driver breakout boards get their power directly from the battery.


  One thing to point out!

  I saw my values for the x and y axis were getting mixed up somewhere
  in my readings.  I then realized the joysticks defautl reference for
  the x and y axis is when the joystick has its pins facing the left side.

  Since I have the joysticks placed so the pins are facing up, the x and y axis
  get flipped.  I correct for this swapping the values in the controller data array.

  Here is a breakdown for the controller data array values:

  [0] = Left X Val ( Which would be joystick Y axis raw data)
  [1] = Left Y Val ( Which would be joystick X axis raw data)
  [2] = Left Button Val ( Press left joystick down)
  [3] = Right X Val ( Which would be joystick Y axis raw data)
  [4] = Right Y Val ( Which would be joystick X axis raw data)
  [5] = Right Button Val ( Press right joystick down)

  Making this correction here allows the data to have a more intuitive feel and keeps the
  wiring simple.



*/


#include <nRF24L01.h>
#include <RF24.h>

RF24 controllerRadio(10, 9);  //CE, CSN
//Use 00001 for Tank Bot
//Use 00002 for Four Wheeler Bot
const byte controllerAddress[6] = {"00002"};

struct SEND_DATA {
  int16_t left_xAxis;
  int16_t left_yAxis;
  int16_t left_button;
  int16_t right_xAxis;
  int16_t right_yAxis;
  int16_t right_button;
};

SEND_DATA sendData;

const int dataCount = 6;
int controllerDataArray[dataCount];

unsigned long controllerCurrent = 0;
unsigned long controllerPrevious = 0;
unsigned long controllerReadPrevious = 0;
const long controllerInterval = 10;


//Joystick Module

int leftXPin = 16;
int leftYPin = 17;

int rightXPin = 14;
int rightYPin = 15;

int leftButtonPin = 3;
int rightButtonPin = 4;


void setup() {

  Serial.begin(9600);

  //The two joysticks can be pressed down to act as buttons
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);

  controllerRadio.begin();
  controllerRadio.openWritingPipe(controllerAddress);  //00001
  controllerRadio.setPALevel(RF24_PA_MIN);
  controllerRadio.stopListening();

}

void loop() {

  controllerCurrent = millis();

  if (controllerCurrent - controllerPrevious >= controllerInterval) {

    //Get values from inputs

    //Left Side
    controllerDataArray[0] = analogRead(leftYPin);
    controllerDataArray[1] = analogRead(leftXPin);
    controllerDataArray[2] = digitalRead(leftButtonPin);

    //Right Side
    controllerDataArray[3] = analogRead(rightYPin);
    controllerDataArray[4] = analogRead(rightXPin);
    controllerDataArray[5] = digitalRead(rightButtonPin);

    //Calibrate data
    controllerDataArray[0] = map(controllerDataArray[0], 0, 1023, 255, -255);
    controllerDataArray[1] = map(controllerDataArray[1], 0, 1023, 255, -255);
    controllerDataArray[2] = map(controllerDataArray[2], 0, 1, 1, 0);

    controllerDataArray[3] = map(controllerDataArray[3], 0, 1023, 255, -255);
    controllerDataArray[4] = map(controllerDataArray[4], 0, 1023, 255, -255);
    controllerDataArray[5] = map(controllerDataArray[5], 0, 1, 1, 0);

    //Print the raw values from inputs
    //printDataArray();

    //Store values in struct
    sendData.left_xAxis = controllerDataArray[0];
    sendData.left_yAxis = controllerDataArray[1];
    sendData.left_button = controllerDataArray[2];
    sendData.right_xAxis = controllerDataArray[3];
    sendData.right_yAxis = controllerDataArray[4];
    sendData.right_button = controllerDataArray[5];

    //Print values in struct
    printSendStruct();

    //Send the raw data
    controllerRadio.write(&sendData, sizeof(SEND_DATA));

    controllerPrevious  = controllerCurrent;

  }

}
void printDataArray() {

  for (int i = 0; i < dataCount; i ++)
  {
    Serial.print(controllerDataArray[i]);
    if (i == dataCount - 1) {
      Serial.println();
    } else {
      Serial.print("  :  ");
    }
  }
}

void printSendStruct() {

  Serial.print(sendData.left_xAxis);
  Serial.print("  :  ");
  Serial.print(sendData.left_yAxis);
  Serial.print("  :  ");
  Serial.print(sendData.left_button);
  Serial.print("  :  ");
  Serial.print(sendData.right_xAxis);
  Serial.print("  :  ");
  Serial.print(sendData.right_yAxis);
  Serial.print("  :  ");
  Serial.println(sendData.right_button);
}
