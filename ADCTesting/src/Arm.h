#ifndef Arm_h
#define Arm_h

class Arm {
    private:
        int dir0;
        int dir1;
        int dir2;

    public:
        int num;
        Arm(int num);
        Arm(int num, int* neutral, int* neutralPot, int servoData[4][4][3]);
        void moveTo(int* servoData);
        void jogArm(int* servoData);
        void calibrateForChord(int* servoData);
        int getServo(int servo);
        int calibrateServo(int servo, int goal, int start);
        void print();
        int neutral[3];
        int neutralPot[3];
        // Depth of array goes String->Fret->Servo
        int noteData[4][4][3];
};

#endif