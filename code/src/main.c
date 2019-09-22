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

mode MODE; //MAPPING or RACING
bool isStarted;
int last_time;//used for storing the time captured at the last(previous) detected node
int key; //primary key/id for the Node list
cardinal cardinal_direction; //the absolute direction which the car is travelling (NSEW)

//used for storing reuslt of mapping
struct Node* head;
int current_coordinates[2];

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

	//head =  (struct Node*) malloc(sizeof(struct Node));
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
				current_coordinates[0] = 0; //set x=0
				current_coordinates[1] = 0; //set y=0
				key=0;
				last_time=0;
				cardinal_direction = NORTH;

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
	default: //indicate error
		turnAround(4); //or flash leds maybe?
	}

	//delay for some time
	delay(30);
	brake();

}

void turnAround(int k){
	int speed = 10;
	//left wheel
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 100*speed;
	//right wheel
	TIM3->CCR3 = 100*speed;
	TIM3->CCR4 = 0;

	//delay for some time
	delay(k);
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

void delay(int s){
	for(int i=0; i<s; i++){
		for(int j=0; j<s; j++){
			continue;
		}
	}
}

//========================
//INTEERRUPT HANDLERS8
//========================

/*controls lines 4 to 15: mapped to inputs from sensors (PB4-PB8)*/
void EXTI4_15_IRQHandler(void){
	//TODO: confirm sensor topological config. Does the tail swing alot?

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
		EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

	}
	else{
		if(stateCompare(state, DRIFT_LEFT)){
			slightTurn(RIGHT);

			EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

		}
		else{
			if(stateCompare(state, DRIFT_RIGHT)){
				slightTurn(LEFT);

				EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

			}
			else{
				if(stateCompare(state, LEFT_CORNER)){
					brake();
					//find current coordinates by subtracting the previous


					//insertLast();

					EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

				}
				else{
					if(stateCompare(state, RIGHT_CORNER)){
						brake();

						EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

					}
					else{
						if(stateCompare(state, LEFT_BRANCH)){
							brake();

							EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

						}
						else{
							if(stateCompare(state, RIGHT_BRANCH)){
								brake();

								EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

							}
							else{
								if(stateCompare(state, T_JUNCTION)){
									brake();

									EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

								}
								else{
									if(stateCompare(state, FOUR_WAY)){
										brake();

										EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

									}
									else{
										if(stateCompare(state, FINISH)){
											brake();


											EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

										}
										else{ //none of these states detected
											drive(forward_speed);

											EXTI->PR |= EXTI_PR_PR0; //clear the interrupt

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

	//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);


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
			else{
				if(MODE==RACING){
					GPIOB->ODR &= ~GPIO_ODR_1; //set LED1 ON to indicate MAPPING MODE

				}

			}
		}

	}

}













