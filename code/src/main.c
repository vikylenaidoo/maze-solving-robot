/*Group32 Maze SolverCode*/
// ----------------------------------------------------------------------------

//========================
//IMPORT LIBRARIES
//========================

#include "maze_solver.h"
//#include <time.h>
//#include "Node.h"

//========================
//DEFINE GLOBAL VARIABLES
//========================
#define forward_speed 100
#define turn_speed 50
#define turn_time 5
#define rotate_time 5

State state = {0,0,0,0,0};

mode MODE; //MAPPING or RACING or STANDBY or FINISHED
bool isStarted;
direction directions_taken[64];
direction optimal_path[64] = {STOP};
uint8_t counter;
bool flag;

//used for decision making and node identification
enum detect{
		WHITE=1,
		BLACK=0
};


State STRAIGHT 		= {WHITE, BLACK, WHITE, WHITE, WHITE};

State T_JUNCTION 	= {BLACK, WHITE, BLACK, WHITE, WHITE};
State t_junction 	= {BLACK, WHITE, BLACK, BLACK, WHITE};
State TJUNCTION 	= {BLACK, WHITE, BLACK, WHITE, BLACK};
State TJunction 	= {BLACK, WHITE, BLACK, BLACK, BLACK};


State FOUR_WAY 		= {BLACK, BLACK, BLACK, WHITE, WHITE};
State LEFT_CORNER 	= {BLACK, WHITE, WHITE, WHITE, WHITE};
State RIGHT_CORNER 	= {WHITE, WHITE, BLACK, WHITE, WHITE};
State LEFT_BRANCH 	= {BLACK, BLACK, WHITE, WHITE, WHITE};
State RIGHT_BRANCH 	= {WHITE, BLACK, BLACK, WHITE, WHITE};
State DRIFT_LEFT 	= {WHITE, WHITE, WHITE, WHITE, BLACK};
State DRIFT_RIGHT 	= {WHITE, WHITE, WHITE, BLACK, WHITE};
State DEAD_END		= {WHITE, WHITE, WHITE, WHITE, WHITE};
State FINISH 		= {BLACK, BLACK, BLACK, BLACK, BLACK};



