#ifndef Chord_h
#define Chord_h

#include <Arduino.h>
#include "ServoPositions.h"

class Chord {
    public:
        // ServoPositions *servoPosArm1;
        // ServoPositions *servoPosArm2;
        // ServoPositions *servoPosArm3;

        int arm1Pos[3];
        int arm2Pos[3];
        int arm3Pos[3];

        Chord (int arm1Pos[3], int arm2Pos[3], int arm3Pos[3]);
        Chord ();
};

#endif