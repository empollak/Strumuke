#ifndef Chord_h
#define Chord_h

#include <Arduino.h>
#include "ServoPositions.h"

class Chord {
    public:

        int arm1Pos[3];
        int arm2Pos[3];
        int arm3Pos[3];

        Chord (int arm1Pos[3], int arm2Pos[3], int arm3Pos[3]);
};

#endif