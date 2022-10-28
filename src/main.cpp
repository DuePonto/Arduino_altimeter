#include <Arduino.h>

#include "Adafruit_BMP085.h"
#include "EEPROM.h"

Adafruit_BMP085 bmp;

uint8_t cntr_altitude;
float old_altitude;
float real_altitude;
float intresting_point;
float result;
float read_result;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (!bmp.begin()) {
	  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	  while (1) {}
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  old_altitude = real_altitude;
  real_altitude = bmp.readAltitude();

  if(real_altitude < old_altitude && cntr_altitude == 0 && real_altitude > 10){
    intresting_point = old_altitude;
    cntr_altitude ++;
  } else if (intresting_point > real_altitude){
    cntr_altitude ++;
    if (cntr_altitude > 10){
      result = intresting_point;
      EEPROM.put(0, result);
      intresting_point = real_altitude;
      cntr_altitude = 0;
    }
  }
  else{
    cntr_altitude = 0;
  }

  Serial.print("result altitude = ");
  Serial.print(result);
  Serial.print(" m");

  Serial.print("  intresting_p = ");
  Serial.print(intresting_point);
  Serial.print(" m");

  EEPROM.get(0, read_result);

  Serial.print("  result altitude = ");
  Serial.print(read_result);
  Serial.print(" m");

  Serial.print("  result altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" m");

  delay(100);
}