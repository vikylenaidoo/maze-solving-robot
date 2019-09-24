/*Group32 Maze SolverCode*/
// ----------------------------------------------------------------------------

//========================
//IMPORT LIBRARIES
//========================

#include "maze_solver.h"
//#include <time.h>
#include "Node.h"

//========================
//DEFINE GLOBAL VARIABLES
//========================

#define forward_speed 80//used for movement control


cardinal order_of_preference[4] = {WEST, NORTH , EAST, SOUTH};//{WEST, NORTH, EAST, SOUTH};

mode MODE; //MAPPING or RACING
bool isStarted;
volatile int last_time;//used for storing the time captured at the last(previous) detected node
uint8_t key; //primary key/id for the Node list
cardinal current_cardinal; //the absolute direction which the car is travelling (NSEW)
volatile int current_time;


//used for storing reuslt of mapping
struct Node* head;
struct Node* tail;

uint8_t current_coordinates[2];
uint8_t list_size;
int total_nodes;

//used for decision making and node identification
enum detect{
		WHITE=0,
		BLACK=1
};




State STRAIGHT 		= {WHITE, BLACK, WHITE, WHITE, WHITE};
State T_JUNCTION 	= {BLACK, WHITE, BLACK, WHITE, WHITE};
State FOUR_WAY 		= {BLACK, BLACK, BLACK, WHITE, WHITE};
State LEFT_CORNER 	= {BLACK, WHITE, WHITE, WHITE, WHITE};
State RIGHT_CORNER 	= {WHITE, WHITE, BLACK, WHITE, WHITE};
State LEFT_BRANCH 	= {BLACK, BLACK, WHITE, WHITE, WHITE};
State RIGHT_BRANCH 	= {WHITE, BLACK, BLACK, WHITE, WHITE};
State DRIFT_LEFT 	= {WHITE, WHITE, WHITE, WHITE, BLACK};
State DRIFT_RIGHT 	= {WHITE, WHITE, WHITE, BLACK, WHITE};
State FINISH 		= {BLACK, BLACK, BLACK, BLACK, BLACK};



//========================
//MAIN IMPLEMENTATION
//========================
void main(void){


	init_GPIOA();
	init_GPIOB();
	init_PWM();

	isStarted = 0; //initially not started
	mode MODE = MAPPING; //start off mapping the after button is prssed change to RACING
	init_LCD();
	lcd_command(CLEAR);

	while(true){

		while(isStarted){


			if(MODE==MAPPING){
				//set initial Mapping parameters
				head =  (struct Node*) malloc(sizeof(struct Node));
				head->key=0;
				head->coordinates[0] = 0;
				head->coordinates[1] = 0;
				head->j_type = start;
				head->explored=0;

				cardinal available_cardinals[4];// = {NORTH, NULL, NULL, NULL};
				available_cardinals[0] = NORTH;

				head->path_cardinals[0] = available_cardinals[0];

				head->next = NULL;
				head->prev = NULL;


				tail = head;
				total_nodes=1;

				current_coordinates[0] = 0; //set x=0
				current_coordinates[1] = 0; //set y=0

				last_time=0;
				current_cardinal= NORTH;

				while(MODE==MAPPING){
					drive(50);

				}




			}
			else{
				while(MODE==RACING){


				}
			}






		}
	}


}











 //========================
 //INITIALIZATION FUNCTIONS
 //========================


