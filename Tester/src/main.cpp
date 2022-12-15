#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "CustomStepper.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
CustomStepper stepper (9, 8, 1); // placeholder limit switch pin

// 149 steps across string. Moving away from the stepper motor is positive.

int lastTime = millis();

void setup() {
  pinMode(17, INPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(9600);
  Serial.setTimeout(50);
  while (Serial.available() == 0) {}
  Serial.println("hey");
  lastTime = millis();
}

int steps = 0;
void loop() {
  if (Serial.available() != 0) {
    String input = Serial.readString();
    if (input.equals("o")) {
      stepper.step(165, 0);
      // stepper.step(1, 0);
      // steps += 1;
      // Serial.println(steps);
    } else if (input.equals("l")) {
      stepper.step(165, 1);
      // stepper.step(1, 1);
      // steps -= 1;
      // Serial.println(steps);
    } else if (input.equals("k")) {
      Serial.println("damp");
      digitalWrite(4, HIGH);
    } else if (input.equals("i")) {
      Serial.println("undamp");
      digitalWrite(4, LOW);
    } else if (input.equals("p")) {
      stepper.step(1, 0);
    } else if (input.equals(";")) {
      stepper.step(1, 1);
    }
    else {
      digitalWrite(4, LOW);
      Serial.println("stopped");
      while (true) {}
    } 
  }
} 