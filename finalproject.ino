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

/* Get a new sensor event */ 
sensors_event_t event; 

void setup(void) 
{
  Serial.begin(57600);

  accel.begin();
  accel.setRange(ADXL345_RANGE_16_G);
  
}

void loop(void) 
{ 
  getAxes();
 // printAxes();
  //mapAxes(); 
  sendMax();
 
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
    //should i set delay
}

/*
void getAverage() {
  accel.getEvent(&event);
  float sum = 0;
  for (int i = 0; i < 15; i++) {
    sum += event.acceleration.x;
  }
  xAvg = sum / 15;
  
}
*/

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

//should i send the audio input and listen to audio from max
//receive the data from max to output audio?
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