/* USE PA4-PA8 FOR SNESOR INPUTS AND INTERRUPTS*/
void init_GPIOA(void){ //used for outputs
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	//SET PINS TO INPUT MODE
		GPIOA->MODER &= ~GPIO_MODER_MODER4;
		GPIOA->MODER &= ~GPIO_MODER_MODER5;
		GPIOA->MODER &= ~GPIO_MODER_MODER6;
		GPIOA->MODER &= ~GPIO_MODER_MODER7;
		GPIOA->MODER &= ~GPIO_MODER_MODER8;

		GPIOA->MODER &= ~GPIO_MODER_MODER0; //use to set start
		GPIOA->MODER &= ~GPIO_MODER_MODER1; // use to set mode
		GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0;
		GPIOA->PUPDR |= GPIO_PUPDR_PUPDR1_0;

		//CONFIGURE INTERRUPTS
		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN; //enable clock for syscfg

		//for sensors
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PA; //map pa4 to exti4
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PA; //map pa5 to exti5
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PA;
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PA;
		SYSCFG->EXTICR[3] |= SYSCFG_EXTICR3_EXTI8_PA;

		//for button inputs
		SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
		SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA;


		EXTI->IMR |= EXTI_IMR_MR0; //UNMASK EXTI0
		EXTI->FTSR |= EXTI_FTSR_TR0; // trigger on falling edge

		EXTI->IMR |= EXTI_IMR_MR1; //UNMASK EXTI1
		EXTI->FTSR |= EXTI_FTSR_TR1; // trigger on falling edge

		EXTI->IMR |= EXTI_IMR_MR4; //UNMASK EXTI4
		EXTI->RTSR |= EXTI_RTSR_TR4; // trigger on rising edge
		EXTI->FTSR |= EXTI_FTSR_TR4; // trigger on falling edge

		EXTI->IMR |= EXTI_IMR_MR5; //UNMASK EXTI5
		EXTI->RTSR |= EXTI_RTSR_TR5; // trigger on rising edge
		EXTI->FTSR |= EXTI_FTSR_TR5; // trigger on falling edge

		EXTI->IMR |= EXTI_IMR_MR6; //UNMASK EXTI6
		EXTI->RTSR |= EXTI_RTSR_TR6; // trigger on rising edge
		EXTI->FTSR |= EXTI_FTSR_TR6; // trigger on falling edge

		EXTI->IMR |= EXTI_IMR_MR7; //UNMASK EXTI7
		EXTI->RTSR |= EXTI_RTSR_TR7; // trigger on rising edge
		EXTI->FTSR |= EXTI_FTSR_TR7; // trigger on falling edge

		EXTI->IMR |= EXTI_IMR_MR8; //UNMASK EXTI8
		EXTI->RTSR |= EXTI_RTSR_TR8; // trigger on rising edge
		EXTI->FTSR |= EXTI_FTSR_TR8; // trigger on falling edge

		//enable interrupt
		NVIC_EnableIRQ(EXTI4_15_IRQn);
		NVIC_EnableIRQ(EXTI0_1_IRQn);
		NVIC_SetPriority(EXTI4_15_IRQn, 64);
		NVIC_SetPriority(EXTI0_1_IRQn, 64);

}


/* USE LINES PB0,1,4,5 ON TIM3_CH3,4,1,2 RESPECTIVELY FOR MOTOR PWM OUTPUTS
 * AF1 ON PB IS MAPPED TO TIM3
 * */
void init_GPIOB(void){// USED FOR OUTPUTS

	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	//use pb0@tim3_ch3 pb1@tim3_ch4 pb4@tim3_ch1 pb5@tim3_ch2 //af1
	GPIOB->MODER |= GPIO_MODER_MODER0_1; //
	GPIOB->MODER |= GPIO_MODER_MODER1_1; //
	GPIOB->MODER |= GPIO_MODER_MODER4_1;
	GPIOB->MODER |= GPIO_MODER_MODER5_1;

	//use PB2 and PB3 for LED outputs for displaying info
	GPIOB->MODER |= GPIO_MODER_MODER2_0;
	GPIOB->MODER |= GPIO_MODER_MODER3_0;
	//set initial value off
	GPIOB->ODR &= ~GPIO_ODR_0;
	GPIOB->ODR &= ~GPIO_ODR_0;

	//set pwm1 mode for af1
	GPIOB->AFR[0] |= 0b0001; //
	GPIOB->AFR[0] |= 0b0001 << (4*1);  //
	GPIOB->AFR[0] |= 0b0001 << (4*4);
	GPIOB->AFR[0] |= 0b0001 << (4*5);


}

/*USE TIM3 FOR PWM
 * */
