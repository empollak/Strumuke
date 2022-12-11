#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_ADS1X15.h>
#include "Chord.h"
#include "..\..\Common Headers\Global.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Adafruit_ADS1115 ads1 = Adafruit_ADS1115();

Chord neutral (new ServoPositions(197, 371, 470), new ServoPositions(3, 2, 2), new ServoPositions(6, 3, 3));
Chord CM (new ServoPositions(196, 279, 376), new ServoPositions(0, 0, 0), new ServoPositions(0, 0, 0));
Chord EM (new ServoPositions(128, 292, 393), new ServoPositions(0, 0, 0), new ServoPositions(0, 0, 0));
Chord E7 (new ServoPositions(281, 118, 3), new ServoPositions(0, 0, 0), new ServoPositions(0, 0, 0));
Chord Bbm (new ServoPositions(214, 118, 0), new ServoPositions(0, 0, 0), new ServoPositions(0, 0, 0));

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