#include <Arduino.h>
#include <RBE1001Lib.h>
#include <button.h>

// declare the pin for the LED
const int LED_PIN = 33;
int count = 0;

// Problem 1
int FLASH_TIME = 250; // ms between each flash
int SET_TIME = 1000; // ms between each set of flashes

// Problem 2
const int FIRST_FLASH = 1000; // ms of the length of the first flash
const int OFF_TIME = 1000;      // ms that the LED is off
int ON_TIME = FIRST_FLASH; // ms that the LED is on for


// Create a button object for the built-in button on the ESP32
Button bootButton(BOOT_FLAG_PIN);

// Define two basic states. For this program, they will correspond to an LED state (on or off).
// "enum" stands for "enumerate". Basically, we define a new variable type called ROBOT_STATE.
// We prepend "ROBOT_" to everything to avoid conflicts with other variables that may be defined elsewhere.
enum SYSTEM_STATE {SYS_IDLE, SYS_ACTIVE};

// Declare a variable, robotState, of our new type, ROBOT_STATE. Initialize it to ROBOT_IDLE.
SYSTEM_STATE state = SYS_IDLE;

// Return true when the input number of ms have passed
int lastRead = 0; // ms of the last time the program switched
bool waitFor(int ms) {
    unsigned long currRead = millis();
    if(currRead - lastRead > ms)
    {
      lastRead = currRead;
      return true;
    }
    return false;
}

void setup() 
{
  // This will initialize the Serial at a baud rate of 115200 for prints
  // Be sure to set your Serial Monitor appropriately in platformio.ini
  Serial.begin(115200);

  // TODO: Set the LED pin to be an OUTPUT
  pinMode(LED_PIN, OUTPUT);
  
  // Initialize the button object
  bootButton.Init();

}

/* Here is where all the fun happens. For each state, check for and respond to a button press.
 */ 
void loop()
{

  // Problem 1. Count to 10
  /*
  // Beginning at 1, flash i times all the way to 10. 250 ms between each flash, 1 second between each set of flashes
  for(int i = 1; i <= 10; i++) {
    for (int j = 0; j < i; j++) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("on");
      delay(FLASH_TIME);
      Serial.println("off");
      digitalWrite(LED_PIN, LOW);
      delay(FLASH_TIME);
    }
    delay(SET_TIME);
  }*/

  // Problem 2. Flash for increasingly long times, reset on button press

  
  // Reset the time spent on to 1s when the button is pressed
  if (bootButton.CheckButtonPress()) {
    ON_TIME = FIRST_FLASH;
  }

  // Go through the state machine
  if(state == SYS_IDLE)
  {
    // After waiting OFF_TIME ms, turn the LED on and go active
    if (waitFor(OFF_TIME)){
      digitalWrite(LED_PIN, HIGH);
      state = SYS_ACTIVE;
    }
  }

  //note that we use else..if for each additional state, so it doesn't get confused
  else if(state == SYS_ACTIVE)
  {
    // After the LED has been on for ON_TIME, turn it off
    if (waitFor(ON_TIME)){
      digitalWrite(LED_PIN, LOW);
      state = SYS_IDLE;
      ON_TIME += 1000;
    }
  }
}