/*
  simple shot timer with OLED graphic output, temprature display and
  signaloutput for a relais for baristlights during shot (version 06/06/2016)
  to use with an OLED diplay via I2C.
  by David Kißling (user mactree of http://www.kaffee-netz.de)
  BIG THANK TO:
  magicbugsbunny - for code snippets
  blu - for optimizing the code
  Torsten Wedemeyer - input and optimizing the code
  kn-forum - all the help

  changelog

  v1.006.000 - refactored code and added display of on-time
   005.003 - change tsic libraray
   004.014 - delay check Temp
   004.005 - delete Bounce
   004.004 - change pin mapping
   003.010 - change to Tsic temperature sensor
   002.030 - added "°" to the library font which makes the temperature display a lot easier
   002.026 - slow down the temperature request set resolution again to 12
   002.025 - change behaviour when sensor looses connection
   002.025 - change temp output
   002.020 - set temperature wait for conversion, slows down but stablelize the output
   002.016 - set temperature wait for conversion, slows down but stablelize the output
   002.014 - set temperature resolution 12 (max)
   002.012 - code rewrite, bugfixing
   002.011 - code rewrite
   002.010 - code rewrite
   002.008 - code rewrite, renamed variables, change behavior when P2 is connected but no PI is used
   001.030 - fix temperature output
   001.028 - change behavior of Baristalight
   001.024 - change pins for mv and pump
   001.023 - add PIN13 for baristlight and PIN10 for baristalight with pwm, added millis for better dimming
   001.011 - code rewrite, post infusion in upper reight corner
   001.001 - code rewrite
   000.010 - change relais pin to 10 for pwm, add postinfusion,
       009 - change behavior of signalinput P1 ist starting the timer and P2 is switchich the PI
       008 - bugfix: degree when temperature sinks under 100°
       004 - change temprature display
       003 - release

*/

// libraries
#include <MsTimer2.h>
#include <Arduino.h>
#include <Wire.h>
#include "Display.hpp"
#include "TSIC.h"

// comment in for use with relais
//#include <Bounce.h>

// setup for the tsic sensor on pin 2
TSIC Sensor1(2, 3); // Signalpin, VCCpin,
//TSIC Sensor1(2);    // only Signalpin, VCCpin unused by default
//TSIC Sensor2(3);    // only Signalpin, VCCpin unused by default

//OLED on I2C-bus
Display lcd;

// define signal input
const int btnPUMPpin = 7;
const int btnSTARTpin = 5;

// define pin => connect to IRLIZ44N (GATE) with PWM
int baristaLightPWM = 6;


// define how long the last shot-time will be shown
int showLastShot = 1000; // 500 => 5s; 1000 => 10s

// define global variables
int tick = 0;
void Tick() {
  tick++;
}

int tcount = 0;
bool isTimerStarted = false;
bool isSleeptimerRun = false;
bool isSleep = true;
bool isHold = true;
const char* version[] = {"Espresso", "Shot Timer", "v1.005.045"};
long previous = 0;
long onTIME = 0;

unsigned int startTime = 0;
unsigned int now() { return millis() / 1000 ; }

bool getSecondTime = false;
bool getFirstTime = false;
bool getThirdTime = false;

float TIME = 0;
int firstTIME = 0;
int secondTIME = 0;

// sensors
int checkTemp = 0;

// sensor 1
float TEMP = 0;
uint16_t temperature = 0;
uint16_t previousTMP = 0;

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int error = 0;

// light
bool turnLightOn = false;
bool isLightOn = false;
bool turnLightOff = false;
bool isLightOff = false;
int dimm = 0;
int brightness = 200; // 255 => max 127 => 50%
unsigned long turnOffDelay = 0;

// display
bool showStartScreen = true;

void setup() {
  //Serial.begin(9600);

  pinMode(btnSTARTpin, INPUT);
  digitalWrite(btnSTARTpin, HIGH);
  pinMode(btnPUMPpin, INPUT);
  digitalWrite(btnPUMPpin, HIGH);
  pinMode(baristaLightPWM, OUTPUT);
  digitalWrite(baristaLightPWM, LOW);
  delay(40);

  lcd.Setup(version);

  delay(3000);

  //check if temperature sensor tsic is connected runs only once

  Sensor1.getTemperature(&temperature);
  Serial.println(temperature);
  TEMP = Sensor1.calc_Celsius(&temperature);
  Serial.println(TEMP);
  if (TEMP > 0) {
    checkTemp = checkTemp + 1;
  }
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  
  startTime = now();  
}

