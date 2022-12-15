#include "CustomStepper.h"
#include <Arduino.h>

CustomStepper::CustomStepper(int stepPin, int dirPin, int limitPin, int pulseLength, int millisBetweenSteps) {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(limitPin, INPUT);
    digitalWrite(stepPin, LOW);
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->limitPin = limitPin;
    this->pulseLength = pulseLength;
    this->millisBetweenSteps = millisBetweenSteps;
}

CustomStepper::CustomStepper(int stepPin, int dirPin, int limitPin) {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(limitPin, INPUT);
    digitalWrite(stepPin, LOW);
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->limitPin = limitPin;
}

void CustomStepper::step(int steps, int dir) {
    // dir 1 is towards the limit switch
    int stepped = 0;
    digitalWrite(dirPin, dir);
    while (stepped < steps) {
        digitalWrite(this->stepPin, HIGH);
        delayMicroseconds(this->pulseLength);
        digitalWrite(this->stepPin, LOW);
        delay(this->millisBetweenSteps);
        stepped++;
    }
}

void CustomStepper::home() {
    // Figuring HIGH on the limitPin means it's pressed down
    while(digitalRead(limitPin) == LOW) {
        step(1, 1);
    }
    this->side = 1;
}

void CustomStepper::stepAcross() {
    if (this->side == -1) {
        Serial.println("Can't step across without being homed!");
        return;
    }
    // dir 1 is towards side 1, and we want to go away from the side we're on
    step(this->stepsAcross, !side); 
    this->side = !side;
}