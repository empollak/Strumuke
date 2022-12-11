#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>

extern Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
extern Adafruit_ADS1115 ads1 = Adafruit_ADS1115();
extern Adafruit_ADS1115 ads2 = Adafruit_ADS1115();
extern int knownSafe[3][3] = {{260, 460, 490}, {480, 450, 490}, {250, 120, 110}};
