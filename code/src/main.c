/*Group32 Maze SolverCode*/
// ----------------------------------------------------------------------------

//========================
//IMPORT LIBRARIES
//========================
#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "maze_solver.h"


//========================
//DEFINE GLOBAL VARIABLES
//========================







//========================
//MAIN IMPLEMENTATION
//========================
void main(void){


}











 //========================
 //INITIALIZATION FUNCTIONS
 //========================


void init_GPIOA(void){ //used for outputs
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	//config pa8 to pa11 for pwm
	GPIOA->MODER |= GPIO_MODER_MODER8_1; // PA8 = AF
	GPIOA->MODER |= GPIO_MODER_MODER9_1; //  = AF
	GPIOA->MODER |= GPIO_MODER_MODER10_1;
	GPIOA->MODER |= GPIO_MODER_MODER11_1;

	GPIOA->AFR[1] |= 2; // PA8_AF = AF2 (ie: map to TIM1_CH1)
	GPIOA->AFR[1] |= 2 << 4;  // PA9_AF = AF2 (ie: map to TIM1_CH2)
	GPIOA->AFR[1] |= 2 << (4*2);
	GPIOA->AFR[1] |= 2 << (4*3);


}


void init_GPIOB(void){// used for inputs
	RCC  ->AHBENR |= RCC_AHBENR_GPIOBEN;

	// configre  5 input lines fot the sensors
	GPIOB->MODER &= ~GPIO_MODER_MODER0;  // set to input mode
	GPIOB->MODER &= ~GPIO_MODER_MODER1;
	GPIOB->MODER &= ~GPIO_MODER_MODER2;
	GPIOB->MODER &= ~GPIO_MODER_MODER3;
	GPIOB->MODER &= ~GPIO_MODER_MODER4;


	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR0_0; // confirm PUPD for each sensor
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR1_0;


	//configure interrupts
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN; //enable clock for syscfg

	// we are using PB4 to PB8 for sensors 1 to 5 (because they are all on the same interrupt handler line)
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PB;
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PB;
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PB;
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PB;
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PB;

	EXTI->IMR |= EXTI_IMR_MR4; //UNMASK EXTI0
	EXTI->RTSR |= EXTI_RTSR_TR4; // trigger on rising edge
	EXTI->RTSR |= EXTI_FTSR_TR4; // trigger on falling edge

	EXTI->IMR |= EXTI_IMR_MR5; //UNMASK EXTI1
	EXTI->RTSR |= EXTI_RTSR_TR5; // trigger on rising edge
	EXTI->FTSR |= EXTI_FTSR_TR5; // trigger on falling edge

	EXTI->IMR |= EXTI_IMR_MR6;
	EXTI->RTSR |= EXTI_RTSR_TR6;
	EXTI->FTSR |= EXTI_FTSR_TR6;


	EXTI->IMR |= EXTI_IMR_MR7;
	EXTI->RTSR |= EXTI_RTSR_TR7;
	EXTI->FTSR |= EXTI_FTSR_TR7;

	EXTI->IMR |= EXTI_IMR_MR8;
	EXTI->RTSR |= EXTI_RTSR_TR8;
	EXTI->FTSR |= EXTI_FTSR_TR8;

	NVIC_EnableIRQ(EXTI4_15_IRQn); //enable interrupts for exti line 4 to 15

}

void init_PWM(void){ //using TIM1 for PWM: use pa8 - pa11 for motor driver control lines
	// TODO: choose frequency

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	//set frequency
	TIM1->PSC = 23;
	TIM1->ARR = 3999;	// PWM freqeuncy = 500Hz

	// Set PWM mode: OCxM bits in CCMRx
	TIM1->CCMR2 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // PWM Mode 1 on ch1
	TIM1->CCMR2 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1); // PWM Mode 1 on ch2
	TIM1->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1); // PWM Mode 1 on channel3
	TIM1->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1); // PWM Mode 1 on channel4

	//set initial duty cycle
	TIM1->CCR1 = 100 * 10;
	TIM1->CCR2 = 100 * 10;
	TIM1->CCR3 = 100 * 10; // Duty Cycle for PA10
	TIM1->CCR4 = 100 * 10; // Duty Cycle for PA11

	// Enable the OC channels
	TIM1->CCER |= TIM_CCER_CC1E;
	TIM1->CCER |= TIM_CCER_CC2E;
	TIM1->CCER |= TIM_CCER_CC3E;
	TIM1->CCER |= TIM_CCER_CC4E;

	// Enable the Counter
	TIM1->CR1 |= TIM_CR1_CEN;

}


//========================
//IMPLMENT FUNCTIONS
//========================
void brake(void){
	TIM1->CCR1 = 100*100;
	TIM1->CCR2 = 100*100;
	TIM1->CCR3 = 100*100;
	TIM1->CCR4 = 100*100;
}

void drive(int speed){ //speed ~ duty cycle between [0; 100]
	//TODO figure out calibration

	//left wheel
	TIM1->CCR1 = 100*speed;
	TIM1->CCR2 = 0;


	//right wheel
	TIM1->CCR3 = 100*speed;
	TIM1->CCR4 = 0;

}

void turn(direction d){
	//TODO decide what optimum turning speed
	brake();
	int speed = 10;

	switch(d){
	case LEFT:
		//left wheel
		TIM1->CCR1 = 0;
		TIM1->CCR2 = 100*speed;
		//right wheel
		TIM1->CCR3 = 100*speed;
		TIM1->CCR4 = 0;
		break;
	case RIGHT:
		//left wheel
		TIM1->CCR1 = 100*speed;
		TIM1->CCR2 = 0;
		//right wheel
		TIM1->CCR3 = 0;
		TIM1->CCR4 = 100*speed;
		break;
	default: //indicate error
		turnAround(4);
	}

}

void turnAround(){


}

//========================
//INTEERRUPT HANDLERS8
//========================

void EXTI4_15_IRQHANDLER(){ //controls lines 4 to 15: mapped to inputs from sensors (PB4-PB8)


}












