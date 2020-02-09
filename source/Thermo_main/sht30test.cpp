/*
 * sht30test.cpp
 *
 *  Created on: Jan 26, 2020
 *      Author: jgaluska
 */

#include "sht30test.h"
#include <Arduino.h>



sht30_test::sht30_test() {

	humidity = 40.0f;
	min_temp = -20.0f;
	max_temp = 50.0f;
	temperature = min_temp;
	increasing = 1;
	counter = 0;
}

float sht30_test::readTemperature(void)
{
#ifdef SERIAL_DEBUG_1
	Serial.println("TEMP\n");
	for(int i; i< 254; i++)
	{
        if(Serial.available() > 0)
        {
        	int incoming_byte = Serial.read();
        	Serial.println("OK\n");
        	return (float) incoming_byte;
        }
        delay(4);
	}
	return 20.0f;
#endif

	if (counter == 4)
	{
		if(increasing)
		{
			temperature += 1.0f;
			if (temperature > max_temp)
			    increasing = false;
		}
		else
		{
			temperature -= 1.0f;
			if (temperature < min_temp)
			    increasing = true;
		}
		Serial.println("\nT:");
		Serial.println((float)temperature,1);
	}
    counter++;
    return temperature;
}

float sht30_test::readHumidity(void)
{
    return 50.0f;
}

sht30_test::~sht30_test() {
	// TODO Auto-generated destructor stub
}

