#include "Chord.h"

Chord::Chord (ServoPositions *servoPosArm1, ServoPositions *servoPosArm2, ServoPositions *servoPosArm3) {
    this->servoPosArm1 = servoPosArm1;
    this->servoPosArm2 = servoPosArm2;
    this->servoPosArm3 = servoPosArm3;
}