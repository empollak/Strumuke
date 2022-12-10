#include <Arduino.h>
#include "Play.h"
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads1 = Adafruit_ADS1115();
Adafruit_ADS1115 ads2 = Adafruit_ADS1115();
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Rightmost string is A, leftmost is G

enum {up, down};

enum {PLAYING, NEUTRAL};
int ARM_STATE = NEUTRAL;

int tempo = 40; // tempo in bpm

// The amount of time after playing a chord to deaden the strings
// Should be tuned. Maybe be relative to tempo?
float deadenDelay = 100; 

enum string {A, E, C, G, bar};
Chord neutral (new ServoPositions(251, 448, 476), new ServoPositions(422, 410, 505), new ServoPositions(224, 100, 98));
Chord CM (new ServoPositions(241, 430, 480), new ServoPositions(411, 405, 467), new ServoPositions(243, 288, 116));
Chord FM (new ServoPositions(237, 337, 389), new ServoPositions(469, 253, 432), new ServoPositions(226, 100, 110));
Chord Am (new ServoPositions(234, 337, 399), new ServoPositions(411, 405, 468), new ServoPositions(229, 100, 110));
Chord neutralPot (new ServoPositions(6284, 11045, 11766), new ServoPositions(10388, 10260, 12351), new ServoPositions(219, 87, 90));
Chord CMPot (new ServoPositions(6284, 11045, 11766), new ServoPositions(10388, 10260, 12351), new ServoPositions(234, 269, 113));
Chord FMPot (new ServoPositions(6140, 8683, 9959), new ServoPositions(10897, 6690, 10908), new ServoPositions(219, 87, 90));
Chord AMPot (new ServoPositions(6142, 8673, 9957), new ServoPositions(10884, 6670, 9788), new ServoPositions(219, 87, 90));

// Arm 1 is the top arm
// Arm 2 is the bottom arm closer to the body of the uke
// Arm 3 is the bottom arm closer to the end of the fretboard

Play* song[] = {new Play (CM, 3, down), new Play (Am, 1, up), new Play (FM, 1, up), new Play (CM, 1, up), new Play (Am, 1, up), new Play (FM, 1, up), new Play (CM, 1, up)};

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

void Arm1MoveTo (ServoPositions* pos) {
  pwm.setPWM(0, 0, pos->base);
  pwm.setPWM(1, 0, pos->joint1);
  pwm.setPWM(2, 0, pos->joint2);
  Serial.println("Base: " + String(pos->base) + " Joint 1 " + String(pos->joint1) + " Joint 2 " + String(pos->joint2));
}

void armMoveTo(ServoPositions* pos, int arm) {
  pwm.setPWM(0 + (arm-1)*3, 0, pos->base);
  pwm.setPWM(1 + (arm-1)*3, 0, pos->joint1);
  pwm.setPWM(2 + (arm-1)*3, 0, pos->joint2);
}

// want to make this blocking but can't until i get the pot values from ChordWriter
void placeArms(Chord chord) {
  armMoveTo(chord.servoPosArm1, 1);
  armMoveTo(chord.servoPosArm2, 2);
  armMoveTo(chord.servoPosArm3, 3);
  ARM_STATE = PLAYING;
}

int getServoData(int arm, int servo, Chord data) {
  ServoPositions* servos;
  if (arm == 1) servos = data.servoPosArm1;
  else if (arm == 2) servos = data.servoPosArm2;
  else servos = data.servoPosArm3;
  if (servo == 0) return servos->base;
  if (servo == 1) return servos->joint1;
  else return servos->joint2;
}

void resetArms() {
  armMoveTo(neutral.servoPosArm1, 1);
  armMoveTo(neutral.servoPosArm2, 2);
  armMoveTo(neutral.servoPosArm3, 3);

  // Make sure the arms are neutral before releasing them from this, for safety
  // unsigned long startedMoving = millis();
  // while (millis() - startedMoving < 1000) {
  //   int allowableDelta = 500;
  //   bool inPosition = true;
  //   for (int i = 1; i <= 3; i++) {
  //     if (i != 3) { // This if statement should be taken out once all arms are in place
  //       for (int j = 0; j <= 2; j++) {
  //         if (abs(getServo(i, j) - getServoData(i, j, neutralPot)) > allowableDelta) inPosition = false;
  //       }
  //     }
  //   }
  //   // Serial.println(String(inPosition) + "\t" + String(millis() - startedMoving));
  //   if (inPosition) break;
  // }
  delay(50);
  ARM_STATE = NEUTRAL;
}

// This should strum
void strum() {
  Serial.println("strum!");
}

void deaden() {
  Serial.println("dead");
}

void undeaden() {
  Serial.println("undead");
}

int min = 150;
int max = 500;
#define POT_PIN 17
#define SERVO_PIN 16
void setup() {
 
  pinMode(SERVO_PIN, INPUT);
  pinMode(POT_PIN, INPUT);

  Serial.begin(9600);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  ads1.begin(0b1001000);
  ads2.begin(0b1001010);
  
  
  
}


int lastPulselen = 200;
void loop() {
  // put your main code here, to run repeatedly:
  int songLength = sizeof(song)/(sizeof(song[0]));
  int beat = 0; // goes from 0 to however many beats there are in a song

  while (Serial.available() == 0) {}
  Serial.readString();
  Serial.println("going");

  placeArms(song[0]->chord); // can this be blocking
  
  unsigned long lastBeatTime = millis();
  bool deadened = false;
  while (beat < songLength) {
    if (Serial.available() > 0) {
      if (Serial.readString() == "b") {
        Serial.println("stopped");
        pwm.sleep();
        return;
      }
    }

    if (millis() - lastBeatTime >= (1.0/tempo) * 60000 ) { //* song[beat-1]->duration
      lastBeatTime = millis();  
      deadened = false;
      undeaden();
      
      strum();
      
      beat++;
      if (beat >= songLength) {
        if (ARM_STATE == PLAYING) {
          resetArms();
        }
        break;
      }

      delay(deadenDelay);
      deaden();
      deadened = true;

      if (ARM_STATE == PLAYING) {
          resetArms();
      }

      if (ARM_STATE == NEUTRAL) {
          placeArms(song[beat]->chord);
      } 
    }
    
    if (!deadened && millis() - lastBeatTime >= deadenDelay && song[beat]->strumDirection == song[beat-1]->strumDirection) {
      deaden(); // want to be blocking
      strum(); // want to be blocking
      undeaden();
      deadened = true;
    }
  } 

  resetArms();
  pwm.sleep();
}