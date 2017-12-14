#ifndef DrumPart_h
#define DrumPart_h

#include "Arduino.h"

class DrumPart
{
  public:
    DrumPart(int _location);

    void position();
    void velocity();
    void trigger();

    int location;
    bool boolRead;

    private:
};

#endif

