/*
This code integrates the CBETA board with GPS and XBEE (read from Software Serial).

Code functions as a whole, but we were unable to read GPS data and send, there may be a fault in updating.

Code needs to be corrected for GPS read and then cleaned up and annotated.
*/ 

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <LiquidTWI.h>
#include <Wire.h>
#include <SdFat.h>

LiquidTWI lcd(0);
SdFat sd;
SdFile myFile;

SoftwareSerial mySerial(5, 4);

Adafruit_GPS GPS(&mySerial);

#define GPSECHO  true

boolean usingInterrupt = false;

const int but1 = 0; 
const int but2 = 20;
const int but3 = 18;
const int but4 = 19; 
const int chipSelect = 4;

void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy
  
void setup() {
  pinMode(but1, INPUT_PULLUP);
  pinMode(but2, INPUT_PULLUP);
  pinMode(but3, INPUT_PULLUP);
  pinMode(but4, INPUT_PULLUP);
  lcd.begin(16,2);
  lcd.setBacklight(HIGH);
  lcd.clear();
  Serial.begin(9600);
  Serial.println("Adafruit GPS library basic test!");
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  useInterrupt(true);
  
  delay(1000);
  mySerial.println(PMTK_Q_RELEASE);
}

  SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  
  #ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
  #endif
}


void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

uint32_t timer = millis();

void loop()                     // run over and over again
{
  // in case you are not using the interrupt above, you'll
  // need to 'hand query' the GPS, not suggested :(
  if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c) Serial.print(c);
  }
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
  
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }
  byte butPress = checkButtons();
  if (butPress != 99) {
    unsigned long startTime = millis();
    while (digitalRead(butPress) == LOW) {
      
    }
    unsigned long endTime = millis();
    int timePress = endTime - startTime;
    shortReport();
    Serial.print("Button: ");
    Serial.print(butPress);
    Serial.print(",");
    Serial.print("Data: ");
    Serial.print(timePress);
    Serial.print(",");
    lcdUpdate();
  }
}

byte checkButtons() { // checks for button press and executes action
  if(digitalRead(but1) == LOW) {
    lcd.clear();
    delay(300);
    return but1;
  }
  else if(digitalRead(but2) == LOW) {
    lcd.clear();
    delay(300);
    return but2;
  }
  else if(digitalRead(but3) == LOW) {
    lcd.clear();
    delay(300);
    return but3;
  }
  else if(digitalRead(but4) == LOW) {
    lcd.clear();
    delay(300);
    return but4;
  }
  else {
    return 99;
  }
}


void lcdUpdate() {
  lcd.setCursor(0,0);
  lcd.print(GPS.latitude, 4);
  lcd.setCursor(0,1);
  lcd.print(GPS.longitude, 4);
}

void shortReport() {
  Serial.print("Time: ");
  Serial.print(GPS.hour, DEC); Serial.print(':');
  Serial.print(GPS.minute, DEC); Serial.print(':');
  Serial.print(GPS.seconds, DEC);
  Serial.print(",");
  Serial.print("Date: ");
  Serial.print(GPS.day, DEC); Serial.print('/');
  Serial.print(GPS.month, DEC); Serial.print("/20");
  Serial.print(GPS.year, DEC);
  Serial.print(",");
  Serial.print("Lat: ");
  Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
  Serial.print(",");
  Serial.print("Lon: "); 
  Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
}

void reportLocation() {
  Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
    }
}
