#include "Play.h"

Play::Play (Chord &chord, int duration, int strumDirection) : chord(chord), duration(duration), strumDirection(strumDirection){
      this->chord = chord;
      this->duration = duration; 
      this->strumDirection = strumDirection;
}