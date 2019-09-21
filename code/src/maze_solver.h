/*
 * maze_solver.h
 *
 *  Created on: 14 Sep 2019
 *      Author: vikylenaidoo
 */

#ifndef MAZE_SOLVER_H_
#define MAZE_SOLVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

#include "stm32f0xx.h"
#include "lcd_stm32f0.h"
//========================
//DEFINE CONSTANTS and GLOBALS
//========================
typedef enum {
	LEFT, //0
	RIGHT //1
} direction;

typedef enum {
	WEST,	//0
	NORTH,	//1
	EAST,	//2
	SOUTH	//3
} cardinal;

typedef enum { //how many distinct paths from each type of junction need to be distinclty explored
	LEFT_TURN = 1,
	RIGHT_TURN = 1,
	T_JUNCTION = 3,
	FOUR_WAY = 4,
	LEFT_BRANCH = 2,
	RIGHT_BRANCH = 2

}junction_type;

extern struct Node* head;
extern struct Node* tail;











//========================
//DEFINE FUNCTIONS
//========================
void init_GPIOA(void); // for inputs
void init_GPIOB(void); // for outputs
void init_PWM(void);

void drive(int speed);
void turn(direction d); //rotate 90* in the given direction
void turnAround(int k); //rotate k*180 degrees
void brake(void);































#endif /* MAZE_SOLVER_H_ */
