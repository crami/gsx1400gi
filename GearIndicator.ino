/*
 *  Geear Indicator for a Suzoky GSX 1400
 *  (c) 2014 by Matthias Cramer, cramer@freestone.net
 *  CC BY-NC-SA (http://creativecommons.org/licenses/by-nc-sa/4.0/)
 *
 */

#include <EEPROM.h>

// Gear Values
#define GEAR_1 330
#define GEAR_2 450
#define GEAR_3 600
#define GEAR_4 750
#define GEAR_5 890
#define GEAR_6 980
#define GEAR_N 1000
#define KILL 100

// delays
#define INITIAL_SWEEP_DELAY 80
#define LOOP_DELAY 50

// Static array for LED dim values
static int dimArray[] = {0, 70, 130, 180, 220};
static byte dimSize = sizeof(dimArray);

// LED brightness correction
static int dimCorr[] = {0, 0, 0, 0, 0, 5};

// EEPROM Addresses
static int dimAddress = 0;

// Global Variables
int baseLedPin = 2;
int gearPin = A0;
int gearValue = 0;
byte gear = 0;
byte oldgear =0;
byte kill = 0;
byte killSweapPos = 1;
byte killSweapDir = 0;
int dimPin = 9;
byte dimValue = 0;
int watchdogPin = 13;
byte watchdogCounter = 0;
int buttonPin = 8;
byte buttonState= 1;

// the setup routine runs once when you press reset:
void setup() {
   int i =0;  
  // initialize the digital pin as an output.
  for (i=0; i < 6; i++) {
    pinMode(baseLedPin + i, OUTPUT);
  }
  
  dimValue =  EEPROM.read(dimAddress);
  if (dimValue > dimSize -1) { // EEPROM is not initialized or array size has changed
    dimValue = 0;
  }  
  // Sweep
  for (i=1; i < 7; i++) {
    gearLight(i);
    delay(INITIAL_SWEEP_DELAY);
  }
  for (i=6; i > 0; i--) {
    gearLight(i);
    delay(INITIAL_SWEEP_DELAY);
  }
  
  pinMode(watchdogPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop() {
  gearValue = analogRead(gearPin);
  if (gearValue < KILL) {
    gear = 0;
    kill = 1;
  } else if (gearValue < GEAR_1) {
    gear = 1;
    kill = 0;
  } else if (gearValue < GEAR_2) {
    gear = 2;
    kill = 0;
  } else if (gearValue < GEAR_3) {
    gear = 3;
    kill = 0;
  } else if (gearValue < GEAR_4) {
    gear = 4;
    kill = 0;
  } else if (gearValue < GEAR_5) {
    gear = 5;
    kill = 0;
  } else if (gearValue < GEAR_6) {
    gear = 6;
    kill = 0;
  } else {
    gear = 0;
    kill = 0;
  }
  if (kill == 0) {
    if (gear != oldgear) {
      
      gearLight(gear);
    }
  } else {
    killSweap();
  }
  if ((watchdogCounter % 20) == 0) {
    digitalWrite(watchdogPin, HIGH);
  } else if ((watchdogCounter % 20) == 10) {
    digitalWrite(watchdogPin, LOW);
  }

  if (digitalRead(buttonPin) == 0 and buttonState == 1) {  // Button got pressed
    buttonState=0;
    dimValue++;
    if (dimValue >= dimSize) {
      dimValue=0;
    }
    analogWrite(dimPin, dimArray[dimValue]);
    EEPROM.write(dimAddress, dimValue);
  } else if (digitalRead(buttonPin) == 1 and buttonState == 0) { // Button got released
    buttonState=1;
  }

  watchdogCounter++;
  delay(LOOP_DELAY);

}

// Disable previous LED and enable the one for the selected gear
void gearLight(int g) {
  int dv=0;
  
  dv=dimArray[dimValue]+dimCorr[(g-1)];
  
  digitalWrite(baseLedPin - 1 + oldgear, LOW);
  analogWrite(dimPin, dv);
  digitalWrite(baseLedPin -1 + g, HIGH);
  oldgear=g;
}

// Sweap LEDs up and down when kill switch is on or side stand is down
void killSweap() {
  gearLight(killSweapPos);
  if (killSweapDir == 0) {
    killSweapPos++;
  } else {
    killSweapPos--;
  }
  if (killSweapPos > 6) {
    killSweapDir = 1;
    killSweapPos = 6;
  } else if (killSweapPos < 1) {
    killSweapDir = 0;
    killSweapPos = 1;
  }
}

