#include <RBE1001Lib.h>
#include <Arduino.h>
#include <button.h>
#include <iostream>
#include <string>

int INPUT_PIN = 35;
int DISARM_PIN = 32;

bool readLaser() {
  return true;
}

bool prevReading = false;
bool checkLaserBreak() {
  bool retVal = false;
  bool currReading = readLaser(); // Assume readLaser produces true if the beam is currently broken
  if (currReading == true && prevReading == false) {
    retVal = true;
  }
  prevReading = currReading;
  return retVal;
}

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  // Every 200 milliseconds, print the value read at INPUT_PIN
  static unsigned long lastRead = 0; // Last millis() the pot value was read
  unsigned long currRead = millis(); 
  float ADC = analogRead(INPUT_PIN);
  float voltage = (ADC/4096.0)*3.3; // ADC converted into voltage
  bool broken; // True if broken, false if not

  if (voltage > 3.3/2.0) {
    broken = true;
  } else {
    broken = false;
  }

  if(currRead - lastRead > 200)
  {

    lastRead = currRead;
    Serial.print(ADC);
    Serial.print('\t');
    Serial.print(voltage);
    Serial.print('\t');
    Serial.print(broken);
    Serial.print('\n');
    
  }
}