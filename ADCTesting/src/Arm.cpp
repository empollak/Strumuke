#include "Arm.h"
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_ADS1X15.h>

extern Adafruit_ADS1115 ads1;
extern Adafruit_ADS1115 ads2;
extern Adafruit_PWMServoDriver pwm;
extern int knownSafe[3][3];

Arm::Arm(int num) {
    this->num = num;

    if (this->num == 1) {
        dir0 = 1;
        dir1 = 1;
        dir2 = -1;
    }
    else if (this->num == 2) {
        dir0 = -1;
        dir1 = 1;
        dir2 = -1;
    }
    else if (this->num == 3) {
        dir0 = -1;
        dir1 = -1;
        dir2 = 1;
    }
}

Arm::Arm(int num, int neutral[3], int neutralPot[3], int servoData[4][4][3]) {
    this->num = num;
    for (int servo = 0; servo < 3; servo++) {
        for (int string = 0; string < 4; string++) {
            for (int fret = 0; fret < 4; fret++) {
                this->noteData[string][fret][servo] = servoData[string][fret][servo];
            }
        }

        this->neutral[servo] = neutral[servo];
        this->neutralPot[servo] = neutralPot[servo];
    } 
}

void Arm::print() {
    // Arm arm1 = Arm(1, 
    Serial.print("Arm arm" + String(this->num) + " = Arm(" + String(this->num) + ", ");
    // {neutral0, neutral1, neutral2}, 
    Serial.print("(const int[3]) {" + String(neutral[0]) + ", " + String(neutral[1]) + ", " + String(neutral[2]) + "}, ");
    // {neutralPot0, neutralPot1, neutralPot2}, 
    Serial.print("(const int[3]) {" + String(neutralPot[0]) + ", " + String(neutralPot[1]) + ", " + String(neutralPot[2]) + "}, ");
    // {{{noteData[0][0][0], noteData[0][0][1], ...}, {noteData[0][1][0]}}}
    Serial.print("(const int[4][4][3]) {");
    for (int string = 0; string < 4; string++) {
        Serial.print("{");
        for (int fret = 0; fret < 4; fret++) {
            Serial.print("{");
            for (int servo = 0; servo < 3; servo++) {
                Serial.print(String(noteData[string][fret][servo]));
                if (servo < 2) Serial.print(", ");
            }
            if (fret < 3) Serial.print("}, ");
            else Serial.print("}");
        }
        if (string < 3) Serial.print("}, ");
        else Serial.print("}");
    }
    Serial.println("});");
}


void Arm::moveTo(int* servoData) {
    pwm.setPWM(0 + (this->num - 1)*3, 0, servoData[0]);
    pwm.setPWM(1 + (this->num - 1)*3, 0, servoData[1]);
    pwm.setPWM(2 + (this->num - 1)*3, 0, servoData[2]);
}

void Arm::calibrateForChord(int* servoData) {
    int arm = this->num;
    pwm.setPWM(0 + ((arm - 1) * 3), 0, knownSafe[arm-1][0]);
    pwm.setPWM(1 + ((arm - 1) * 3), 0, knownSafe[arm-1][1]);
    pwm.setPWM(2 + ((arm - 1) * 3), 0, knownSafe[arm-1][2]);

    delay(300);

    if (arm == 1) servoData[2] = knownSafe[0][2];
    else servoData[2] = calibrateServo(2, servoData[2], knownSafe[arm-1][2]);
    servoData[0] = calibrateServo(0, servoData[0], knownSafe[arm-1][0]);
    servoData[1] = calibrateServo(1, servoData[1], knownSafe[arm-1][1]);
}

int Arm::calibrateServo(int servo, int goal, int start) {
    int arm = this->num;
    int pulselen;
    int error = 51;
    int lastPulselen = start;
    int allowableDelta = 50;
    if (arm == 3) allowableDelta = 2;
    while (abs(error) > allowableDelta) {
        int servoPos = this->getServo(servo);
        error = goal - servoPos;
        if (arm == 3) {
        pulselen = lastPulselen + error/3;
        }
        else {
        pulselen = lastPulselen + map(error, -5000, 5000, -100, 100);
        }
        lastPulselen = pulselen;
        pwm.setPWM(servo + ((arm - 1) * 3), 0, pulselen);

        Serial.println("Calibrating arm " + String(arm) + " servo " + String(servo) + "\t" + "servoPos " + String(servoPos) + "\t" + "goal " + String(goal) + "\t" + "error "+ String(error) + "\t" + "pulselen " + String(pulselen) + "\t");
        
        delay(200);
        servoPos = this->getServo(servo);
        error = goal - servoPos;
    }

    return pulselen;
}

int Arm::getServo(int servo) {
    int arm = this->num;
    switch (arm) {
        case 1:
        switch (servo) {
            case 0:
            return ads1.readADC_SingleEnded(0);
            break;

            case 1:
            return ads1.readADC_SingleEnded(1);
            break;

            default:
            return ads1.readADC_SingleEnded(2);
            break;
        }
        break;

        case 2:
        switch (servo) {
            case 0:
            return ads2.readADC_SingleEnded(0);
            break;

            case 1:
            return ads2.readADC_SingleEnded(1);
            break;

            default:
            return ads2.readADC_SingleEnded(2);
            break;
        }

        case 3:
        switch (servo) {
            case 0:
            return analogRead(17);
            break;

            case 1:
            return analogRead(16);
            break;

            case 2:
            return analogRead(15);
            break;
        }
    }
}

void Arm::jogArm(int* servoData) {
    bool stop = false;
    int offset0 = 0;
    int offset1 = 0;
    int offset2 = 0;
    int delta = 4;

    while (!stop) {
        pwm.setPWM(0 + (this->num-1)*3, 0, servoData[0] + offset0);
        pwm.setPWM(1 + (this->num-1)*3, 0, servoData[1] + offset1);
        pwm.setPWM(2 + (this->num-1)*3, 0, servoData[2] + offset2);

        while(Serial.available() == 0) {}
        String input = Serial.readString();
        if (input.equals("u")) {
            offset0 += dir0*delta;
            Serial.println(offset0);
        } else if (input.equals("j")) {
            offset0 -= dir0*delta;
            Serial.println(offset0);
        } else if (input.equals("i")) {
            offset1 += dir1*delta;
            Serial.println(offset1);
        } else if (input.equals("k")) {
            offset1 -= dir1*delta;
            Serial.println(offset1);
        } else if (input.equals("o")) {
            offset2 += dir2*delta;
            Serial.println(offset2);
        } else if (input.equals("l")) {
            offset2 -= dir2*delta; 
            Serial.println(offset2);
        } else if (input.equals("h")) {
            stop = true;
        }
    }

  servoData[0] += offset0;
  servoData[1] += offset1;
  servoData[2] += offset2;
}