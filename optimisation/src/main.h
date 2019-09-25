/*
 * main.h
 *
 *  Created on: 25 Sep 2019
 *      Author: vikylenaidoo
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_


typedef enum {
	LEFT, //0
	FORWARD,
	RIGHT,
	BACKWARDS,
	STOP
} direction;

void printArray(direction* array);
void clearArray(direction* array);
void optimise(direction* input_array, direction* output_array);

#endif /* SRC_MAIN_H_ */
