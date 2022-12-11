#ifndef Chord_h
#define Chord_h

class Arm {
    private:
        int num;

    public:
        Arm(int num);
        void moveTo(int* servoData);
        int neutral[3];
        int neutralPot[3];
        // Top level array is strings (0=A), next down is fret
        int noteData[4][4][3];
};

#endif