// ClimateTracker Library CPP file

//Reading in libraries
#include "EEPROM.h"
#include "Arduino.h"

void CTClass::incrEEPROM(int addr)
{
byte a = EEPROM.read(addr);
a++;
EEPROM.write(addr,a);
}

CTClass CT;
