#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>
#include "Arm.h"
#include "Global.h"

// Need to have two separate arrays for the pot reading and the required pulselen. Somehow need to output that so the other code can use it.


// Implement jogging for all the arms
// Implement the ADC


Arm arm1(1);

Arm arms[3] = {Arm(1), Arm(2), Arm(3)};

// Chord AM (new ServoPositions(238, 332, 390), new ServoPositions(446, 254, 399), new ServoPositions(1726, 1726, 1726));

bool arrayEquals(int* array1, int* array2) {
  int array1Size = sizeof(array1)/sizeof(array1[0]);
  int array2Size = sizeof(array2)/sizeof(array2[0]);
  if (array1Size != array2Size) return false;
  for (int i = 0; i < array1Size; i++) {
    if (array1[i] != array2[i]) return false;
  }
  return true;
}

/**
 * Do array1 = array2
*/
void setArraysEqual(int* array1, int* array2) {
  int array1Size = sizeof(array1)/sizeof(array1[0]);
  int array2Size = sizeof(array2)/sizeof(array2[0]);
  if (array1Size != array2Size) return;
  for (int i = 0; i < array1Size; i++) {
    array2[i] = array1[i];
  }
}

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
  // put your setup code here, to run once:
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
	String chords[] = {"CM", "FM", "Am"} ; // CM FM AM
  int chordsSize = sizeof(chords)/sizeof(chords[3]);
  // servoData holds all of the pulselen data for each servo for every arm.
  // First layer is which chord, second layer is which arm, third layer is which servo
  int servoData[chordsSize][3][3];
  // servodatapot will have the position feedback data
  int servoDataPot[chordsSize][3][3];
  String input;
  
  for (Arm arm : arms) {
    Serial.println("Set neutral position for arm " + String(arm.num + 1) + " then press b");
    getInput();
    int armPos[3] = {arm.getServo(0), arm.getServo(1), arm.getServo(2)};
    setArraysEqual(arm.neutral, armPos);
  }

  for (int string = 0; string < 4; string++) {
    for (int fret = 0; fret < 4; fret++) {      
      for (Arm arm : arms) {
        Serial.println("Set arm " + String(arm.num + 1) + 
                " for string " + String(string + 1) + " 1=A " + 
                " fret " + String(fret) + " then press b, or n if it is neutral");

        String input = getInput();

        if (input.equals("b")) {
          int armPos[3] = {arm.getServo(0), arm.getServo(1), arm.getServo(2)};
          setArraysEqual(arm.noteData[string][fret], armPos);
        } else {
          setArraysEqual(arm.noteData[string][fret], arm.neutral);
        }
      }
    }
  }

  pwm.wakeup();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  Serial.println("Calibrating...");

  // Calibrate the pulselen for the arms based on pos feedback
  for (Arm arm : arms) {
    // Save the position feedback data before calibrating the pulselen
    setArraysEqual(arm.neutralPot, arm.neutral);

    Serial.println("Neutral");
    arm.calibrateForChord(arm.neutral);

    for (int string = 0; string < 4; string++) {
      for (int fret = 0; fret < 3; fret++) {
        int *posData = arm.noteData[string][fret];

        if (arrayEquals(arm.neutralPot, posData)) setArraysEqual(posData, arm.neutral);
        else {
          arm.calibrateForChord(posData);

          Serial.println("Place arm " + String(arm.num + 1) + " for string " + String(string + 1) + " (1 = A), fret " + String(fret + 1) + " (from the nut)");

          arm.jogArm(posData);
          
        }
      }
    }
    arm.moveTo(arm.neutral);
  }

  for (Arm arm : arms) {
    arm.print();
  }

  pwm.sleep();
}

void loop() {
  // put your main code here, to run repeatedly:
}