void init_PWM(void){
	// TODO: choose frequency

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = 23;
	TIM3->ARR = 3999;	// PWM freqeuncy = 500Hz


	//set pwm mode 1
	TIM3->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // PWM Mode 1 on channel1
	TIM3->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1); // PWM Mode 1 on channel2
	TIM3->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1); // PWM Mode 1 on channel3
	TIM3->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1); // PWM Mode 1 on channel4

	//set duty cycle 40*duty_cycle
	TIM3->CCR1 = 40 * 100; // Duty Cycle for PB4 (ch1)
	TIM3->CCR2 = 40 * 100; // Duty Cycle for PB5 (ch2)
	TIM3->CCR3 = 40 * 10; // Duty Cycle for PB0 (ch3)
	TIM3->CCR4 = 40 * 10; // Duty Cycle for PB1 (ch4)

	//enable OC channels
	TIM3->CCER |= TIM_CCER_CC1E;
	TIM3->CCER |= TIM_CCER_CC2E;
	TIM3->CCER |= TIM_CCER_CC3E;
	TIM3->CCER |= TIM_CCER_CC4E;

	TIM3->CR1 |= TIM_CR1_CEN;
	//printf("pwm initialisation done \n");
}

void init_TIM2(void){
	RCC->AHBENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 999;
	TIM2->ARR = 2399;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_CEN;

	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 0);

}


//========================
//IMPLMENT FUNCTIONS
//========================
void brake(void){
	TIM3->CCR1 = 40*100;
	TIM3->CCR2 = 40*100;
	TIM3->CCR3 = 40*100;
	TIM3->CCR4 = 40*100;
}

void drive(int speed){ //speed ~ duty cycle between [0; 100]
	//TODO figure out calibration

	//left wheel
	TIM3->CCR1 = 40*speed;
	TIM3->CCR2 = 0;


	//right wheel
	TIM3->CCR3 = 40*speed;
	TIM3->CCR4 = 0;

}

/*can only turn left or right*/
void turn(direction d){
	//TODO decide what optimum turning speed
	brake();
	int speed = 10;

	switch(d){
	case LEFT:
		//left wheel
		TIM3->CCR1 = 0;
		TIM3->CCR2 = 40*speed;
		//right wheel
		TIM3->CCR3 = 40*speed;
		TIM3->CCR4 = 0;
		break;
	case RIGHT:
		//left wheel
		TIM3->CCR1 = 40*speed;
		TIM3->CCR2 = 0;
		//right wheel
		TIM3->CCR3 = 0;
		TIM3->CCR4 = 40*speed;
		break;
	case FORWARD: //not recommended
		drive(forward_speed);
		break;
	default: //indicate error
		drive(forward_speed);
	}

	//wait for some time
	wait(30);
	brake();
	updateCardinal(d);

}

void turnAround(int k){
	int speed = 10;
	//left wheel
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 40*speed;
	//right wheel
	TIM3->CCR3 = 40*speed;
	TIM3->CCR4 = 0;

	//wait for some time
	wait(k);
	brake();

}

/*turn slightly while still going forward*/
void slightTurn(direction d){

		switch(d){
		case LEFT:
			//left wheel
			TIM3->CCR1 = 15*forward_speed;
			TIM3->CCR2 = 0;
			//right wheel
			TIM3->CCR3 = 30*forward_speed;
			TIM3->CCR4 = 0;
			break;
		case RIGHT:
			//left wheel
			TIM3->CCR1 = 30*forward_speed;
			TIM3->CCR2 = 0;
			//right wheel
			TIM3->CCR3 = 15*forward_speed;
			TIM3->CCR4 = 0;
			break;
		default: //indicate error
			turnAround(4); //or flash leds maybe? //should never actually occur
		}

}

/*check wheteher the combination of states are corresponding to the given state*/
bool stateCompare(State state1, State state2){

	if(state1.s1 != state2.s1) return false;
	if(state1.s2 != state2.s2) return false;
	if(state1.s3 != state2.s3) return false;
	if(state1.s4 != state2.s4) return false;
	if(state1.s5 != state2.s5) return false;

	return true;
}

void wait(int s){
	for(int i=0; i<s; i++){
		for(int j=0; j<s; j++){
			continue;
		}
	}
}

