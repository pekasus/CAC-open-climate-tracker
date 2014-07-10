// Climate Tracker Library

#ifndef CT_h
#define CT_h

#include <inttypes.h>
#include <Arduino.h>
//Professor Martin: Which do we include? Do we need inttypes? What is it? Is it uint8? But if we include Arduino we can just its
//Byte right?
//Is this C++ or C construct?

class CTClass
{
  public:
    void incrEEPROM(int);
    
  //Review private classes with Professor Martin. When to use. Conflicts with variables.
};

extern CTClass CT;

#endif
