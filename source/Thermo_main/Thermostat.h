/*
 * Thermostat.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Jiri Galuska
 */

//#define DEBUG 1
//#define DEBUG_TEMP_SENSOR

#include "Adafruit_SHT31.h"
#include "sht30test.h"
#include <EEPROM.h>

#define SHT30_ADDR 0x44

#ifndef THERMOSTAT_H_
#define THERMOSTAT_H_
#define MAX_UINT32 4294967295

#define MAX_TEMP 50
#define MIN_TEMP -10
#define DEBOUNCING_10th_C 5  // 1 represents 0.1 °C
#define HEATER_POWER_W 1500

/**************************************************************
 *              PIN MAPPING
 **************************************************************/
#define PIN_HEATER_RELE 2


/**************************************************************
 *              Memory mapping
 **************************************************************/
#define ADDR_TARGET_TEMP_2B 0x00
#define ADDR_PRICE_ELECTRICITY 0x02

/**************************************************************
 *              Class declaration
 **************************************************************/
class Thermostat {
public:
	//  Constructor
	Thermostat();
	// Destructor
	virtual ~Thermostat();

	// Turn on the thermostat
	void enable_termostat(bool v, bool mobile = false);

    // Get thermostat status. True = enabled
	bool get_termostat_status(void) {return termostat_on;}

	// Get heater status
	bool get_heater_status(void)    {return heater_on;}

    // Updates and returns temperature from the sensor. Inside temperature
	int16_t get_temperature_10th_c(void);

	int8_t get_rel_humidity(void);

	// Electricity price in 0.1 CZK
	void set_electricity_price_10th_CZK(uint8_t price);

	// Sets new target temperature in 10th °C
    void set_target_temperature_10th_c(int16_t temp) {target_temp_10th_c = temp;}

    // Reads set temperature
    // @Returns: Target temperature in 10th°C
    int16_t get_target_temperature_10th_c(void) { return target_temp_10th_c;} // OBOSLETE


    void update();


/**************************************************************
*              Member variables
**************************************************************/

	// Inside temperature in 0.1°C
	// Value factor 10! 20 -> 0.2°C
    //int8_t inside_temp;   // OBSOLETE
    int16_t inside_temp_10th_c;


    // Relative humidity
    uint8_t inside_humid_rel;

	//  Day consumption of electricity [100th kw/h]
	//
	uint16_t day_consuption_100thW;


	// 10th CZK for 1 Kw/h
	//
    uint8_t electricity_price_10th_CZK;

    bool enabled_by_mobile;

private:

/**************************************************************
*              Private Devices
**************************************************************/
	// Temperature sensor
#ifndef DEBUG_TEMP_SENSOR
    Adafruit_SHT31 tempSensor;
#else
    sht30_test tempSensor;
#endif

/**************************************************************
*             Private Member Methods
**************************************************************/
    // Activates the heater
    void enable_heater(bool v);

    // Writes target temperature to a ram
    void EEPROM_write_target_temp(int16_t temp);

    // Reads last target temperature
    int16_t EEPROM_read_target_temp(void);


/**************************************************************
*              Private Member Variables
**************************************************************/
    // Target temperature. Factor 0.1, offset 0
    int16_t target_temp_10th_c;

    // Status of the thermostat.
   	//       True = regulation of the temperature
   	//       False = Display only temperature and humidity on the display
    bool termostat_on;

    // Is heating element turned on?
    //       True = heater is activated
    //       False =
    bool heater_on;

    // Timestamp when the heater was engaged
    uint32_t heater_timestamp_ms;
};

#endif /* THERMOSTAT_H_ */
