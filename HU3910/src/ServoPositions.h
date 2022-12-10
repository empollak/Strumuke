#ifndef ServoPositions_h
#define ServoPositions_h

class ServoPositions {
    public:
        float base;
        float joint1;
        float joint2;

        ServoPositions (float base, float joint1, float joint2);

        ServoPositions ();
};

#endif