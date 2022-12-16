#include "CustomStepper.h"
#include <Arduino.h>

CustomStepper::CustomStepper(int stepPin, int dirPin, int limitPin, int pulseLength, int microsBetweenSteps) {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(limitPin, INPUT);
    digitalWrite(stepPin, LOW);
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->limitPin = limitPin;
    this->pulseLength = pulseLength;
    this->microsBetweenSteps = microsBetweenSteps;
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

CustomStepper::CustomStepper() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(limitPin, INPUT);
    digitalWrite(stepPin, LOW);
}

void CustomStepper::step(int steps, int dir) {
    int stepped = 0;
    digitalWrite(dirPin, dir);
    while (stepped < steps) {
        digitalWrite(this->stepPin, HIGH);
        delayMicroseconds(this->pulseLength);
        digitalWrite(this->stepPin, LOW);
        delayMicroseconds(this->microsBetweenSteps);
        stepped++;
    }
}

void CustomStepper::home() {
    // limitPin goes LOW when pressed down
    // Only move if we aren't already homed at the edgeq
    if (digitalRead(limitPin == HIGH)) {
        while(digitalRead(limitPin) == HIGH) {
            step(2, 1); 
            delayMicroseconds(1000);
        }
        step(5, 1);
    }
    Serial.println("homed");
    this->side = 1;
}

void CustomStepper::stepAcross() {
    if (this->side == -1) {
        Serial.println("Can't step across without being homed!");
        return;
    }
    step(this->stepsAcross, !side);
    if (this->side == 0) {
        home();
    } else this->side = !side;
    Serial.println("side " + String(side));
}