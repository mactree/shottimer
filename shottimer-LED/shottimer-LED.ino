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
TSIC Sensor1(2, 3); 

// Module connection pins (Digital Pins)
#define CLK 19  //A4
#define DIO 18  //A5
TM1637Display display(CLK, DIO);

const uint8_t bar1[] = { SEG_A, SEG_A, SEG_A, SEG_A };
const uint8_t bar2[] = { SEG_G, SEG_G, SEG_G, SEG_G };
const uint8_t bar3[] = { SEG_D, SEG_D, SEG_D, SEG_D };

// define signal input
const int btnSTARTpin = 5;

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

float TIME;

// tempsensor
bool checkTemp = 0; ///
float TEMP = 0;
uint16_t temperature = 0;
bool requestT;

void setup() {
  pinMode(btnSTARTpin, INPUT);
  digitalWrite(btnSTARTpin, HIGH);

  MsTimer2::set(TICK_INTERVAL, [&](){ ++tick; });
  
  Serial.begin(9600);
  Serial.println("setup done");

  // check if temperature sensor tsic is connected 
  Serial.println("check for sensor");
  display.setBrightness(10);
  display.setSegments(bar1);
  delay(500);

  if (Sensor1.getTemperature(&temperature)) {
    TEMP = Sensor1.calc_Celsius(&temperature);  
  };
  
  display.setSegments(bar2);
  delay(500);

  if (TEMP > 0 ) {
    Serial.println("TEMP > 1");
    checkTemp = 1;
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
  bool isStartPressed = digitalRead(btnSTARTpin) == LOW;

  // active signal on P1 start timer
  if (isStartPressed) {
    if (!isTimerRun) {
      MsTimer2::start();

      // reset lcd
      display.clear();

      // set variable
      isTimerRun = true;
      tick = 0;
      TIME = 0;
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
    TIME = countF / 100;
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
      display.showNumberDec(TIME, true, 3, 0);      
    }
  }
  
  if (isSleep && isSleeptimerRun) {
    if ((tick > showLastShot) || ((TIME) < 8)) {
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
  if (checkTemp && !isTimerRun && !isStartPressed && !isHold) {
    if (requestT) {
      requestT = 0;
    }

	if (!requestT && tcount >= 800000) {
       requestT = 1;
       tcount = 0;
      
      if(Sensor1.getTemperature(&temperature)) {
         TEMP = Sensor1.calc_Celsius(&temperature);
      }
	  
      // set TEMP to -1 when sensor looses conection
      Serial.println(TEMP);
      if (TEMP == -127) {
        TEMP = -1;
      }
      display.setBrightness(10);
      
      if (TEMP < 100) {
        int TEMPt = TEMP*10;
        display.showNumberDecEx(TEMPt ,(0x80 >> 1) , false, 3, 0);
      }
      else {
        display.showNumberDec(TEMP, false, 3, 0);       
      }
    }
    tcount++;
  }
}