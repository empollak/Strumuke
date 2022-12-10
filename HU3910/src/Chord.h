#ifndef Chord_h
#define Chord_h

#include <Arduino.h>
#include "ServoPositions.h"

class Chord {
    public:
        ServoPositions *servoPosArm1;
        ServoPositions *servoPosArm2;
        ServoPositions *servoPosArm3;

        Chord (ServoPositions *servoPosArm1, ServoPositions *servoPosArm2, ServoPositions *servoPosArm3);
};

#endif