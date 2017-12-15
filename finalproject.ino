#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(1);

//x and y variables
float x = 0.0;
float y = 0.0;
float xAvg = 0.0;
int xMap = 0;
int xArray[64];
//bypass switch
int switchPin = 37;
int ledBlue = 35;
int ledRed = 36;
int remap = 0;

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
  
}

void loop(void) 
{ 
  //always want to get axis
  getAxes();
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
  
  /*
  getAxes();
 // printAxes();
  //mapAxes(); 
  sendMax();
  */
 
}

//method to get values
void getAxes() {
  //getEvent - variable set up top
  accel.getEvent(&event);
  //set up variables at top 
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

//setLeds() - if switch pin == High
void setLeds() {
  if (digitalRead(switchPin) == HIGH) {
    //turn off red led
    digitalWrite(ledBlue, LOW);
    //analog write the led
    for (int i = 0; i < 64; i++) {
      remap = map(xArray[i], 0, 255, 250, 0);
      analogWrite(ledRed, remap);
    }
  }
  else {
    //turn red led off and blue on
    analogWrite(ledRed, 0);
    digitalWrite(ledBlue, HIGH);
  }
}
//input and output audio from max

//use rotary knob to implement different frequency ranges

/*
void printAxes() {
  
  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);
 
  //Serial.println(xAvg);
  //delay(100);
}
*/
/**
//variables for buttons, switches and leds

int sampleSize = 15; //average read analog vals

int i = 0; //index of the hand control list
int j = 0; //index of the head control list
int l = 0; //index of the total control list
int channel = 1; //MIDI channel
boolean debug = true;

//variables for accelerometer
int xPin = A4;
int yPin = A5;
int xMax = 558;
int xMin = 470;
int yMax = 595;
int yMin = 475;
int xVal = 0;
int yVal = 0;
int xMidi = 0;
int yMidi = 0;
//controls array? 
//length of array int variable?

void setup() {
  //pinMode(switchPin1, INPUT);
  Serial.begin(9600);
  
}

void loop() {
  //read analog input values and maybe map to MIDI vals
  //printData();
  Serial.print("X: ");
  Serial.println(readPin(xPin));
  Serial.print("Y: ");
  Serial.println(readPin(yPin));
  delay(1000);
}

//reset calibration if need be

//read input function
void readInputs() {
  xVal = readPin(xPin);
  yVal = readPin(yPin);
}

int readPin(int pin) {
  long number = 0;
  for (int i = 0; i < sampleSize; i++) {
    number +=analogRead(pin);
  }
  return number / sampleSize;
}

void printData() {
  Serial.print("X: ");
  Serial.println(readPin(xPin));
  Serial.print("Y: ");
  Serial.println(readPin(yPin));
}
*/

