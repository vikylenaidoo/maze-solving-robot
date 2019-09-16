/*
 * maze_solver.h
 *
 *  Created on: 14 Sep 2019
 *      Author: vikylenaidoo
 */

#ifndef MAZE_SOLVER_H_
#define MAZE_SOLVER_H_

//========================
//DEFINE CONSTANTS
//========================
typedef enum {
	LEFT,
	RIGHT
} direction;

typedef enum {
	WEST,
	NORTH,
	EAST,
	SOUTH
} cardinal;



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
