#include <Arduino.h>
#include "Play.h"
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>
#include "Arm.h"

extern Adafruit_ADS1115 ads1;
extern Adafruit_ADS1115 ads2;
extern Adafruit_PWMServoDriver pwm;
extern int knownSafe[3][3];

Arm arm1 = Arm(1, (const int[3]) {262, 464, 485}, (const int[3]) {6600, 11482, 11990}, (const int[4][4][3]) {{{262, 464, 485}, {262, 464, 485}, {0, 0, 0}, {0, 0, 0}}, {{290, 316, 327}, {250, 306, 307}, {0, 0, 0}, {0, 0, 
0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}});
Arm arm2 = Arm(2, (const int[3]) {421, 387, 497}, (const int[3]) {10398, 9792, 12247}, (const int[4][4][3]) {{{486, 211, 466}, {421, 387, 497}, {0, 0, 0}, {0, 0, 0}}, {{464, 222, 436}, {509, 220, 403}, {0, 0, 0}, {0, 0, 
0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}});
Arm arm3 = Arm(3, (const int[3]) {226, 100, 103}, (const int[3]) {224, 89, 104}, (const int[4][4][3]) {{{142, 268, 150}, {226, 100, 103}, {0, 0, 0}, {0, 0, 0}}, {{157, 271, 174}, {226, 100, 103}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}});


// Rightmost string is A, leftmost is G

enum {up, down};

enum {PLAYING, NEUTRAL};
int ARM_STATE = NEUTRAL;

int tempo = 40; // tempo in bpm

// The amount of time after playing a chord to deaden the strings
// Should be tuned. Maybe be relative to tempo?
float deadenDelay = 100; 

enum string {A, E, C, G, bar};
Chord neutral(arm1.neutral, arm2.neutral, arm3.neutral);
Chord neutralPot(arm1.neutralPot, arm2.neutralPot, arm3.neutralPot);
Chord AM (arm1.noteData[1][1], arm2.neutral, arm3.neutral);

// Arm 1 is the top arm
// Arm 2 is the bottom arm closer to the body of the uke
// Arm 3 is the bottom arm closer to the end of the fretboard

Play* song[] = {new Play (AM, 3, down)};


void armMoveTo(ServoPositions* pos, int arm) {
  pwm.setPWM(0 + (arm-1)*3, 0, pos->base);
  pwm.setPWM(1 + (arm-1)*3, 0, pos->joint1);
  pwm.setPWM(2 + (arm-1)*3, 0, pos->joint2);
}

// want to make this blocking but can't until i get the pot values from ChordWriter
void placeArms(Chord chord) {
  arm1.moveTo(chord.arm1Pos);
  arm2.moveTo(chord.arm2Pos);
  arm3.moveTo(chord.arm3Pos);
  ARM_STATE = PLAYING;
}

int getServoData(int arm, int servo, Chord data) {
  if (arm == 1) return data.arm1Pos[servo];
  else if (arm == 2) return data.arm2Pos[servo];
  else return data.arm3Pos[servo];
}

void resetArms() {
  arm1.moveTo(arm1.neutral);
  arm2.moveTo(arm2.neutral);
  arm3.moveTo(arm3.neutral);

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

void setup() {
 
  pinMode(17, INPUT);
  pinMode(16, INPUT);
  pinMode(15, INPUT);

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

  delay(100);
  
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