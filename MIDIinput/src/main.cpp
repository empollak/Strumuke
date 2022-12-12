#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_ADS1X15.h>
#include "Chord.h"
#include "Arm.h"

extern Adafruit_PWMServoDriver pwm;
extern Adafruit_ADS1115 ads1;
extern Adafruit_ADS1115 ads2;
extern int knownSafe[3][3];

Arm arm1 = Arm(1, (const int[3]) {262, 464, 485}, (const int[3]) {6600, 11482, 11990}, (const int[4][4][3]) {{{262, 464, 485}, {262, 464, 485}, {0, 0, 0}, {0, 0, 0}}, {{290, 316, 327}, {250, 306, 307}, {0, 0, 0}, {0, 0, 
0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}});
Arm arm2 = Arm(2, (const int[3]) {421, 387, 497}, (const int[3]) {10398, 9792, 12247}, (const int[4][4][3]) {{{486, 211, 466}, {421, 387, 497}, {0, 0, 0}, {0, 0, 0}}, {{464, 222, 436}, {509, 220, 403}, {0, 0, 0}, {0, 0, 
0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}});
Arm arm3 = Arm(3, (const int[3]) {226, 100, 103}, (const int[3]) {224, 89, 104}, (const int[4][4][3]) {{{142, 268, 150}, {226, 100, 103}, {0, 0, 0}, {0, 0, 0}}, {{157, 271, 174}, {226, 100, 103}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}});


void setup() {
  // put your setup code here, to run once:
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  Serial.begin(9600);
  Serial.setTimeout(100);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
}

int pwmVal = 200;
int min = 200;
int max = 650;
void loop() {
  // put your main code here, to run repeatedly:
  //while (Serial.available() == 0) {}
  String input;
  bool read = false;
  //while (Serial.available() == 0) {}
  if (Serial.available() > 0) {
    input = Serial.readString();
    read = true;
    if (input.equals("1")) {
      digitalWrite(7, HIGH);
      pwmVal += 100;
      if (pwmVal > max) {
        pwmVal = min;
      }
    } else if (input.equals("0")) {
      digitalWrite(7, LOW);
    }
  }

  pwm.setPWM(0, 0, pwmVal);
  /*String test = "{1}";
  char *to_send = &test[0];
  Serial.write(to_send);*/
  /*if (read) {
    Serial.println(input);
  }*/
}