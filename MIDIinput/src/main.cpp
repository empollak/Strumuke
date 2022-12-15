#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_ADS1X15.h>
#include "Chord.h"
#include "Arm.h"
#include "CustomStepper.h"

extern Adafruit_PWMServoDriver pwm;
extern Adafruit_ADS1115 ads1;
extern Adafruit_ADS1115 ads2;
extern int knownSafe[3][3];

CustomStepper stepper(9, 8, 1);

Arm arm1 = Arm(1, (const int[3]) {262, 464, 485}, (const int[3]) {6600, 11482, 11990}, (const int[4][4][3]) {{{262, 464, 485}, {262, 464, 485}, {0, 0, 0}, {0, 0, 0}}, {{290, 316, 327}, {250, 306, 307}, {0, 0, 0}, {0, 0, 
0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}});
Arm arm2 = Arm(2, (const int[3]) {421, 387, 497}, (const int[3]) {10398, 9792, 12247}, (const int[4][4][3]) {{{486, 211, 466}, {421, 387, 497}, {0, 0, 0}, {0, 0, 0}}, {{464, 222, 436}, {509, 220, 403}, {0, 0, 0}, {0, 0, 
0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}});
Arm arm3 = Arm(3, (const int[3]) {226, 100, 103}, (const int[3]) {224, 89, 104}, (const int[4][4][3]) {{{142, 268, 150}, {226, 100, 103}, {0, 0, 0}, {0, 0, 0}}, {{157, 271, 174}, {226, 100, 103}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}});


Chord neutral(arm1.neutral, arm2.neutral, arm3.neutral);
Chord neutralPot(arm1.neutralPot, arm2.neutralPot, arm3.neutralPot);
Chord CM(arm1.neutral, arm2.neutral, arm3.noteData[0][2]);
// Dbm?
// Check out DM
Chord EM(arm1.noteData[3][0], arm2.noteData[0][1], arm3.noteData[2][3]);
// Check out E7 when the arms are assembled
// Also check out Em
Chord FM(arm1.noteData[3][1], arm2.noteData[1][0], arm3.neutral);
Chord GM(arm1.noteData[2][1], arm2.noteData[0][1], arm3.noteData[1][2]);
Chord AM (arm1.noteData[3][1], arm2.noteData[2][0], arm3.neutral);
Chord Am (arm1.noteData[3][1], arm2.neutral, arm3.neutral);
Chord seven[12];
Chord major[12]; // holds all major chords
Chord minor[12];

void placeArms(Chord chord) {
  // arm1.moveTo(chord.arm1Pos);
  // arm2.moveTo(chord.arm2Pos);
  // arm3.moveTo(chord.arm3Pos);
}

void strum() {
  Serial.println("strum!");
}

void damp() {
  Serial.println("damp");
  digitalWrite(4, HIGH);
}

void undamp() {
  Serial.println("undamp");
  digitalWrite(4, LOW);
}

void setup() {
  pinMode(12, OUTPUT);
  for (int i = 0; i < 12; i++) {
    minor[i] = neutral;
    major[i] = neutral;
    seven[i] = neutral;
  }
  major[9] = AM;
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(100);
  // pwm.begin();
  // pwm.setOscillatorFrequency(27000000);
  // pwm.setPWMFreq(50);
  placeArms(neutral);
}

// -- OCTAVES --
// 3 is 7
// 2 is M
// 1 is m
// 0 is control

// -- PITCHES IN OCTAVE 0 --
// 0 - reset arms
// 1 - strum
// 2 - damp on
// 3 - damp off

void loop() {
  // put your main code here, to run repeatedly:
  //while (Serial.available() == 0) {}
  int input;
  //while (Serial.available() == 0) {}
  while (Serial.available() == 0) {}
  input = Serial.readString().toInt();
  Serial.println(input);
  int octave = floor(input/12) - 1;

  int pitch = input % 12;

  //Serial.println("octave " + String(octave) + " pitch " + String(pitch));
  if (octave == 0) {
    if (pitch == 0) placeArms(neutral);
    else if (pitch == 1) strum();
    else if (pitch == 2) damp();
    else if (pitch == 3) undamp();
  }

  if (octave == 2) placeArms(major[pitch]);
}