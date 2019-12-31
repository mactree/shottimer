/*
  simple shot timer with 7-Seg style graphic output, temprature display and
  signaloutput for a relais for baristlights during shot (version 06/06/2016)
  to use with an OLED diplay via I2C.
  by David Kißling (user mactree of http://www.kaffee-netz.de )
  BIG THANK TO:
  magicbugsbunny - for code snippets
  blu - for optimizing the code
  Torsten Wedemeyer - input and optimizing the code
  kn-forum - all the help

  changelog
  
v2.000.000 - rewrite for 1637 based 7Segment  

*/

#include <TM1637Display.h>
#include "TSIC.h"
#include <MsTimer2.h>
#include <Arduino.h>

// setup for the tsic sensor on pin 2
TSIC sensor(2, 3); 

// Module connection pins (Digital Pins)
const int CLK_PIN = 19;  //A4
const int DIO_PIN = 18;  //A5
TM1637Display display(CLK_PIN, DIO_PIN);

const uint8_t bar1[] = { SEG_A, SEG_A, SEG_A, SEG_A };
const uint8_t bar2[] = { SEG_G, SEG_G, SEG_G, SEG_G };
const uint8_t bar3[] = { SEG_D, SEG_D, SEG_D, SEG_D };

// define signal input
const int START_PIN = 5;

// define how long the last shot-time will be shown
int showLastShot = 1000; // 500 => 5s; 1000 => 10s

// increment tick variable every XX ms
const int TICK_INTERVAL = 10; 
int tick = 0;  

long tcount = 1000000;
bool isTimerRun = false;
bool isSleeptimerRun = false;
bool isSleep = true;
bool isHold = true;

float timerValue = 0.0f;

// tempsensor
bool isSensorConnected = false;
float tempInCelsius = 0;
uint16_t temperature = 0;
bool requestT;

void setup() {
  pinMode(START_PIN, INPUT);
  digitalWrite(START_PIN, HIGH);

  MsTimer2::set(TICK_INTERVAL, [&](){ ++tick; });
  
  Serial.begin(9600);
  Serial.println("setup done");

  // check if temperature sensor tsic is connected 
  Serial.println("check for sensor");
  display.setBrightness(10);
  display.setSegments(bar1);
  delay(500);

  if (sensor.getTemperature(&temperature) && sensor.calc_Celsius(&temperature) > 0) {
    isSensorConnected = true;
  };
  
  display.setSegments(bar2);
  delay(500);

  if (isSensorConnected) {
	Serial.println("temperature sensor is active (value > 0 degree)");
    display.setSegments(bar3);
    delay(500);
  }
  else {
    Serial.println("set check For Sensor => FALSE");
    display.setSegments(bar1);
    delay(500);
  }
}

void loop() {

  // check for signal
  bool isStartPressed = digitalRead(START_PIN) == LOW;

  // active signal on P1 start timer
  if (isStartPressed) {
    if (!isTimerRun) {
      MsTimer2::start();

      // reset lcd
      display.clear();

      // set variable
      isTimerRun = true;
      tick = 0;
      timerValue = 0;
      isSleep = false;
      isSleeptimerRun = false;
      isHold = true;
    }
  }

  // timer is running
  if (isTimerRun && isStartPressed) {
    float countF = tick;
    int TIME = countF / 100;
    // display Time
    display.setBrightness(10);
    display.showNumberDec(TIME, true, 3, 0);
  }
  
  // no active signal
  if (isTimerRun && !isStartPressed) {
    MsTimer2::stop();
    float countF = tick;
    timerValue = countF / 100;
    isTimerRun = false;
    isSleep = true;
    tick = 0;
    //tcount = 0;
  }
  
  if (isSleep) {
    if (!isSleeptimerRun) {
      MsTimer2::start();
      isSleeptimerRun = true;
      display.setBrightness(10);
      display.showNumberDec(timerValue, true, 3, 0);      
    }
  }
  
  if (isSleep && isSleeptimerRun) {
    if (tick > showLastShot || timerValue < 8) {
      MsTimer2::stop();
      display.clear();

      //reset variables
      isSleeptimerRun = false;
      tick = 0;
      isSleep = false;
      isHold = false;
      requestT = 1;
      tcount = 1000000;
    }
  }
  
  // get and display temperature runs only if tsic is present on startup
  if (isSensorConnected && !isTimerRun && !isStartPressed && !isHold) {
    if (requestT) {
      requestT = 0;
    }

	if (!requestT && tcount >= 800000) {
       requestT = 1;
       tcount = 0;
      
      if(sensor.getTemperature(&temperature)) {
         tempInCelsius = sensor.calc_Celsius(&temperature);
      }
	  
      // set tempInCelsius to -1 when sensor looses conection
      Serial.println(tempInCelsius);
      if (tempInCelsius == -127) {
        tempInCelsius = -1;
      }
      display.setBrightness(10);
      
      if (tempInCelsius < 100) {
        int TEMPt = tempInCelsius*10;
        display.showNumberDecEx(TEMPt ,(0x80 >> 1) , false, 3, 0);
      }
      else {
        display.showNumberDec(tempInCelsius, false, 3, 0);       
      }
    }
    tcount++;
  }
}