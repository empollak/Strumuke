#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>
#include "Arm.h"

// Need to have two separate arrays for the pot reading and the required pulselen. Somehow need to output that so the other code can use it.


// Implement jogging for all the arms
// Implement the ADC

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_ADS1115 ads1 = Adafruit_ADS1115();
Adafruit_ADS1115 ads2 = Adafruit_ADS1115();

Arm arm1(1);

Arm arms[3] = {Arm(1), Arm(2), Arm(3)};

int knownSafe[3][3] = {{260, 460, 490}, {480, 450, 490}, {250, 120, 110}};
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

// used to define how all of the servos are read
int getServo(int arm, int servo) {
  switch (arm) {
    case 1:
    switch (servo) {
      case 0:
      return ads1.readADC_SingleEnded(0);
      break;

      case 1:
      return ads1.readADC_SingleEnded(1);
      break;

      default:
      return ads1.readADC_SingleEnded(2);
      break;
    }
    break;

    case 2:
    switch (servo) {
      case 0:
      return ads2.readADC_SingleEnded(0);
      break;

      case 1:
      return ads2.readADC_SingleEnded(1);
      break;

      default:
      return ads2.readADC_SingleEnded(2);
      break;
    }

    case 3:
    switch (servo) {
      case 0:
      return analogRead(17);
      break;

      case 1:
      return analogRead(16);
      break;

      case 2:
      return analogRead(15);
      break;
    }
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

int calibrateServo(int arm, int servo, int goal, int start) {
  int pulselen;
  int error = 51;
  int lastPulselen = start;
  int allowableDelta = 50;
  if (arm == 3) allowableDelta = 2;
  while (abs(error) > allowableDelta) {
    int servoPos = getServo(arm, servo);
    error = goal - servoPos;
    if (arm == 3) {
      pulselen = lastPulselen + error/3;
    }
    else {
      pulselen = lastPulselen + map(error, -5000, 5000, -100, 100);
    }
    lastPulselen = pulselen;
    pwm.setPWM(servo + ((arm - 1) * 3), 0, pulselen);

    Serial.println("Calibrating arm " + String(arm) + " servo " + String(servo) + "\t" + "servoPos " + String(servoPos) + "\t" + "goal " + String(goal) + "\t" + "error "+ String(error) + "\t" + "pulselen " + String(pulselen) + "\t");
    
    delay(200);
    servoPos = getServo(arm, servo);
    error = goal - servoPos;
  }

  return pulselen;
}


// Order of servo calibration is 2 -> 0 -> 1 for safety
void calibrateArmForChord(int arm, int * armServoData) {

  pwm.setPWM(0 + ((arm - 1) * 3), 0, knownSafe[arm-1][0]);
  pwm.setPWM(1 + ((arm - 1) * 3), 0, knownSafe[arm-1][1]);
  pwm.setPWM(2 + ((arm - 1) * 3), 0, knownSafe[arm-1][2]);

  delay(300);

  armServoData[2] = calibrateServo(arm, 2, armServoData[2], knownSafe[arm-1][2]);
  armServoData[0] = calibrateServo(arm, 0, armServoData[0], knownSafe[arm-1][0]);
  armServoData[1] = calibrateServo(arm, 1, armServoData[1], knownSafe[arm-1][1]);

}

String printServoData(int* servoData) {
  return "new ServoPositions(" + String(servoData[0]) + ", " + String(servoData[1]) + ", " + String(servoData[2]) + ")";
}

void jogArm(int arm, int* servoData) {

  bool stop = false;
  int offset0 = 0;
  int offset1 = 0;
  int offset2 = 0;
  int delta = 4;
  int dir0 = 1;
  int dir1 = 1;
  int dir2 = 1;
  if (arm == 1) {
    dir0 = 1;
    dir1 = 1;
    dir2 = -1;
  }
  else if (arm == 2) {
    dir0 = -1;
    dir1 = 1;
    dir2 = -1;
  }
  else if (arm == 3) {
    dir0 = -1;
    dir1 = -1;
    dir2 = 1;
  }
  while (!stop) {
    pwm.setPWM(0 + (arm-1)*3, 0, servoData[0] + offset0);
    pwm.setPWM(1 + (arm-1)*3, 0, servoData[1] + offset1);
    pwm.setPWM(2 + (arm-1)*3, 0, servoData[2] + offset2);

    while(Serial.available() == 0) {}
    String input = Serial.readString();
    if (input.equals("u")) {
      offset0 += dir0*delta;
      Serial.println(offset0);
    } else if (input.equals("j")) {
      offset0 -= dir0*delta;
      Serial.println(offset0);
    } else if (input.equals("i")) {
      offset1 += dir1*delta;
      Serial.println(offset1);
    } else if (input.equals("k")) {
      offset1 -= dir1*delta;
      Serial.println(offset1);
    } else if (input.equals("o")) {
      offset2 += dir2*delta;
      Serial.println(offset2);
    } else if (input.equals("l")) {
      offset2 -= dir2*delta; 
      Serial.println(offset2);
    } else if (input.equals("h")) {
      stop = true;
    }
  }

  servoData[0] += offset0;
  servoData[1] += offset1;
  servoData[2] += offset2;
}

void armMoveTo(int* pos, int arm) {
  pwm.setPWM(0 + (arm-1)*3, 0, pos[0]);
  pwm.setPWM(1 + (arm-1)*3, 0, pos[1]);
  pwm.setPWM(2 + (arm-1)*3, 0, pos[2]);
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
  
  
  Serial.println("Set neutral position for arm 1 then press b then enter");
  getInput();
  int arm1Pos[3] = {getServo(1, 0), getServo(1, 1), getServo(1, 2)};
  setArraysEqual(arms[0].neutral, arm1Pos);
  Serial.println("Set neutral position for arm 2 then press b then enter");
  getInput();
  int arm2Pos[3] = {getServo(2, 0), getServo(2, 1), getServo(2, 2)};
  setArraysEqual(arms[1].neutral, arm2Pos);
  Serial.println("Set neutral position for arm 3 then press b then enter");
  getInput();
  int arm3Pos[3] = {getServo(3, 0), getServo(3, 1), getServo(3, 2)};
  setArraysEqual(arms[2].neutral, arm3Pos);
  

  // neutral holds the data for each servo for the neutral position
  // first layer is which arm, second is which servo
  // Here it holds the given position feedback for neutral
  // int neutral[3][3] = {{arm1Pos[0], arm1Pos[1], arm1Pos[2]}, {arm2Pos[0], arm2Pos[1], arm2Pos[2]}, {3, 3, 3}};
  int neutral[3][3] = {{arm1Pos[0], arm1Pos[1], arm1Pos[2]}, {arm2Pos[0], arm2Pos[1], arm2Pos[2]}, {arm3Pos[0], arm3Pos[1], arm3Pos[2]}};
  int neutralPot[3][3];
  
  // Fill servoData with the position feedback given for each chord
  for (int i = 0; i < chordsSize; i++) {
    String chord = chords[i];
    Serial.println("Set arm 1 for chord " + chord + " then press b then enter, or press n then enter if it is neutral");
    String input = getInput();
    if (input.equals("b")) {
      arm1Pos[0] = getServo(1, 0);
      arm1Pos[1] = getServo(1, 1);
      arm1Pos[2] = getServo(1, 2);
    }
    else {
      arm1Pos[0] = neutral[0][0];
      arm1Pos[1] = neutral[0][1];
      arm1Pos[2] = neutral[0][2];
    }
  
    Serial.println("Set arm 2 for chord " + chord + " then press b then enter, or press n then enter if it is neutral");
    input = getInput();
    if (input.equals("b")) {
      arm2Pos[0] = getServo(2, 0);
      arm2Pos[1] = getServo(2, 1);
      arm2Pos[2] = getServo(2, 2);
    }
    else {
      arm2Pos[0] = neutral[1][0];
      arm2Pos[1] = neutral[1][1];
      arm2Pos[2] = neutral[1][2];
    }
  
    Serial.println("Set arm 3 for chord " + chord + " then press b then enter, or press n then enter if it is neutral");
    input = getInput();
    if (input.equals("b")) {
      arm3Pos[0] = getServo(3, 0);
      arm3Pos[1] = getServo(3, 1);
      arm3Pos[2] = getServo(3, 2);
    }
    else {
      arm3Pos[0] = neutral[2][0];
      arm3Pos[1] = neutral[2][1];
      arm3Pos[2] = neutral[2][2];
    }

    
    servoData[i][0][0] = arm1Pos[0];
    servoData[i][0][1] = arm1Pos[1];
    servoData[i][0][2] = arm1Pos[2];
    servoData[i][1][0] = arm2Pos[0];
    servoData[i][1][1] = arm2Pos[1];
    servoData[i][1][2] = arm2Pos[2];
    servoData[i][2][0] = arm3Pos[0];
    servoData[i][2][1] = arm3Pos[1];
    servoData[i][2][2] = arm3Pos[2];
  }

  // Save all the position feedback data before calibrating the pulselen
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      neutralPot[i][j] = neutral[i][j];
      for (int k = 0; k < chordsSize; k++) {
        servoDataPot[k][i][j] = servoData[k][i][j];
      }
    }
  }

  // Rewrite servoData and neutral to hold the pulselength required to obtain the desired position
  pwm.wakeup();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  Serial.println("Calibrating...");
  // CALIBRATING NEUTRAL
  // ARM 1
  Serial.println("Neutral");
  calibrateArmForChord(1, neutral[0]);
  calibrateArmForChord(2, neutral[1]);
  calibrateArmForChord(3, neutral[2]);

  for (int i = 0; i < chordsSize; i++) {
    Serial.println("Chord " + String(chords[i]));
    
    if (!arrayEquals(servoData[i][0], neutralPot[0])) calibrateArmForChord(1, servoData[i][0]);
    else setArraysEqual(servoData[i][0], neutral[0]);

    armMoveTo(neutral[0], 1);

    if (!arrayEquals(servoData[i][0], neutralPot[1])) calibrateArmForChord(2, servoData[i][1]);
    else setArraysEqual(servoData[i][1], neutral[1]);

    armMoveTo(neutral[1], 2);

    if (!arrayEquals(servoData[i][0], neutralPot[2])) calibrateArmForChord(3, servoData[i][2]);
    else setArraysEqual(servoData[i][2], neutral[2]);

    armMoveTo(neutral[2], 3);
  }

  for (int i = 0; i <=2; i++) {
    for (int j = 0; j <= 2; j++) {
      pwm.setPWM(i*3 + j, 0, neutral[i][j]);
    }
  }

  Serial.println("Place arm 1 in neutral with uiojkl, press h to set (will move servos)");
  jogArm(1, neutral[0]);
  Serial.println("Place arm 2 in neutral with uiojkl, press h to set (will move servos)");
  jogArm(2, neutral[1]);
  Serial.println("Place arm 3 in neutral with uiojkl, press h to set (will move servos)");
  jogArm(3, neutral[2]);

  for (int i = 0; i < chordsSize; i++) {
    String chord = chords[i];
    Serial.println("Place arm 1 for chord " + chord + " with uiojkl, press h to set (will move servos)");
    jogArm(1, servoData[i][0]);
    pwm.setPWM(0, 0, neutral[0][0]);
    pwm.setPWM(1, 0, neutral[0][1]);
    pwm.setPWM(2, 0, neutral[0][2]);
    Serial.println("Place arm 2 for chord " + chord + " with uiojkl, press h to set (will move servos)");
    jogArm(2, servoData[i][1]);
    pwm.setPWM(3, 0, neutral[1][0]);
    pwm.setPWM(4, 0, neutral[1][1]);
    pwm.setPWM(5, 0, neutral[1][2]);
    Serial.println("Place arm 3 for chord " + chord + " with uiojkl, press h to set (will move servos)");
    jogArm(3, servoData[i][2]);
    pwm.setPWM(6, 0, neutral[2][0]);
    pwm.setPWM(7, 0, neutral[2][1]);
    pwm.setPWM(8, 0, neutral[2][2]);
  }
  
  Serial.print("Chord neutral (");
  Serial.print(printServoData(neutral[0])+ ", ");
  Serial.print(printServoData(neutral[1])+ ", ");
  Serial.println(printServoData(neutral[2]) + ");");

  for (int i = 0; i < chordsSize; i++) {
    String chord = chords[i];
    Serial.print("Chord " + chord + " (");
    Serial.print(printServoData(servoData[i][0])+ ", ");
    Serial.print(printServoData(servoData[i][1])+ ", ");
    Serial.println(printServoData(servoData[i][2]) + ");");
  }

  Serial.print("Chord neutralPot (");
  Serial.print(printServoData(neutralPot[0])+ ", ");
  Serial.print(printServoData(neutralPot[1])+ ", ");
  Serial.println(printServoData(neutralPot[2]) + ");");

  for (int i = 0; i < chordsSize; i++) {
    String chord = chords[i];
    Serial.print("Chord " + chord + "Pot (");
    Serial.print(printServoData(servoDataPot[i][0])+ ", ");
    Serial.print(printServoData(servoDataPot[i][1])+ ", ");
    Serial.println(printServoData(servoDataPot[i][2]) + ");");
  }

  pwm.sleep();
}

void loop() {
  // put your main code here, to run repeatedly:
}