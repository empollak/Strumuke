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
    // Only move if we aren't already homed at the edge
    if (digitalRead(limitPin == HIGH)) {
        while(digitalRead(limitPin) == HIGH) {
            step(1, 1); // I am just guessing on which direction is which
            if (Serial.available()>0) {
                Serial.println("stopped");
                while(true) {}
            }
            delay(1);
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
    this->side = !side;
}