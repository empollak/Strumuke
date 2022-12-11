#include "Arm.h"
#include "Global.h"

Arm::Arm(int num) {
    this->num = num;
}

void Arm::moveTo(int* servoData) {
    pwm.setPWM(0 + (this->num - 1 )*3, 0, servoData[0]);
    pwm.setPWM(1 + (this->num - 1)*3, 0, servoData[1]);
    pwm.setPWM(2 + (this->num - 1)*3, 0, servoData[2]);
}