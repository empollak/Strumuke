#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_PWMServoDriver.h>
#include "Arm.h"

extern Adafruit_PWMServoDriver pwm;
extern Adafruit_ADS1115 ads1;
extern Adafruit_ADS1115 ads2;
extern int knownSafe[3][3];

Arm arm1 (1);
Arm arm2 (2);
Arm arm3 (3);

Arm arms[] = {arm1, arm2, arm3};

// Adafruit_ADS1115 ads1 = Adafruit_ADS1115();

// ADC1 (top, front facing) is 0b1001000
// ADC2 (lower, backwards) is 0b1001010

// connect ADDR to ground for 1001000
// to VDD for 1001001
// to SDA for 1001010
// to SCL for 1001011

// base of arm 1 ADC goes from 85 to 510

// PULSE LENGTHS:
// arm 2:
// 260 50 480 all the way up
// 90 170 80 all the way downh
// default neutral: 260 110 90
// int knownSafe[3][3] = {{237, 337, 389}, {469, 253, 432}, {226, 100, 110}};


//int knownSafe[3][3] = {{231, 327, 390}, {446, 256, 396}, {1, 1, 1}}; // fake


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(50);
  Serial.println(ads1.begin(0b1001000));
  ads2.begin(0b1001010);
  pwm.begin();
  pwm.wakeup();
  pwm.reset();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  int arm = 1; 
  Serial.println(String(ads2.readADC_SingleEnded(0)) + "\t" + String(ads2.readADC_SingleEnded(1)) + "\t" + String(ads2.readADC_SingleEnded(2)));
  delay(100);
  delay(50);
  bool stop = false;
  int offset0 = 0;
  int offset1 = 0;
  int offset2 = 0;
  int delta = 10;
  int dir0 = 1;
  int dir1 = 1;
  int dir2 = 1;
  if (arm == 1) {
    dir0 = 1;
    dir1 = 1;
    dir2 = -1;
  }
  if (arm == 2) {
    dir0 = -1;
    dir1 = 1;
    dir2 = -1;
  }
  if (arm == 3) {
    dir0 = -1;
    dir1 = -1;
    dir2 = 1;
  }
  while (!stop) {

    pwm.setPWM(0 + (arm-1)*3, 0, knownSafe[arm-1][0] + offset0);
    pwm.setPWM(1 + (arm-1)*3, 0, knownSafe[arm-1][1] + offset1);
    pwm.setPWM(2 + (arm-1)*3, 0, knownSafe[arm-1][2] + offset2);

    while(Serial.available() == 0) {}
    String input = Serial.readString();
    if (input.equals("u")) {
      offset0 += dir0*delta;
      Serial.println(offset0);
      Serial.println(arms[arm].getServo(0));
    } else if (input.equals("j")) {
      offset0 -= dir0*delta;
      Serial.println(offset0);
      Serial.println(arms[arm].getServo(0));
    } else if (input.equals("i")) {
      offset1 += dir1*delta;
      Serial.println(offset1);
      Serial.println(arms[arm].getServo(1));
    } else if (input.equals("k")) {
      offset1 -= dir1*delta;
      Serial.println(offset1);
      Serial.println(arms[arm].getServo(1));
    } else if (input.equals("o")) {
      offset2 += dir2*delta;
      Serial.println(offset2);
      Serial.println(arms[arm].getServo(2));
    } else if (input.equals("l")) {
      offset2 -= dir2*delta; 
      Serial.println(offset2);
      Serial.println(arms[arm].getServo(2));
    } else if (input.equals("h")) {
      stop = true;
    }
  }
  Serial.println("0: " + String(knownSafe[arm-1][0]+offset0) + "\t" + "1: " + String(knownSafe[arm-1][1]+offset1) + "\t" + "2: " + String(knownSafe[arm-1][2]+offset2));
} 
// 2 goes 11000 to 2200
// 1 goes 12400 to 4200
// 0 goes 2030 to 10780
void loop() {
  
}