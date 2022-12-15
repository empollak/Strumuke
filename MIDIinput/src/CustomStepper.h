#ifndef CustomStepper_h
#define CustomStepper_h

class CustomStepper {
    private:
        int stepsAcross = 150;
        int side = -1; // 1 is at limit switch/arduino, 0 is away from limit switch/arduino, -1 is unzeroed
        int stepPin;
        int dirPin;
        int limitPin;
        int pulseLength = 20;
        int millisBetweenSteps = 2;
    
    public:
        CustomStepper(int stepPin, int dirPin, int limitPin, int pulseLength, int millisBetweenSteps);
        CustomStepper(int stepPin, int dirPin, int limitPin);
        void home();
        void stepAcross();
        // dir 1 is towards the limit switch
        void step(int steps, int dir);
};


#endif