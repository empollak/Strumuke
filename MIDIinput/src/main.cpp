#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_ADS1X15.h>
#include <SPI.h>
#include "Chord.h"
#include "Arm.h"
#include "CustomStepper.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_ADS1115 ads1 = Adafruit_ADS1115();
Adafruit_ADS1115 ads2 = Adafruit_ADS1115();
int knownSafe[3][3] = {{290, 500, 464}, {418, 320, 492}, {260, 106, 104}};

CustomStepper stepper = CustomStepper();

Arm arm1 = Arm(1, (const int[3]) {282, 482, 412}, (const int[3]) {6778, 11508, 1328}, (const int[4][4][3]) {{{297, 310, 364}, {253, 271, 284}, {282, 482, 412}, {282, 482, 412}}, {{295, 304, 356}, {254, 296, 340}, {216, 259, 280}, {282, 482, 412}}, {{296, 326, 396}, {250, 320, 376}, {205, 300, 340}, {282, 482, 412}}, {{303, 330, 424}, {245, 334, 408}, {193, 312, 376}, {176, 297, 324}}});
Arm arm2 = Arm(2, (const int[3]) {460, 324, 485}, (const int[3]) {11007, 8044, 11704}, (const int[4][4][3]) {{{455, 231, 459}, {502, 228, 425}, {422, 331, 467}, {460, 324, 485}}, {{438, 208, 403}, {486, 207, 388}, {460, 324, 485}, {460, 324, 485}}, {{436, 194, 364}, {469, 181, 311}, {460, 324, 485}, {460, 324, 485}}, {{460, 324, 485}, {460, 324, 485}, {460, 324, 485}, {460, 324, 485}}});
Arm arm3 = Arm(3, (const int[3]) {240, 113, 104}, (const int[3]) {139, 76, 59}, (const int[4][4][3]) {{{260, 113, 104}, {198, 188, 142}, {240, 182, 124}, {288, 174, 127}}, {{260, 113, 104}, {207, 203, 173}, {242, 202, 160}, {290, 207, 163}}, {{260, 113, 104}, {260, 113, 104}, {248, 114, 107}, {260, 113, 104}}, {{260, 113, 104}, {260, 113, 104}, {260, 113, 104}, {260, 113, 104}}});
// 0: 240  1: 182  2: 124
Chord neutral(arm1.neutral, arm2.neutral, arm3.neutral);
Chord neutralPot(arm1.neutralPot, arm2.neutralPot, arm3.neutralPot);
Chord CM(arm1.neutral, arm2.neutral, arm3.noteData[0][2]);
// Dbm?
// Check out DM
Chord D7(arm1.noteData[3][1], arm2.neutral, arm3.neutral);
Chord EM(arm1.noteData[3][0], arm2.noteData[0][1], arm3.noteData[2][3]);
// Check out E7 when the arms are assembled
// Also check out Em
Chord FM(arm1.noteData[3][1], arm2.noteData[1][0], arm3.neutral);
Chord GM(arm1.noteData[2][1], arm2.noteData[0][1], arm3.noteData[1][2]);
Chord G7(arm1.noteData[2][1], arm2.noteData[1][0], arm3.noteData[0][1]);
Chord AM (arm1.noteData[3][1], arm2.noteData[2][0], arm3.neutral);
Chord Am (arm1.noteData[3][1], arm2.neutral, arm3.neutral);
Chord Aseven (arm1.noteData[2][0], arm2.neutral, arm3.neutral);
Chord* seven[12];
Chord* major[12]; // holds all major chords
Chord* minor[12];

void placeArms(Chord chord) {
  arm1.moveTo(chord.arm1Pos);
  arm2.moveTo(chord.arm2Pos);
  arm3.moveTo(chord.arm3Pos);
}

void strum() {
  //Serial.println("strum!");
  stepper.stepAcross();
}

void damp() {
  Serial.println("damp");
  digitalWrite(4, HIGH);
}

void undamp() {
  Serial.println("undamp");
  digitalWrite(4, LOW);
}

// -- OCTAVES --
// 3 is 7
// 2 is M
// 1 is m
// 0 is control

// -- PITCHES IN OCTAVE 0 --
// 0 - reset arms (C)
// 1 - strum (C#)
// 2 - damp on (D)
// 3 - damp off (D#)

void setup() {
  pinMode(12, OUTPUT);
  for (int i = 0; i < 12; i++) {
    minor[i] = &neutral;
    major[i] = &neutral;
    seven[i] = &neutral;
  }
  major[0] = &CM;
  major[4] = &EM;
  major[5] = &FM;
  major[7] = &GM;
  major[9] = &AM;
  minor[9] = &Am;
  seven[2] = &D7;
  seven[9] = &Aseven;
  seven[7] = &G7;
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(100);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  placeArms(neutral);

  stepper.home();

}



void loop() {
  // put your main code here, to run repeatedly:
  //while (Serial.available() == 0) {}
  int input;
  char buf[50];
  //while (Serial.available() == 0) {}
  Serial.println("huh");
  while (Serial.readBytesUntil(13, buf, 4) == 0) {}
  input = atoi(buf);
  // input = Serial.readString().toInt();
  Serial.println(input);
  
  int octave = floor(input/12) - 2;

  int pitch = input % 12;

  //Serial.println("octave " + String(octave) + " pitch " + String(pitch));
  if (octave == 0) {
    if (pitch == 0) placeArms(neutral);
    else if (pitch == 1) strum();
    else if (pitch == 2) damp();
    else if (pitch == 3) undamp();
  }
  if (octave == 3) placeArms(*(seven[pitch]));
  if (octave == 2) placeArms(*major[pitch]);
  if (octave == 1) placeArms(*minor[pitch]);
}