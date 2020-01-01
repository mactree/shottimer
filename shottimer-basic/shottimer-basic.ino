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
#include "Display.h"
#include <MsTimer2.h>
#include <Arduino.h>

// Module connection pins (Digital Pins)
const int CLK_PIN = 2; //19;  //A4
const int DIO_PIN = 3; //18;  //A5
Display display(CLK_PIN, DIO_PIN);

// define signal input
const int START_PIN = 5;

// increment tick variable every XX ms
const int TICK_INTERVAL = 10;
const int TICK_TOLERANCE = 8;
int tick = 0;
void Tick() {
  ++tick;
}
int getSecondsFromTick() {
  return tick / (1000 / TICK_INTERVAL);
}

// define how long the last shot-time will be shown (depends on TICK_INTERVAL)
const int SHOW_DURATION = 1000; // 500 => 5s; 1000 => 10s

bool isTimerStarted = false;
bool isSleeptimerRun = false;
bool isSleep = true;
bool isHold = true;

int timerValue = 0;

unsigned int startTime = 0;
unsigned int now() { return millis() / 1000 ; }

void setup() {
  pinMode(START_PIN, INPUT);
  digitalWrite(START_PIN, HIGH);

  MsTimer2::set(TICK_INTERVAL, Tick);

  Serial.begin(9600);
  Serial.println("setup done");

  display.Init();
  display.SetBrightness(Display::Brightness::Dark);

  delay(1000);
  startTime = now();
}

void loop() {
  // check for signal
  bool isStartPressed = digitalRead(START_PIN) == LOW;

  // active signal on P1 start timer
  if (isStartPressed) {
    if (!isTimerStarted) {
      MsTimer2::start();

      // set variable
      isTimerStarted = true;
      tick = 0;
      timerValue = 0;
      isSleep = false;
      isSleeptimerRun = false;
      isHold = true;
      display.SetBrightness(Display::Brightness::Bright);
    }
    
    display.DrawShotTime(getSecondsFromTick());
  } 
  else {
    // no active signal
    if (isTimerStarted) {
      MsTimer2::stop();
      timerValue = getSecondsFromTick();
      isTimerStarted = false;
      isSleep = true;
      tick = 0;
    }
  
    if (isSleep) {
      if (!isSleeptimerRun) {
        MsTimer2::start();
        isSleeptimerRun = true;
        display.DrawShotTime(timerValue);
      }
  
      if (tick > SHOW_DURATION || timerValue < TICK_TOLERANCE) {
        MsTimer2::stop();
  
        //reset variables
        isSleeptimerRun = false;
        tick = 0;
        isSleep = false;
        isHold = false;
        display.SetBrightness(Display::Brightness::Dark);
        display.Clear();
      }    
    }
    else {
      auto secs = now() - startTime;
      display.DrawOnTime(secs);
      Serial.println(secs);
    }
  }

  //delay(100);
}
