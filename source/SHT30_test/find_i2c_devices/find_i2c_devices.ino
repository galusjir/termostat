#include "Wire.h"
extern "C" { 
#include "utility/twi.h"  
}

void scanI2CBus(byte from_addr, byte to_addr, void(*callback)(byte address, byte result) ) 
{
  byte rc;
  byte data = 0;
  for( byte addr = from_addr; addr <= to_addr; addr++ ) {
    rc = twi_writeTo(addr, &data, 0, 1, 0);
    callback( addr, rc );
  }
}

void scanFunc( byte addr, byte result ) {
  Serial.print("addr: ");
  Serial.print(addr,HEX);
  Serial.print( (result==0) ? " found!":"       ");
  Serial.print( (addr%4) ? "t":"n");
  Serial.print("\n");
}

byte start_address = 8;       
byte end_address = 119;       

void setup()
{
    Wire.begin();
    Serial.begin(9600);                
    Serial.println("nI2CScanner ready!");

    Serial.print("starting scanning of I2C bus from ");
    Serial.print(start_address,DEC);
    Serial.print(" to ");
    Serial.print(end_address,DEC);
    Serial.println("...");

    scanI2CBus( start_address, end_address, scanFunc );

    Serial.println("ndone");
    pinMode(13, OUTPUT);
}

void loop() 
{
    digitalWrite(13,HIGH);
    delay(300);
    digitalWrite(13,LOW);
    delay(300);
}
