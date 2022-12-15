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
        step(1, 0); // I am just guessing on which direction is which
    }
    this->side = 0;
}

void CustomStepper::stepAcross() {
    if (this->side == -1) {
        Serial.println("Can't step across without being homed!");
        return;
    }
    step(this->stepsAcross, side); // Still just guessing on which direction is which
    this->side = !side;
}