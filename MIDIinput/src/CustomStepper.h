#ifndef CustomStepper_h
#define CustomStepper_h

class CustomStepper {
    private:
        int stepsAcross = 169;
        int side = -1; // 1 is at limit switch/arduino, 0 is away from limit switch/arduino, -1 is unzeroed
        int stepPin = 9;
        int dirPin = 8;
        int limitPin = 13;
        int pulseLength = 20;
        int microsBetweenSteps = 1500;
    
    public:
        CustomStepper(int stepPin, int dirPin, int limitPin, int pulseLength, int microsBetweenSteps);
        CustomStepper(int stepPin, int dirPin, int limitPin);
        CustomStepper();
        void home();
        void stepAcross();
        // dir 1 is towards the limit switch
        void step(int steps, int dir);
};


#endif