void loop() {
 
  //check for signal
  int klick1 = !digitalRead(btnSTARTpin);
  int klick2 = !digitalRead(btnPUMPpin);

  // active signal on P1 start timer
  if (klick1) {
    if (!isTimerStarted) {
      MsTimer2::set(10, Tick);
      MsTimer2::start();

      lcd.Clear();

      // set variable
      isTimerStarted = true;
      tick = 0;
      TIME = 0;
      firstTIME = 0;
      secondTIME = 0;
      isSleep = false;
      isSleeptimerRun = false;
      isHold = true;
      turnLightOn = true;
      showStartScreen = false;
    }
    
	// timer is running
    if (isTimerStarted) {
 	  float countF = tick;
 	  int TIME = countF / 100;
 	  
	  // get first time
 	  if (!getFirstTime) {
 	    getFirstTime = true;
 	  }
 	  // get second time
 	  if (klick2 && !getSecondTime) {
 	    firstTIME = TIME;
 	    getSecondTime = true;
 	    tick = 0;
 	  }
 	  // get third time
 	  if (getFirstTime && getSecondTime && !getThirdTime && !klick2) {
 	    secondTIME = TIME;
 	    TIME = 0;
 	    tick = 0;
 	    getThirdTime = true;
 	  }
 	
 	  if (!getThirdTime) {
 	    lcd.DrawTimer(TIME);
 	  }
 	  if (firstTIME != 0 && getSecondTime) {
 	    lcd.DrawPreInfusion(5, 0, firstTIME);
 	  }
 	  if (TIME > 0 && getThirdTime ) {
 	    lcd.DrawPreInfusion(70, 0, TIME);
 	  }
    }
  }
  else {
    // no active signal
    if (isTimerStarted) {
      MsTimer2::stop();
      float countF = tick;
      TIME = countF / 100;
      isTimerStarted = false;
      isSleep = true;
      tick = 0;
      //      tcount = 0;
      getFirstTime = true;
      getSecondTime = false;
      getThirdTime = false;
    }
  }
  
  if (isSleep) {
    if (!isSleeptimerRun) {
      MsTimer2::set(10, Tick);
      MsTimer2::start();
      isSleeptimerRun = true;
    }
	
	if (tick > showLastShot || TIME + firstTIME + secondTIME < 8) {
	  MsTimer2::stop();
	  lcd.Clear();
	  isSleeptimerRun = false;
	  tick = 0;
	  isSleep = false;
	  isHold = false;
	  turnLightOff = true;
	  getFirstTime = false;
	  getSecondTime = false;
	  getThirdTime = false;
	}
  }

  if (!isTimerStarted && !isSleep) {
	lcd.DrawOnTime(now() - startTime);
  }
  
  // get and display temperature runs only if tsic is present on startup
  if (checkTemp && !isTimerStarted && !klick1 && !isHold) {
    if (tcount >= 20000) {
      tcount = 0;
      if (checkTemp == 1) {
        getTemp();
		lcd.DrawGroupTemperature(TEMP);
      }
    }

    tcount++;
  }

  if (turnLightOn && !isLightOn) {
    // turn BaristaLight on
    isLightOn = true;
    isLightOff = false;
    turnLightOff = false;
    if (dimm < 100) {
      dimm = 100;
    }
    turnOffDelay = 0;
  }

  if (isLightOn) {
    if (dimm < brightness) {
      unsigned long current = millis();
      if (current - previous > 10) {
        previous = current;
        dimm = dimm + 5;
      }
    }
    analogWrite(baristaLightPWM, dimm);
  }

  if (turnLightOff && !isLightOff && dimm == brightness) {
    // turn BaristaLight off
    turnLightOn = false;
    isLightOn = false;
    isLightOff = true;
    turnOffDelay = millis();
  }

  if (isLightOff) {
    unsigned long current = millis();

    if (current - turnOffDelay > 90000) {
      if (dimm > 0 ) {
        //unsigned long current = millis();
        if (current - previous > 10) {
          previous = current;
          dimm = dimm - 5;
        }
      }
      if (dimm < 10) {
        isHold = false;
      }
      analogWrite(baristaLightPWM, dimm);
    }
  }
}
