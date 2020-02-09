/*
 * sht30test.h
 *
 *  Created on: Jan 26, 2020
 *      Author: jgaluska
 */

#ifndef SHT30TEST_H_
#define SHT30TEST_H_
#define SERIAL_DEBUG 1

class sht30_test {
public:
	sht30_test();
	virtual ~sht30_test();
	float temperature;
	float humidity;

	float readTemperature(void);
	float readHumidity(void);

	void begin(int add) {};

	float min_temp;
	float max_temp;

	bool increasing;
	unsigned char counter;

};

#endif /* SHT30TEST_H_ */
