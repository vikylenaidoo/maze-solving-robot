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
typedef enum{
	MAPPING,
	RACING

}mode;

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
	left_turn = 1,
	right_turn = 1,
	T_junction = 3,
	four_way = 4,
	left_branch = 2,
	right_branch = 2

}junction_type;

extern struct Node* head;
extern struct Node* tail;

typedef struct{
	int s1;
	int s2;
	int s3;
	int s4;
	int s5;
}State;









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
void slightTurn(direction d);

bool stateCompare(State state1, State state2); //will return what state the






























#endif /* MAZE_SOLVER_H_ */
