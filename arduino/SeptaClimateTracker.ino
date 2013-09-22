#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <SD.h>
//#include <DHT.h>

#define GPSECHO  false
//#define DHTPIN 7
//#define DHTTYPE DHT22

SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);

//DHT dht(DHTPIN, DHTTYPE);
//File myFile;
const int chipSelect = 10;
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

void setup()  
{
  
  Serial.begin(115200);
  Serial.println("Septa Climate Tracker");
  pinMode(10, OUTPUT);
  pinMode(4, OUTPUT);

//  dht.begin();
  
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  useInterrupt(true);
  delay(1000);
  Serial.println("Setup complete.");
  SD.begin();
  delay(2000);
  if (!SD.begin(chipSelect)) {
    Serial.println("PC Load Letter? WTF does that mean?");
    return;
  } 
  Serial.println("--------------------------");
  Serial.println("Commencing Nuclear Launch Sequence.");
  Serial.println("--------------------------");
  
} // closes void setup

/*--------------------------------------------------------------*/

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

/*--------------------------------------------------------------*/

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

/*--------------------------------------------------------------*/

uint32_t timer = millis();

void loop() 
{
  
 // float h = dht.readHumidity();
 // float t = dht.readTemperature();
  
  if (! usingInterrupt) {
    char c = GPS.read();
    if (GPSECHO)
      if (c) Serial.print(c);
  }
  
  if (GPS.newNMEAreceived()) {
    
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  if (timer > millis())  timer = millis();

  if (millis() - timer > 2000) 
  { 
    timer = millis(); // reset the timer
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Location: ");
    Serial.print(GPS.latitude, 4);
    Serial.print(", "); 
    Serial.println(GPS.longitude, 4); 
    Serial.print("Speed (knots): "); Serial.println(GPS.speed);
 //   Serial.print(t); Serial.println("*C");
 //   Serial.print(h); Serial.println("%");
    
  File myFile = SD.open("TempLocA.txt" , FILE_WRITE);
    delay(200);
  if (myFile) {
//    myFile.print("Start - "); myFile.print(",");
    myFile.print(GPS.latitude, 4);
    myFile.print(",");
    myFile.print(GPS.longitude, 4);
    myFile.print(",");
    myFile.print(GPS.hour, DEC);
    myFile.print(":");
    myFile.print(GPS.minute, DEC);
    myFile.print(":");
    myFile.print(GPS.seconds, DEC);
    myFile.print(",");
    myFile.print(GPS.day, DEC);
    myFile.print("-");
    myFile.print(GPS.month, DEC);
    myFile.print("-");
    myFile.print(GPS.year, DEC);
    myFile.print(",");
    myFile.print(GPS.speed);
/*    myFile.print(h);
    myFile.print(",");
    myFile.println(t);
    myFile.print(","); 
    myFile.print(" - End");*/
    myFile.close(); 
   }
   
   else {
     Serial.println("file not open");
   }
    
    delay(3000);
    
  }
}










