/*
 * Thermostat.cpp
 *
 *  Created on: Jan 25, 2020
 *      Author: jgaluska
 */

#include "Thermostat.h"


Thermostat::Thermostat() {

	// Configure the pinMode
	pinMode(PIN_HEATER_RELE, OUTPUT);
	enable_termostat(false);

	// Initialize the temperature sensor
#ifndef DEBUG_TEMP_SENSOR
	tempSensor = Adafruit_SHT31();
#else
    tempSensor = sht30_test();
#endif

    tempSensor.begin(SHT30_ADDR);

    target_temp_10th_c = EEPROM_read_target_temp();
    if (target_temp_10th_c > 300 || target_temp_10th_c <= 0)
    {  // Not plausible temperature
#ifndef DEBUG
    	target_temp_10th_c = 17*10;
    	EEPROM_write_target_temp(target_temp_10th_c);
#endif /* DEBUG */
    }

    day_consuption_100thW = 0;
    electricity_price_10th_CZK = EEPROM.read(ADDR_PRICE_ELECTRICITY);
}

Thermostat::~Thermostat() {
	// TODO Auto-generated destructor stub
}

void Thermostat::enable_termostat(bool v, bool mobile = false)
{
	if (v)
	{
		termostat_on = true;
		enabled_by_mobile = mobile;
	}
	else
	{
		enable_heater(false);
        termostat_on = false;
	}

}

void Thermostat::set_electricity_price_10th_CZK(uint8_t price)
{
    EEPROM.update(ADDR_PRICE_ELECTRICITY, price);
}

void Thermostat::update(void)
{
    get_temperature_10th_c();
    get_rel_humidity();

	if (termostat_on)
	{
		if ( (inside_temp_10th_c < (get_target_temperature_10th_c()- DEBOUNCING_10th_C)) && !heater_on )
		{
			enable_heater(true);
		}
		else if ( heater_on && ((inside_temp_10th_c - DEBOUNCING_10th_C) > get_target_temperature_10th_c()) )
		{
			enable_heater(false);
		}
	}
}


int16_t Thermostat::get_temperature_10th_c(void)
{
	float temp_fl = tempSensor.readTemperature();
	if (temp_fl > MAX_TEMP)
		temp_fl = (float)  MAX_TEMP;
	else if (temp_fl < MIN_TEMP)
		temp_fl = (float) MIN_TEMP;
	inside_temp_10th_c = (int16_t)(tempSensor.readTemperature() * 10); // TODO: Handle properly the comas!
	return inside_temp_10th_c;
}


int8_t Thermostat::get_rel_humidity(void)
{
    float humid_fl = tempSensor.readHumidity();
    if( humid_fl > 100.0)    // Sanity check, but should not happen
    	humid_fl = 100.0;
    else if (humid_fl < 0.0)
    	humid_fl = 0;

    inside_humid_rel = (uint8_t) humid_fl;
    return inside_humid_rel;
}

void Thermostat::enable_heater(bool v)
{
	uint32_t timestamp_now = millis();
	if (heater_on && v == false)
	{ //Disable heater
		heater_on = false;
		digitalWrite(PIN_HEATER_RELE,LOW);
		uint32_t tmp_s;
		if (timestamp_now > heater_timestamp_ms)
			tmp_s = (timestamp_now - heater_timestamp_ms) / 1000;
		else // Timestamp overflow!!
			tmp_s = (MAX_UINT32 - heater_timestamp_ms) + timestamp_now;

		tmp_s = (tmp_s * HEATER_POWER_W) / (3600 * 10);
		day_consuption_100thW +=(uint16_t) tmp_s;
	}
	else if(!heater_on && v == true)
	{ // Enable heater
        heater_timestamp_ms = timestamp_now;
        heater_on = true;
        digitalWrite(PIN_HEATER_RELE,HIGH);
	}
}

void Thermostat::EEPROM_write_target_temp(int16_t temp)
{
	int8_t n;
	n = (int8_t) (temp >> 8 ) & 0xFFU; // Most significant part
	EEPROM.write(ADDR_TARGET_TEMP_2B,n);
	n = (int8_t) temp & 0xFFU;         // Least significant part
	EEPROM.write(ADDR_TARGET_TEMP_2B + 1U, n);
}

int16_t Thermostat::EEPROM_read_target_temp(void)
{
	int16_t n;
	n = (
		(( (int16_t)EEPROM.read(ADDR_TARGET_TEMP_2B) & 0x00FF ) << 8) |
	    (  (int16_t)EEPROM.read(ADDR_TARGET_TEMP_2B +1U) & 0x00FF )
	);
	return n;
}


