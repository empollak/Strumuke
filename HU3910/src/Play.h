#ifndef Play_h
#define Play_h

#include <Arduino.h>
#include "Chord.h"


class Play {
    public:
        Chord &chord;
        int duration;
        int strumDirection;
        Play(Chord &chord, int duration, int strumDirection);
};

#endif