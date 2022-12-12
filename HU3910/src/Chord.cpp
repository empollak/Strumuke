#include "Chord.h"

Chord::Chord (int arm1Pos[3], int arm2Pos[3], int arm3Pos[3]) {
    // this->servoPosArm1 = servoPosArm1;
    // this->servoPosArm2 = servoPosArm2;
    // this->servoPosArm3 = servoPosArm3;
    for (int i = 0; i < 3; i++) {
        this->arm1Pos[i] = arm1Pos[i];
        this->arm2Pos[i] = arm2Pos[i];
        this->arm3Pos[i] = arm3Pos[i];
    }
}