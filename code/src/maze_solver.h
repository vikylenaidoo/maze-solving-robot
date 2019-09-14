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
enum direction {
	LEFT
};



//========================
//DEFINE FUNCTIONS
//========================
void init_GPIOA(void); // for inputs
void init_GPIOB(void); // for outputs
void init_PWM(void);
void goForward(int speed);
void turn(int direction);

































#endif /* MAZE_SOLVER_H_ */
