#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_ADS1115 ads1 = Adafruit_ADS1115();
Adafruit_ADS1115 ads2 = Adafruit_ADS1115();
int knownSafe[3][3] = {{290, 500, 450}, {460, 330, 490}, {270, 110, 100}};