void updateCurrentCoordinates(void){
	int current_x = current_coordinates[0];
	int current_y = current_coordinates[1];
	int distance = (int)floor(current_time-last_time);

	switch(current_cardinal){
		case NORTH:
			current_y = current_coordinates[1]+distance;
			break;
		case SOUTH:
			current_y = current_coordinates[1]-distance;
			break;
		case EAST:
			current_x = current_coordinates[0]+distance;
			break;
		case WEST:
			current_x = current_coordinates[0]-distance;
			break;
	}

	current_coordinates[0] = current_x;
	current_coordinates[1] = current_y;

	last_time = current_time;

}

/*valis for 90* rotation only */
void updateCardinal(direction turning){
	switch(turning){
		case LEFT:
			if(current_cardinal==WEST){
				current_cardinal=SOUTH;
			}
			else{
				current_cardinal--;
			}
			break;
		case RIGHT:
			if(current_cardinal==SOUTH){
				current_cardinal=WEST;
			}
			else{
				current_cardinal++;
			}
			break;
		case FORWARD:
			break;



	}

}

/*given the current cardinal orientaion and a cardinal we want to turn to,
 * return a direction in which to turn
 * can only give cardinals which are 90* apart or 0* apart
 * eg: given N, N ==> turn Forward
 * given N, W ==> turn LEFT
 * */
direction cardinalToDirection(cardinal desired_cardinal	){
	direction need_turn = FORWARD;
	if(desired_cardinal==current_cardinal){
		need_turn = FORWARD;
	}
	else{
		if(desired_cardinal==(current_cardinal+1)){
			need_turn = RIGHT;

		}
		else{
			if(desired_cardinal==(current_cardinal-1)){
				need_turn = LEFT;
			}
			else{
				if(current_cardinal==SOUTH && desired_cardinal==WEST){
					need_turn = RIGHT;
				}
				else{
					if(current_cardinal==WEST && desired_cardinal==SOUTH){
						need_turn = LEFT;
					}

				}

			}

		}

	}

	return need_turn;

}


/*input: direction you want to turn
 * output: cardinal required depending on your current cardinal orientation
 * only input LEFT, RIGHT, FORWARD
 * */
cardinal directionToCardinal(direction desired_direction){
	cardinal need_cardinal = NORTH;

	if(desired_direction == LEFT){
		if(current_cardinal == WEST){
			need_cardinal = SOUTH;
		}
		else{
			need_cardinal = current_cardinal-1;
		}
	}
	else{
		if(desired_direction == RIGHT){
			if(current_cardinal == SOUTH){
				need_cardinal = WEST;
			}
			else{
				need_cardinal = current_cardinal+1;

			}

		}
		else{
			if(desired_direction==FORWARD){
				need_cardinal = current_cardinal;

			}

		}

	}


	return need_cardinal;
}

cardinal invertOrientation(void){
	cardinal opposite = current_cardinal;

	switch (current_cardinal) {
		case NORTH:
			opposite = SOUTH;
			break;
		case SOUTH:
			opposite = NORTH;
			break;
		case EAST:
			opposite = WEST;
			break;
		case WEST:
			opposite = EAST;
			break;

		default:
			break;
	}

	return opposite;

}

/*IMPORTANT: use free() after done usiong this returned array to avois memory leak
 * length of array is 4
 * will return an array containging the keys of adjcacent nodes
 * one key for each direction
 * if no adjcent in a certain direction then key of the current node
 * if nodes have a path to the current node
 * */
