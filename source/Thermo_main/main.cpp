
#include "main.h"

Thermostat termostat;
uint8_t alive_counter = 0;

void setup() {
	Serial.begin(9600);
	Serial.println("Start");
	while(!Serial){};  // Wait for initialization of the communication

    //wdt_enable(WDTO_8S);

    /* TESTING */
    //termostat.set_electricity_price_10th_CZK(25U);
    //termostat.set_target_temperature_10th_c(200);
    termostat.enable_termostat(true, false);
    Serial.println("Start");


    /*  */
}


void loop(){
	Serial.println("Start");
    task_1_prio();
    if( !(alive_counter % 10U))
    {
    	task_2_prio();
    }
    if( !(alive_counter % 20U))
    {
    	Serial.println(alive_counter);
    	task_3_prio();
    	delay(500);
    }
    alive_counter++;
    if (alive_counter > 60U)
    	alive_counter = 0;


}

// Fastest runable to control the UA
void task_1_prio(void)
{

}

void task_2_prio(void)
{

}

// Runnable function to update the temperature readings
void task_3_prio(void)
{

	termostat.update();
	//wdt_reset();
	print_thermostat_status(&termostat);

}

void print_thermostat_status(Thermostat * termostat)
{
	Serial.println('\n');
	Serial.println("Temp:");
	Serial.println(termostat->get_temperature_10th_c());
	Serial.println("\nStat:");
	Serial.println(termostat->get_termostat_status());
	Serial.println("\nHeater: ");
	Serial.println(termostat->get_heater_status());
	// Electricity Price
	Serial.println("Price: ");
    Serial.println(termostat->electricity_price_10th_CZK);

	// Target temperature
	Serial.println("Target:");
	Serial.println(termostat->get_target_temperature_10th_c());

}
