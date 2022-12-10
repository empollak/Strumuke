// Rafe Pollak

#include <Arduino.h>
#include <RBE1001Lib.h>
#include <ESP32Servo.h>

//define the states in easy-to-read terms
enum AlarmState {ALARM_OFF, ALARM_ARMED, ALARM_INTRUDER};
AlarmState currentState = ALARM_OFF; //start off with the alarm deactivated

//Some pin definitions:
const int buttonArmingPin = 0;
const int buttonDisarmingPin = 32;
const int photoPin  = 35;
const int piezoPin  = 26;
const int ledPin    = 33;
const int servoPin  = 14;

Servo servo;

bool skipNext = false; // Need to ignore the first value after the LED is turned on. It's almost always bad.
void HandleArmingButton(void)
{
  if(currentState == ALARM_OFF)
  {
    Serial.println("Arming!");

    //take action: light the LED
    digitalWrite(ledPin, HIGH);    
    skipNext = true;

    //take action: you get to ADD CODE HERE to move the servo!
    servo.write(135);

    digitalWrite(ledPin, HIGH);
    
    currentState = ALARM_ARMED; //Don't forget to change the state!
  }
}

void HandleLaserBroken(void)
{
  if(currentState == ALARM_ARMED) {
    Serial.printf("Alarm activated! \t %d \n", analogRead(photoPin));

    // Sound the alarm
    tone(piezoPin, 1200);

    currentState = ALARM_INTRUDER;
  }
}

void HandleDisarmingButton(void)
{
  // If the system is armed or alarming, unlock the door, turn off the alarm, and turn off the LED
  if(currentState == ALARM_ARMED || currentState == ALARM_INTRUDER) {
    Serial.println("Disarmed.");

    servo.write(0);
    noTone(piezoPin);
    digitalWrite(ledPin, LOW);
    currentState = ALARM_OFF;
  }
}

/*
 * Code for checking the button -- you don't need to edit this one
 */
bool CheckArmingButton(void)
{
  static int prevButtonState = HIGH; //button up => pin reads HIGH

  bool retVal = false;
  
  int currButtonState = digitalRead(buttonArmingPin);
  if(prevButtonState != currButtonState)
  {
    delay(10); //this is a cheat for debouncing -- the only place delay is allowed!
    if(currButtonState == LOW) retVal = true;  //button is down => pin reads LOW
  }
  prevButtonState = currButtonState;

  return retVal;
}

/*
 * Code for checking the laser. YOU WILL NEED TO EDIT THIS ONE
 */
enum {DARK, LIGHT};

bool CheckIfLaserBroken(void)
{
  static int prevLaserSensorState = DARK; //the program starts with the laser off
  const int threshold = 1300;
  bool retVal = false;
  static int currLaserSensorState = DARK;

  static unsigned long lastRead = millis();
  unsigned long currRead = millis();
  // Check every 400 milliseconds for the laser broken
  if(currRead - lastRead > 400) {
    // Skip the first value given after the LED is turned on. It's usually read before the LED actually lights up and is bad
    if (skipNext) {
      skipNext = false;
    } else {
      if (analogRead(photoPin) < threshold) {
        currLaserSensorState = LIGHT;
      }
      else currLaserSensorState = DARK;
    }
  }

  //add code here to detect the EVENT of the laser being broken. See the button checker above for hints
  if (currLaserSensorState == DARK && prevLaserSensorState == LIGHT) {
    retVal = true;
  }
  prevLaserSensorState = currLaserSensorState;
  
  return retVal;
}

/*
 * You'll have to make your own disarming checker!
 */

bool CheckDisarmingButton(void) 
{
  static int prevButtonState = HIGH; //button up => pin reads HIGH

  bool retVal = false;
  
  int currButtonState = digitalRead(buttonDisarmingPin);
  if(prevButtonState != currButtonState)
  {
    delay(10); //this is a cheat for debouncing -- the only place delay is allowed!
    if(currButtonState == LOW) retVal = true;  //button is down => pin reads LOW
  }
  prevButtonState = currButtonState;

  return retVal;
}

void setup(void) 
{
  Serial.begin(115200);
  Serial.println("Hello!");

  pinMode(buttonArmingPin, INPUT_PULLUP); //if we use INPUT_PULLUP, we don't have to have an external pullup resistor
  pinMode(buttonDisarmingPin, INPUT_PULLUP); //if we use INPUT_PULLUP, we don't have to have an external pullup resistor
  pinMode(photoPin, INPUT);
  pinMode(piezoPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  servo.attach(servoPin);
  
  delay(500);

  Serial.println("setup() complete");
}

void loop(void)
/*
 * Our loop() is just a set of checker-handler pairs.
 */
{
  if(CheckArmingButton()) HandleArmingButton();
  if(CheckIfLaserBroken()) HandleLaserBroken();
  if(CheckDisarmingButton()) HandleDisarmingButton();
}