/*
 * Termo_main.h
 *
 *  Created on: Feb 8, 2020
 *      Author: jgaluska
 */

#ifndef MAIN_H_
#define MAIN_H_


#include <Wire.h>
#include <Arduino.h>
#include "Thermostat.h"
#include <avr/wdt.h>


void task_1_prio(void);
void task_2_prio(void);
void task_3_prio(void);

void print_thermostat_status(Thermostat * termostat);


#endif /* MAIN_H_ */
