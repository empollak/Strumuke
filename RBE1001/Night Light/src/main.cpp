#include <Arduino.h>
#include <RBE1001Lib.h>
#include <button.h>
#include <iostream>
#include <string>


const int LED_PIN = 33; // declare the pin for the LED
const int INPUT_PIN = 35; // declare the pin for the input from the pot

unsigned int factorial(int n){
  unsigned int value = 1;
  for(int i = 1; i<=n; i++){
    value = value * i;
  }
  return value;
}

// Create a button object for the built-in button on the ESP32
Button bootButton(BOOT_FLAG_PIN);

// Define two basic states. For this program, they will correspond to an LED state (on or off).
// "enum" stands for "enumerate". Basically, we define a new variable type called ROBOT_STATE.
// We prepend "ROBOT_" to everything to avoid conflicts with other variables that may be defined elsewhere.
enum SYSTEM_STATE {SYS_IDLE, SYS_ACTIVE};

// Declare a variable, robotState, of our new type, ROBOT_STATE. Initialize it to ROBOT_IDLE.
SYSTEM_STATE state = SYS_ACTIVE;

// Calculate voltage from an ADC reading
float voltage(int ADC){
  return ADC/4096.0*3.3;
} 

/*
 * This is the standard setup function that is called when the ESP32 is rebooted.
 * It is used to initialize anything that needs to be done once.
 */
void setup() 
{
  std::cout << voltage(2048) << std::endl;

  // This will initialize the Serial at a baud rate of 115200 for prints
  // Be sure to set your Serial Monitor appropriately in platformio.ini
  Serial.begin(115200);

  // Set the LED pin to be an OUTPUT
  pinMode(LED_PIN, OUTPUT);

  // Set the input pin to be an INPUT
  pinMode(INPUT_PIN, INPUT);
  
  // Initialize the button object
  bootButton.Init();

}

/* Here is where all the fun happens. For each state, check for and respond to a button press.
 */ 
void loop()
{
  // Go through the state machine
  if(state == SYS_IDLE)
  {
    if(bootButton.CheckButtonPress()) //if the button was pressed, switch to ACTIVE
    {
      // Notify us that we're switching to ACTIVE
      Serial.println("Button press -> ACTIVE");

      // Update the state
      state = SYS_ACTIVE;

      digitalWrite(LED_PIN, HIGH);
    }
  }

  //note that we use else..if for each additional state, so it doesn't get confused
  else if(state == SYS_ACTIVE)
  {
    // Every 200 milliseconds, print the value read at INPUT_PIN
    static unsigned long lastRead = 0; // Last millis() the pot value was read
    unsigned long currRead = millis(); 
    float ADC = analogRead(INPUT_PIN);
    float voltage = (ADC/4096.0)*3.3; // ADC converted into voltage
    char lightLevel; // Store whether it's DARK or LIGHT
    // Turn on the LED if the input voltage is high
    /*if (voltage > 3.3/2.0) {
      digitalWrite(LED_PIN, HIGH);
      lightLevel = 'D';
    } else {
      digitalWrite(LED_PIN, LOW); 
      lightLevel = 'L';
    }*/


    if(currRead - lastRead > 200)
    {

      lastRead = currRead;
      Serial.print(ADC);
      Serial.print('\t');
      Serial.print(voltage);
      Serial.print('\t');
      Serial.print(lightLevel);
      Serial.print('\n');
      
    }
    
    if(bootButton.CheckButtonPress()) //if the button was pressed, switch to IDLE
    {
      // Notify us that we're switching to IDLE
      Serial.println("Button press -> IDLE");

      // Finally, update the state
      state = SYS_IDLE;

      digitalWrite(LED_PIN, LOW);
    }
  }
}

