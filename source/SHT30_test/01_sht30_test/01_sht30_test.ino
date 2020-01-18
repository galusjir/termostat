
#include <Wire.h>
#include <Arduino.h>
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht30 = Adafruit_SHT31();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial)
      delay(10);
  Serial.println("SHT30 testing");
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