uint8_t* detectNearbyNodes(struct Node* currentNode){
	//uint8_t keys[4] = {currentNode->key,currentNode->key, currentNode->key, currentNode->key}; //default values
	uint8_t *keys = (uint8_t*)malloc(4*sizeof(uint8_t)); //initialise new array of size 4 to keep keys
	keys[0] = currentNode->key;
	keys[1] = currentNode->key;
	keys[2] = currentNode->key;
	keys[3] = currentNode->key;

	struct Node* tempNode; // = (struct Node*) malloc(sizeof(struct Node));
	tempNode = head;

	/*traverse through whole list checking each node
	 * check if the node is closer than the current neighbour in each direction
	 * if so then set this as the new naighbour
	 *
	 * */
	while(tempNode->next != NULL){

		if(tempNode->coordinates[0] == currentNode->coordinates[0] && tempNode->key != currentNode->key){ //same x value: search vertical
			if(tempNode->coordinates[1]>currentNode->coordinates[1]){ //to the north
				if(tempNode->coordinates[1]<find(keys[1])->coordinates[1] || keys[1] == currentNode->key){ //it is closer to the currentNode
					keys[1] = tempNode->key; //set this as the new North neighbour;
				}

			}
			else{
				if(tempNode->coordinates[1] < currentNode->coordinates[1]){//to the south
					if(tempNode->coordinates[1]>find(keys[3])->coordinates[1] || keys[3] == currentNode->key){ //it is closer to the currentNode
						keys[3] = tempNode->key; //new south neighbour
					}

				}

			}

		}
		else{
			if(tempNode->coordinates[1] == currentNode->coordinates[1] && tempNode->key != currentNode->key){ //same y value: search horizantal

				if(tempNode->coordinates[1]>currentNode->coordinates[1]){ //to the east
					if(tempNode->coordinates[0]<find(keys[0])->coordinates[0] || keys[0] == currentNode->key){ //it is closer to the currentNode
						keys[0] = tempNode->key; //set this as the new east neighbour;
					}

				}
				else{
					if(tempNode->coordinates[0] < currentNode->coordinates[0]){//to the west
						if(tempNode->coordinates[0]>find(keys[2])->coordinates[1] || keys[2] == currentNode->key){ //it is closer to the currentNode
							keys[2] = tempNode->key; //new west neighbour
						}

					}

				}

			}

		}

	}

	return keys;

}

//========================
//INTEERRUPT HANDLERS8
//========================

