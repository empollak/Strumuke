#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "CustomStepper.h"

// This file is useful for testing the stepper motor

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
CustomStepper stepper = CustomStepper(); 

int lastTime = millis();

void setup() {
  pinMode(17, INPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(13, INPUT);
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
      stepper.stepAcross();
      // stepper.step(1, 0);
      // steps += 1;
      // Serial.println(steps);
    } else if (input.equals("l")) {
      stepper.stepAcross();
      // stepper.step(1, 1);
      // steps -= 1;
      // Serial.println(steps);
    } else if (input.equals("[")) {
      stepper.step(50, 0);
    } else if (input.equals("'")) {
      stepper.step(50, 1);
    }
    else if (input.equals("k")) {
      Serial.println("damp");
      digitalWrite(4, HIGH);
    } else if (input.equals("i")) {
      Serial.println("undamp");
      digitalWrite(4, LOW);
    } else if (input.equals("p")) {
      stepper.step(1, 0);
    } else if (input.equals(";")) {
      stepper.step(1, 1);
    } else if (input.equals("m")) {
      stepper.home();
    } else if (input.equals("t")) {
      Serial.println("listening...");
      while (digitalRead(13) == HIGH) {}
      Serial.println("Woah!");
    }
    else {
      digitalWrite(4, LOW);
      Serial.println("stopped");
      while (true) {}
    } 
  }
} 