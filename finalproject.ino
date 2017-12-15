#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#include <Encoder.h>
#include <MIDI.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(1);

//x and y variables
float x = 0.0;
float y = 0.0;
float xAvg = 0.0;
int xMap = 0;
int xArray[64];
//bypass switch
int switchPin = 37;
//leds
int ledBlue = 35;
int ledRed = 36;
int ledGreen = 34;

int remap = 0;
int remap2 = 0;

//pots
//int potQ = 5;
//int potGain = 6;

//rotary pot to control range
//Rotary Encoder Wheel Part
Encoder rotaryWheel(32, 31);
int newPosition = 0;
int oldPosition = 0;

int rotaryVal = 0;
int realRotaryVal = 0;
int resetRotaryVal = 0;

//led for rotary sequence
int rotaryLeds[4] = { 29, 30, 39, 38 };

/* Get a new sensor event */
sensors_event_t event;

void setup(void)
{
  Serial.begin(57600);

  accel.begin();
  accel.setRange(ADXL345_RANGE_16_G);

  pinMode (switchPin, INPUT);
  pinMode (ledBlue, OUTPUT);
  pinMode (ledRed, OUTPUT);
  pinMode (remap, INPUT);
  pinMode (remap2, INPUT);
  pinMode (ledGreen, OUTPUT);
  for ( int i = 0; i < 4; i++) {
    pinMode(rotaryLeds[i], OUTPUT);
  }
}

void loop(void)
{
  //always want to get axis
  getAxes();
  //turn on appropriate led
  setLeds();
  if (digitalRead(switchPin) == HIGH) {
    //run the things to read serial and send data
    sendMax();
  }
  else {
    //send a serial.write command that changes the filter to allpass
    //instead of bypass. send a 0
    //if want to react faster, I can override all the xArray to equal 0
    //and eventually call sendMax()
    Serial.write(0);
  }
  //rotary wheel methods
  checkRotaryWheel();
  sendRotaryVal();
}

//method to get values
void getAxes() {
  //getEvent - variable set up top
  accel.getEvent(&event);
  for (int i = 0; i < 64; i++) {
    x = event.acceleration.x;
    xArray[i] = mapAxes(x);
  }
}

//send these x values to max
void sendMax() {
  //cannot serial.write a float
  //use Serial.write(buffer, length) bundles of 64 bytes are most efficient
  for (int i = 0; i < 64; i++) {
    Serial.write(xArray[i]);
  }
}

//map the x value from -9. to 9. => 0 255
int mapAxes(float x) {
  int mapped = 0;
  mapped = map(x, -9, 9, 255, 0);
  return mapped;
}

void setLeds() {
  if (digitalRead(switchPin) == HIGH) {
    //turn off blue led
    digitalWrite(ledBlue, LOW);
    //analog write the red led and green led
    for (int i = 0; i < 64; i++) {
      remap = map(xArray[i], 0, 255, 250, 0);
      analogWrite(ledRed, remap);
      remap2 = map(xArray[i], 10, 255, 10, 250);
      analogWrite(ledGreen, remap2);
    }
  }
  else {
    //turn red led off and blue on
    analogWrite(ledRed, 0);
    analogWrite(ledGreen, 0);
    digitalWrite(ledBlue, HIGH);
  }
}

void sendRotaryVal() {
  if (rotaryVal == 0) {
    usbMIDI.sendControlChange(0, 1, 1);
    usbMIDI.sendControlChange(0, 0, 1);
    usbMIDI.sendControlChange(0, 0, 1);
    usbMIDI.sendControlChange(0, 0, 1);
    ledOnOff(0);
  }
  if (rotaryVal == 1) {
    usbMIDI.sendControlChange(0, 0, 1);
    usbMIDI.sendControlChange(0, 2, 1);
    usbMIDI.sendControlChange(0, 0, 1);
    usbMIDI.sendControlChange(0, 0, 1);
    ledOnOff(1);
  }
  if (rotaryVal == 2) {
    usbMIDI.sendControlChange(0, 0, 1);
    usbMIDI.sendControlChange(0, 0, 1);
    usbMIDI.sendControlChange(0, 3, 1);
    usbMIDI.sendControlChange(0, 0, 1);
    ledOnOff(2);
  }
  if (rotaryVal == 3) {
    usbMIDI.sendControlChange(0, 0, 1);
    usbMIDI.sendControlChange(0, 0, 1);
    usbMIDI.sendControlChange(0, 0, 1);
    usbMIDI.sendControlChange(0, 4, 1);
   ledOnOff(3);
  }
}


//use rotary knob to implement different set frequency ranges
//4 different ranges: presets
//cry baby: 350 - 2.2k
void checkRotaryWheel() {
  newPosition = rotaryWheel.read();
  //every click,the position moves forwards or backwards by 4
  if (newPosition == oldPosition + 4) {
    oldPosition = newPosition;
    //up one postion
    rotaryVal++;
  }
  else if (newPosition == oldPosition - 4) {
    oldPosition = newPosition;
    //down one position
    rotaryVal--;
  }
  if (rotaryVal > 3) {
    rotaryVal = 0;
  }
  if (rotaryVal < 0) {
    rotaryVal = 3;
  }
}

//4 leds to display which range is on
void ledOnOff(int led) {
  
  for ( int i = 0; i < 4; i++) {
    if ( i == led ) {
      digitalWrite(rotaryLeds[i],HIGH);
    }
    else {
      digitalWrite(rotaryLeds[i],LOW);
    }
  }
}

//input and output audio from max

//one pot control the gain?
//another pot controls the Q

//auto wah setting!
//switch that turns off wah and turns on wah

//do i need to calibrate?
