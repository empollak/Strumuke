#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <Stepper.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define STEPS 200
// 149 steps across string. Moving away from the stepper motor is positive.

Stepper stepper (STEPS, 8, 9, 10, 11);

int steps = 0;
int lastTime = millis();


void setup() {
  pinMode(17, INPUT);
  Serial.begin(9600);
  stepper.setSpeed(80);
  Serial.setTimeout(50);
  while (Serial.available() == 0) {}
  Serial.println("hey");
  lastTime = millis();
}
void loop() {
  // while (Serial.available() == 0) {}
  // String input = Serial.readString();
  // if (input.equals("o")) {
  //   stepper.step(1);
  //   steps += 1;
  // }
  // if (input.equals("l")) {
  //   stepper.step(-1);
  //   steps -= 1;
  // }
  // Serial.println(steps);
  stepper.step(50);
  delay(500);
  stepper.step(-50);
  delay(500);
  if (Serial.available() != 0) {
    String input = Serial.readString();
    if (!(input.equals("o") || input.equals("l"))) {
      Serial.println("stopped");
      while (true) {}
    }
  }
} 