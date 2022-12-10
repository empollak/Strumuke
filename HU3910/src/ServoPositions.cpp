#include "ServoPositions.h"

ServoPositions::ServoPositions (float base, float joint1, float joint2) {
    this->base = base;
    this->joint1 = joint1;
    this->joint2 = joint2;
}

ServoPositions::ServoPositions () {}