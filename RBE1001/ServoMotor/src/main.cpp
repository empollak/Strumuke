#include <RBE1001Lib.h>
#include <ESP32Servo.h>
#include <Arduino.h>
#include <button.h>
#include <iostream>
#include <string>

const int INPUT_PIN = 35; 
const int BUZZER_PIN = 26;

// https://wpiroboticsengineering.github.io/RBE1001Lib/classServo.html
Servo servo;
#define SERVO_PIN	14

// Create a button object for the built-in button on the ESP32
Button bootButton(BOOT_FLAG_PIN);

// Define two basic states. For this program, they will correspond to an LED state (on or off).
// "enum" stands for "enumerate". Basically, we define a new variable type called ROBOT_STATE.
// We prepend "ROBOT_" to everything to avoid conflicts with other variables that may be defined elsewhere.
enum SYSTEM_STATE {SYS_IDLE, SYS_ACTIVE};

// Declare a variable, robotState, of our new type, ROBOT_STATE. Initialize it to ROBOT_IDLE.
SYSTEM_STATE state = SYS_ACTIVE;

void handleTimerExpired() {
      
}
void setup() 
{
	Serial.begin(115200);
	// pin definitions https://wpiroboticsengineering.github.io/RBE1001Lib/RBE1001Lib_8h.html#define-members
	servo.attach(SERVO_PIN);
    // Initialize the button object
  bootButton.Init();
  pinMode(INPUT_PIN, INPUT);
}

/*
 * The main loop for the program. The loop function is repeatedly called
 * once the ESP32 is started. 
 */

void loop() 
{
	//uint16_t angle = (millis()/20) % 180;
	//servo.write(angle);
	//Serial.println(angle);
	//delay(10);
  // Go through the state machine
  if(state == SYS_IDLE)
  {
    if(bootButton.CheckButtonPress()) //if the button was pressed, switch to ACTIVE
    {
      // Notify us that we're switching to ACTIVE
      Serial.println("Button press -> ACTIVE");

      servo.attach(SERVO_PIN);
      // Update the state
      state = SYS_ACTIVE;
    }
  }

  //note that we use else..if for each additional state, so it doesn't get confused
  else if(state == SYS_ACTIVE)
  {
    // Every 200 milliseconds, print the value read at INPUT_PIN
    static unsigned long lastRead = 0; // Last millis() the pot value was read
    unsigned long currRead = millis(); 

    if(currRead - lastRead > 200)
    {
      float ADC = analogRead(INPUT_PIN);
      float angle = (ADC/4095.0)*180;
      lastRead = currRead;
      servo.write(angle);
      tone(BUZZER_PIN, ADC);
    
    }
    
    if(bootButton.CheckButtonPress()) //if the button was pressed, switch to IDLE
    {
      // Notify us that we're switching to IDLE
      Serial.println("Button press -> IDLE");

      servo.detach();
      // Finally, update the state
      state = SYS_IDLE;
      noTone(BUZZER_PIN);
      //digitalWrite(LED_PIN, LOW);
    }
  }
}