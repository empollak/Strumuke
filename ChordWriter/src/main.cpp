#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>
#include "Arm.h"

// This file is used to set the arm positions for each string/fret

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_ADS1115 ads1 = Adafruit_ADS1115();
Adafruit_ADS1115 ads2 = Adafruit_ADS1115();
int knownSafe[3][3] = {{290, 500, 412}, {460, 330, 490}, {270, 110, 100}};

Arm arms[] = {Arm(1), Arm(2), Arm(3)};


// Return true if all elements of array1 = array2
bool arrayEquals(int array1[3], int array2[3]) {
  for (int i = 0; i < 3; i++) {
    if (array1[i] != array2[i]) return false;
  }
  return true;
}

// array1 = array2
void setArraysEqual(int array1[3], int array2[3]) {
  for (int i = 0; i < 3; i++) {
    array1[i] = array2[i];
  }
}

// Wait for user input
String getInput() {
  String input = "";
  while (true) {
		while(Serial.available() == 0) {}
    input = Serial.readString();
		if (input == "b") {
			return input;
		}
		else if (input == "n") {
			return input;
		}
	}
}

void setup() {
  pinMode(17, INPUT);
  pinMode(16, INPUT);
  pinMode(15, INPUT);
  Serial.begin(9600);
  Serial.setTimeout(50);
  ads1.begin(0b1001000);
  bool conn = ads2.begin(0b1001010);
  Serial.println(conn);
  while(!conn) {}
  pwm.begin();
  pwm.sleep();
  int numArms = sizeof(arms)/sizeof(arms[0]);
  Serial.println(numArms);
  String input;
  
  // Set neutral for all arms
  for (int i = 0; i < numArms; i++) {
    Serial.println("Set neutral position for arm " + String(arms[i].num) + " then press b");
    getInput();
    int armPos[3] = {arms[i].getServo(0), arms[i].getServo(1), arms[i].getServo(2)};
    setArraysEqual(arms[i].neutral, armPos);
  }

  //Set each arm's position for each fret/string combo
  for (int arm = 0; arm < numArms; arm++) {
    for (int string = 0; string < 4; string++) {
      for (int fret = 0; fret < 4; fret++) {      
        Serial.println("Set arm " + String(arms[arm].num) + 
                " for string " + String(string + 1) + " (1=A), " + 
                "fret " + String(fret + 1) + " then press b, or n if it is neutral");

        String input = getInput();

        if (input.equals("b")) {
          int armPos[3] = {arms[arm].getServo(0), arms[arm].getServo(1), arms[arm].getServo(2)};
          setArraysEqual(arms[arm].noteData[string][fret], armPos);
        } else {
          setArraysEqual(arms[arm].noteData[string][fret], arms[arm].neutral);
        }
      }
    }

    arms[arm].moveTo(knownSafe[arm]);
  }
  Serial.println(arms[1].neutral[1]);

  pwm.wakeup();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  Serial.println("Calibrating...");

  // Calibrate the pulselen for the arms based on pos feedback
  for (int arm = 0; arm < numArms; arm++) {
    // Save the position feedback data before calibrating the pulselen
    setArraysEqual(arms[arm].neutralPot, arms[arm].neutral);

    Serial.println("Neutral ");
    arms[arm].calibrateForChord(arms[arm].neutral);

    // Calibrate each string/fret combo
    for (int string = 0; string < 4; string++) {
      for (int fret = 0; fret < 4; fret++) {
        int *posData = arms[arm].noteData[string][fret];

        if (arrayEquals(arms[arm].neutralPot, posData)) setArraysEqual(posData, arms[arm].neutral);
        else {
          arms[arm].calibrateForChord(posData);

          Serial.println("Use ujikol to place arm " + String(arms[arm].num) + " for string " + String(string + 1) + " (1 = A), fret " + String(fret + 1) + " (first fret is 1). Press h when done");

          arms[arm].jogArm(posData);
          
        }
      }
    }

    // reset arm once done to make space for the next one
    arms[arm].moveTo(arms[arm].neutral);
  }

  for (Arm arm : arms) {
    arm.print();
  }

  pwm.sleep();
}

void loop() {
}