/*controls lines 4 to 15: mapped to inputs from sensors (PB4-PB8)*/
void EXTI4_15_IRQHandler(void){
	/*TODO:
	 * confirm sensor topological config. Does the tail swing alot?
	 * update cardinal direction for each case
	 * keep which cardinals still need to be explored as opposed to directions (directions are relative whilst cardinals are absolute)
	 */

	// 1st interrupt will trigger and disable other interruots until the PR flag is reset;


	//lines pb4 to pb8 used for sensor inputs
	int s1 = (GPIOA->IDR & GPIO_IDR_4)>>4; //s1==0 means line is low ; s1==1 means line is high
	int s2 = (GPIOA->IDR & GPIO_IDR_5)>>5;
	int s3 = (GPIOA->IDR & GPIO_IDR_6)>>6;
	int s4 = (GPIOA->IDR & GPIO_IDR_7)>>7;
	int s5 = (GPIOA->IDR & GPIO_IDR_8)>>8;



	State state = {s1, s2, s3, s4, s5};

	if(stateCompare(state, STRAIGHT)){
		drive(forward_speed);
		//EXTI->PR |= EXTI_PR_PR0; //clear the interrupt
		NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
	}
	else{
		if(stateCompare(state, DRIFT_LEFT)){
			slightTurn(RIGHT);

			//EXTI->PR |= EXTI_PR_PR0; //clear the interrupt
			NVIC_ClearPendingIRQ(EXTI4_15_IRQn);

		}
		else{
			if(stateCompare(state, DRIFT_RIGHT)){
				slightTurn(LEFT);

				//EXTI->PR |= EXTI_PR_PR0; //clear the interrupt
				NVIC_ClearPendingIRQ(EXTI4_15_IRQn);

			}
			else{
				if(stateCompare(state, LEFT_CORNER)){
					brake();
					//find current coordinates by subtracting the previous
					updateCurrentCoordinates();
					if(find_by_coordinates(current_coordinates[0], current_coordinates[1]) == NULL){ // has not been found before
						/*create new node
						 * set id key to the last id on the list +1
						 * set current coordinates
						 * it is left turn
						 * explored = 1 (all new nodes will start with being explored once)
						 * */

						cardinal available_paths[4];
						available_paths[0] = directionToCardinal(LEFT);

						insertLast(tail->key+1, current_coordinates, left_turn, 1, available_paths); // in this case explored==junction_type hwnce we need not come back here

						//turn left and continue forward
						turn(LEFT);
						drive(forward_speed);

					}
					else{
						turn(LEFT);
						drive(forward_speed);
					}

					//insertLast();

					NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
					//EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

				}
				else{
					if(stateCompare(state, RIGHT_CORNER)){
						brake();

						updateCurrentCoordinates();
						if(find_by_coordinates(current_coordinates[0], current_coordinates[1]) == NULL){ // has not been found before
							/*create new node
							 * set id key to the last id on the list +1
							 * set current coordinates
							 * it is left turn
							 * explored = 1 (all new nodes will start with being explored once)
							 * */

							cardinal available_paths[4];
							available_paths[0] = directionToCardinal(RIGHT);

							insertLast(tail->key+1, current_coordinates, right_turn, 1, available_paths); // in this case explored==junction_type hwnce we need not come back here

							//turn left and continue forward
							turn(RIGHT);
							drive(forward_speed);

						}
						else{
							turn(RIGHT);
							drive(forward_speed);
						}

						NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
						//EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

					}
					else{
						if(stateCompare(state, LEFT_BRANCH)){
							brake();
							//find current coordinates by subtracting the previous
							updateCurrentCoordinates();
							struct Node* current_node = find_by_coordinates(current_coordinates[0], current_coordinates[1]);

							if(current_node == NULL){ // has not been found before
								/*create new node */
								cardinal *available_paths[4] = {NULL, NULL, NULL, NULL};
								available_paths[0] = &directionToCardinal(LEFT);
								available_paths[1] = &directionToCardinal(FORWARD);

								insertLast(tail->key+1, current_coordinates, left_branch, 1, available_paths); // in this case explored==junction_type hwnce we need not come back here

								direction chosen_direction = available_paths[tail->explored-1];
								//decide which cardinal to choose and figure out which direction to turn
								//turn(LEFT);



								drive(forward_speed);

							}
							else{ // has been discovered

								if(current_node->explored < current_node->j_type){ //not fully explored
									//which cardinal are we coming from?
									cardinal coming_from = invertOrientation();

									for(int i=0; i<4; i++){
										if(current_node->path_cardinals[i] == coming_from){
											//has come from this path before so dont increment explored
											break;
										}
									}


								}
								else{
									if(current_node->explored==current_node->j_type){ //has been fully explored
										//see dead end
										//look for more unexplored nodes

									}

								}

							}

							NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
							//EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

						}
						else{
							if(stateCompare(state, RIGHT_BRANCH)){
								brake();

								NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
								//EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

							}
							else{
								if(stateCompare(state, T_JUNCTION)){
									brake();

									NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
									//EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

								}
								else{
									if(stateCompare(state, FOUR_WAY)){
										brake();

										NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
										//EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

									}
									else{
										if(stateCompare(state, FINISH)){
											brake();

											NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
											//EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

										}
										else{ //none of these states detected
											drive(forward_speed);

											NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
											//EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	/*if none of these states are occuring then its a false trigger
	 * perhaps slow the robot down in this case?
	 */

	NVIC_ClearPendingIRQ(EXTI4_15_IRQn);


}

void EXTI0_1_IRQHandler(void){
	if(GPIOA->IDR & GPIO_IDR_0){
		lcd_putstring("start btn");
		if(!isStarted){
			isStarted =1;
			GPIOB->ODR |= GPIO_ODR_0; //set LED0 ON to indicate started
		}
		else{
			isStarted =0;
			GPIOB->ODR &= ~GPIO_ODR_0; //set LED0 OFF to indicate not started
		}

	}
	else{
		if(GPIOA->IDR & GPIO_IDR_1){
			lcd_putstring("Mode changed");
			if(MODE==MAPPING){
				GPIOB->ODR |= GPIO_ODR_1; //set LED1 ON to indicate MAPPING MODE

			}
			/*else{
				if(MODE==RACING){
					GPIOB->ODR &= ~GPIO_ODR_1; //set LED1 ON to indicate MAPPING MODE

				}

			}*/
		}

	}

	NVIC_ClearPendingIRQ(EXTI0_1_IRQn);

}

void TIM2_IRQHandler(){
	current_time++;
	NVIC_ClearPendingIRQ(TIM2_IRQn);

}