//========================
//MAIN IMPLEMENTATION
//========================
void main(void){
	//init_LCD();
	//lcd_command(CLEAR);
	//lcd_putstring("Start");
	//head =  (struct Node*) malloc(sizeof(struct Node));
	init_GPIOA();
	init_GPIOB();
	init_PWM();
	//lcd_command(LINE_TWO);
	//lcd_putstring("initialisation");
	//printf("initialisation done \n");
	isStarted = 0;
	flag =0;
	while(true){

			while(isStarted){

				if(MODE==MAPPING){
					//set initial Mapping parameters
					counter = 0;
					directions_taken[counter] = FORWARD;
					counter++;

					while(MODE==MAPPING){
						drive(forward_speed);

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

		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PA; //map pa4 to exti4
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PA; //map pa5 to exti5
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PA;
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PA;
		SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PA;

		//for button inputs
		SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
		SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA;

		EXTI->IMR |= EXTI_IMR_MR0; //UNMASK EXTI0
		EXTI->FTSR |= EXTI_FTSR_TR0; // trigger on falling edge
		EXTI->FTSR |= EXTI_RTSR_TR0;

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
	//	NVIC_EnableIRQ(EXTI4_15_IRQn);
		NVIC_EnableIRQ(EXTI0_1_IRQn);


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

	//use for LED outputs
	GPIOB->MODER |= GPIO_MODER_MODER6_0;
	GPIOB->MODER |= GPIO_MODER_MODER7_0;
	GPIOB->MODER |= GPIO_MODER_MODER2_0;


	GPIOB->AFR[0] |= 0b0001; //
	GPIOB->AFR[0] |= 0b0001 << (4*1);  //
	GPIOB->AFR[0] |= 0b0001 << (4*4);
	GPIOB->AFR[0] |= 0b0001 << (4*5);

	GPIOB->ODR &= (~GPIO_ODR_6 & ~GPIO_ODR_7 & ~GPIO_ODR_2);


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


//========================
//IMPLMENT FUNCTIONS
//========================
void brake(void){
	TIM3->CCR1 = 100*100;
	TIM3->CCR2 = 100*100;
	TIM3->CCR3 = 100*100;
	TIM3->CCR4 = 100*100;
}

void drive(int speed){ //speed ~ duty cycle between [0; 100]
	//TODO figure out calibration

	//right wheel
	TIM3->CCR1 = (int)(40*speed);
	TIM3->CCR2 = 0;


	//left wheel
	TIM3->CCR3 = (int)(40*speed*0.93);
	TIM3->CCR4 = 0;

}

/*can only turn left or right*/
void turn(direction d){
	//TODO decide what optimum turning speed
	//brake();
	//directions_taken[counter] = d;
	//counter++;

	switch(d){
	case RIGHT:
		//right wheel
		TIM3->CCR1 = 0;
		TIM3->CCR2 = 40*turn_speed;
		//left wheel
		TIM3->CCR3 = 40*turn_speed;
		TIM3->CCR4 = 0;
		break;
	case LEFT:
		//right wheel
		TIM3->CCR1 = 40*turn_speed;
		TIM3->CCR2 = 0;
		//left wheel
		TIM3->CCR3 = 0;
		TIM3->CCR4 = 40*turn_speed;
		break;
	case FORWARD:
		drive(forward_speed);
		break;
	default: //indicate error
		//drive(forward_speed);
		brake();
		break;
	}

	//delay for some time
	delay(turn_time);
	brake();

}

void turnAround(){

//	directions_taken[counter] = BACKWARDS;
	//counter++;

	int speed = 10;
	//left wheel
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 100*speed;
	//right wheel
	TIM3->CCR3 = 100*speed;
	TIM3->CCR4 = 0;

	//delay(rotate_time);
	brake();

}

/*turn slightly while still going forward*/
void slightTurn(direction d){

		switch(d){
		case LEFT:
			//right wheel
			TIM3->CCR1 = 30*forward_speed;
			TIM3->CCR2 = 0;
			//left wheel
			TIM3->CCR3 = 25*forward_speed;
			TIM3->CCR4 = 0;
			break;
		case RIGHT:
			//right wheel
			TIM3->CCR1 = 25*forward_speed;
			TIM3->CCR2 = 0;
			//left wheel
			TIM3->CCR3 = 30*forward_speed;
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


void delay(int s){
	for(int i=0; i<s; i++){
		for(int j=0; j<s; j++){
			continue;
		}
	}
}

void optimise(){


}


State determineState(void){
	int s1 = (GPIOA->IDR & GPIO_IDR_4)>>4; //s1==0 means line is low ; s1==1 means line is high
	int s2 = (GPIOA->IDR & GPIO_IDR_5)>>5;
	int s3 = (GPIOA->IDR & GPIO_IDR_6)>>6;
	int s4 = (GPIOA->IDR & GPIO_IDR_7)>>7;
	int s5 = (GPIOA->IDR & GPIO_IDR_8)>>8;

	State state = {s1, s2, s3, s4, s5};
	return state;
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
	EXTI->IMR &= ~EXTI_IMR_MR4 & ~EXTI_IMR_MR5 & ~EXTI_IMR_MR6 & ~EXTI_IMR_MR7 & ~EXTI_IMR_MR8;

	// 1st interrupt will trigger and disable other interruots until the PR flag is reset;

	//lines pb4 to pb8 used for sensor inputs



	/*if(!isStarted){
		//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
		EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8;
		return;

	}*/

//	delay(2);// wait a bit before checking state

//	State state = determineState();
	GPIOB->ODR &= (~GPIO_ODR_6 & ~GPIO_ODR_7 & ~GPIO_ODR_2);

	brake();
	delay(1000);
	bool detect = 0;

	if((EXTI->PR & ~EXTI_PR_PR4)){ //s1 detected
		//brake();
		detect = 1;
		drive(20);

		delay(800);
		brake();

		state = determineState();


	}
	else{
		if(EXTI->PR & ~EXTI_PR_PR5){ //s2 detected
			//brake();
			detect = 1;
			drive(20);

			delay(800);
			brake();


			state = determineState();

		}
		else{
			if(EXTI->PR & ~EXTI_PR_PR6){ //s3 detected
				//brake();
				detect = 1;
				drive(20);

				delay(800);
				brake();

				state = determineState();

			}
			else{
				if(EXTI->PR & ~EXTI_PR_PR7){ //s4 detected
					//brake();
					detect = 1;
					drive(20);

					delay(800);
					brake();

					state = determineState();

				}
				else{
					if(EXTI->PR & ~EXTI_PR_PR8){ //s5 detected
						brake();
						detect = 1;
						drive(20);

						delay(800);
						brake();
						state = determineState();

					}
					else{ //false trigger??
						EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8;
						EXTI->IMR |= EXTI_IMR_MR4 | EXTI_IMR_MR5 | EXTI_IMR_MR6 | EXTI_IMR_MR7 | EXTI_IMR_MR8;
						brake();
						return;
					}

				}

			}

		}

	}

	if(detect){

		if(stateCompare(state, STRAIGHT)){
			GPIOB->ODR |= GPIO_ODR_2;
			drive(50);
			delay(1000);
		}
		else{
			if(stateCompare(state, DRIFT_LEFT)){
				GPIOB->ODR |= GPIO_ODR_6;
				slightTurn(RIGHT);
				delay(2000);
			}
			else{
				if(stateCompare(state, DRIFT_RIGHT)){
					GPIOB->ODR |= GPIO_ODR_7;
					slightTurn(LEFT);
					delay(2000);
				}
				else{
					if(stateCompare(state, LEFT_CORNER)){
						brake();

					}
					else{
						if(stateCompare(state, LEFT_BRANCH)){
							brake();

						}
						else{
							if(stateCompare(state, RIGHT_CORNER)){
								brake();

							}
							else{
								if(stateCompare(state, RIGHT_BRANCH)){
									brake();

								}
								else{
									if(stateCompare(state, TJunction) || stateCompare(state, T_JUNCTION) || stateCompare(state, TJUNCTION) || stateCompare(state, t_junction)){
										brake();
										GPIOB->ODR |= GPIO_ODR_7 | GPIO_ODR_6;
										//delay(1000);
										//drive(50);
										delay(500);
										brake();
										//delay(1000);
										turn(LEFT);
										delay(5000);

									}
									else{
										if(stateCompare(state, FOUR_WAY)){
											brake();

										}
										else{
											if(stateCompare(state, DEAD_END)){
												brake();

											}
											else{
												if(stateCompare(state, FINISH)){
													brake();

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

		}











	}


/*
	if(stateCompare(state, STRAIGHT)){
		drive(forward_speed);
		GPIOB->ODR = (GPIO_ODR_2);

		EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8; //clear the interrupt
		//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
	}
	else{
		if(stateCompare(state, DRIFT_LEFT)){

			delay(200);
			slightTurn(RIGHT);
			GPIOB->ODR |= (GPIO_ODR_6);

			EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8; //clear the interrupt
			//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);

		}
		else{
			if(stateCompare(state, DRIFT_RIGHT)){

				delay(200);
				slightTurn(LEFT);
				GPIOB->ODR |= (GPIO_ODR_7);

				EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8; //clear the interrupt
				//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);

			}
			else{

				delay(200);


				if(stateCompare(state, LEFT_CORNER)){
					brake();



					if(MODE==MAPPING){
						turn(LEFT);
						directions_taken[counter] = LEFT;
						counter++;
						drive(forward_speed);
					}


					//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
					EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8; //clear the interrupt

				}
				else{
					if(stateCompare(state, RIGHT_CORNER)){
						brake();
						if(MODE==MAPPING){
							turn(RIGHT);
							directions_taken[counter] = RIGHT;
							counter++;
							drive(forward_speed);
						}
						//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
						EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8; //clear the interrupt

					}
					else{
						if(stateCompare(state, LEFT_BRANCH)){

							if(MODE==MAPPING){
								brake();
								turn(LEFT);
								directions_taken[counter] = LEFT;
								counter++;
								drive(forward_speed);
							}
							//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
							EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8;
							//EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

						}
						else{
							if(stateCompare(state, RIGHT_BRANCH)){

								if(MODE==MAPPING){
									brake();
									directions_taken[counter] = FORWARD;
									counter++;
									turn(FORWARD);
								}
								//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);

								EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8;
							}
							else{
								if(stateCompare(state, T_JUNCTION)){

									delay(200);

									brake();
									if(MODE==MAPPING){
										turn(LEFT);
										directions_taken[counter] = LEFT;
										counter++;
										drive(forward_speed);
									}
									//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
									EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8; //clear the interrupt

								}
								else{
									if(stateCompare(state, FOUR_WAY)){

										if(MODE==MAPPING){
											brake();
											turn(LEFT);
											directions_taken[counter] = LEFT;
											counter++;
											drive(forward_speed);
										}
										//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
										EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

									}
									else{
										if(stateCompare(state, FINISH)){

											brake();
											if(MODE==MAPPING){
												MODE = STANDBY;
												//call optimisation

											}
											//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
											EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8; //clear the interrupt

										}
										else{ //none of these states detected
											if(stateCompare(state, DEAD_END)){

												brake();
												if(MODE==MAPPING){
													turnAround();
													directions_taken[counter] = BACKWARDS;
													counter++;

												}
												//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
												EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8;
											}
											else{

												drive(forward_speed);
												//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
												EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8;
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
	}
*/
	/*if none of these states are occuring then its a false trigger
	 * perhaps slow the robot down in this case?
	 */

	//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);

	EXTI->PR |= EXTI_PR_PR4| EXTI_PR_PR5 | EXTI_PR_PR6 |EXTI_PR_PR7| EXTI_PR_PR8;
	EXTI->IMR |= EXTI_IMR_MR4 | EXTI_IMR_MR5 | EXTI_IMR_MR6 | EXTI_IMR_MR7 | EXTI_IMR_MR8;


}

void EXTI0_1_IRQHandler(void){

	//EXTI->IMR &= ~EXTI_IMR_MR0;
	//EXTI->IMR &= ~EXTI_IMR_MR1;

	//GPIOB->ODR &= ~GPIO_ODR_6 & ~GPIO_ODR_7 & ~GPIO_ODR_8;

	/*if(flag) {
		GPIOB->ODR |= GPIO_ODR_6 | GPIO_ODR_7 | GPIO_ODR_8;
		flag = !flag;
	}
	else{
		GPIOB->ODR &= ~GPIO_ODR_6 & ~GPIO_ODR_7 & ~GPIO_ODR_8;
		flag = !flag;
	}*/
	if(GPIOA->IDR & GPIO_IDR_0){
		//GPIOB->ODR |= GPIO_ODR_6 | GPIO_ODR_7 | GPIO_ODR_8;
		//lcd_putstring("start btn");
		NVIC_EnableIRQ(EXTI4_15_IRQn);
		if(!isStarted ){
			isStarted =1;
			MODE = MAPPING;
			//GPIOB->ODR |= GPIO_ODR_6; //set LED0 ON to indicate started
			GPIOB->ODR |= GPIO_ODR_6 | GPIO_ODR_7 | GPIO_ODR_8;
			delay(500);
			GPIOB->ODR &= ~GPIO_ODR_6 & ~GPIO_ODR_7 & ~GPIO_ODR_8;

			drive(50);
		}
		/*else{
			isStarted =0;
			GPIOB->ODR &= ~GPIO_ODR_6; //set LED0 OFF to indicate not started
		}*/

	}
	/*else{
		if(GPIOA->IDR & GPIO_IDR_1){
			//lcd_putstring("Mode changed");
			if(MODE==MAPPING){
				GPIOB->ODR |= GPIO_ODR_7; //set LED1 ON to indicate RACING MODE
				MODE = RACING;
			}
			else{
				if(MODE==RACING){
					GPIOB->ODR &= ~GPIO_ODR_7;
					MODE = STANDBY;
				}
			}
		}

	}*/

	//NVIC_ClearPendingIRQ(EXTI0_1_IRQn);
	//EXTI->IMR |= EXTI_IMR_MR0;
	//EXTI->IMR |= EXTI_IMR_MR0;
	EXTI->PR |= EXTI_PR_PR0 | EXTI_PR_PR1;

}









