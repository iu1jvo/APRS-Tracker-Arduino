#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <LibAPRS.h>

// The TinyGPS++ object
TinyGPSPlus gps;

SoftwareSerial ss(PIN_SPI_MISO, PIN_SPI_MOSI);


// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
    {
      char c = ss.read();
      //Serial.write(c);
      gps.encode(c);
    }
      
  } while (millis() - start < ms);
}


void setup() {
  // put your setup code here, to run once:
  ss.begin(9600);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (gps.location.isValid())
  {
    digitalWrite(8,1);
    char buf[50];
    char strLat[14];
    char strLng[14];
    char strAge[12];
    dtostrf(gps.location.lat(),2,8,strLat);
    dtostrf(gps.location.lng(),2,8,strLng);
    itoa(gps.location.age(),strAge,0);

    sprintf(buf,"Lat=%s, Lon=%s, Age=%s", strLat , strLng, strAge);
    Serial.println(buf);
  }
  else
  {
    digitalWrite(8,0);
    Serial.print("----->Waiting Data....");
    Serial.println(gps.satellites.value());
  }

  smartDelay(1000);
}



