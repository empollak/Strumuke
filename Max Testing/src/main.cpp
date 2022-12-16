#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("hello");
  pwm.begin();
  Serial.println("2");
}

void loop() {
  // put your main code here, to run repeatedly:

  
}