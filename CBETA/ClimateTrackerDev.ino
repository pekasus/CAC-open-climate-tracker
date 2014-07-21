#include <Adafruit_GPS.h>
#include <SoftwareSerial.h> // must include du to GPS library
#include "DHT.h"
#define DHTPIN 23
#include <Wire.h>
#include <LiquidTWI2.h>


//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
Adafruit_GPS GPS(&Serial1);
#define GPSECHO  false // set to true to listen to raw GPS data (debug)

boolean usingInterrupt = false;

LiquidTWI2 lcd(0);

void setup() {
  Serial.begin(115200);
  Serial.println("CBETA is powered up.");
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY); // alt to line above
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // update rate 1 Hz
  //GPS.sendCommand(PGCMD_ANTENNA); // uncomment for updates on antenna status
  useInterrupt(true); // sets data read on a timer
  dht.begin();
  delay(1000);
  lcd.begin(16,2);
lcd.clear();
lcd.print("All Good!");
}



uint32_t timer = millis();

void loop() {
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

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) { 
    timer = millis(); // reset the timer
//    if (GPS.fix) {
//      Serial.print("\nTime: ");
      Serial.print(GPS.hour, DEC); Serial.print(':');
      Serial.print(GPS.minute, DEC); Serial.print(':');
      Serial.print(GPS.seconds, DEC); Serial.print(',');
//      Serial.print("Date: ");
//      Serial.print(GPS.day, DEC); Serial.print('/');
//      Serial.print(GPS.month, DEC); Serial.print("/20");
//      Serial.print(GPS.year, DEC); Serial.print(',');
//      Serial.print("Fix: "); Serial.print((int)GPS.fix);
//      Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
//      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); //Serial.print(GPS.lat);
      Serial.print(','); 
      lcd.clear();
      lcd.print("Latd: ");
      lcd.print(GPS.latitude, 4);
      Serial.print(GPS.longitude, 4); //Serial.println(GPS.lon);
      Serial.print(',');
      lcd.setCursor(0,1);
      lcd.print("LDECong:  ");
      lcd.print(GPS.longitude, 4);
//      Serial.print("Speed (knots): "); 
      Serial.print(GPS.speed);
//      Serial.print("Angle: ");
      Serial.print(','); 
      Serial.print(GPS.angle);
      Serial.print(',');
//      Serial.print("Altitude: "); 
      Serial.print(GPS.altitude);
      Serial.print(',');
//      Serial.print("Satellites: "); Serial.print((int)GPS.satellites);
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      if (isnan(t) || isnan(h)) {
      Serial.println("Failed to read from DHT");
      } 
      else {
//      Serial.print("Humidity: "); 
      Serial.print(h);
      Serial.print(" %");
      Serial.print(',');
//      Serial.print("Temperature: "); 
      Serial.print(t);
      Serial.println(" *C");
      }
//    }
  }
}


// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
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
