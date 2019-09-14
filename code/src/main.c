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
 //IMPLEMENT FUNCTIONS
 //========================


void init_GPIOA(void){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// configre  5 input lines fot the sensors
	GPIOA->MODER &= ~GPIO_MODER_MODER0;  // set to input mode
	GPIOA->MODER &= ~GPIO_MODER_MODER1;
	GPIOA->MODER &= ~GPIO_MODER_MODER2;
	GPIOA->MODER &= ~GPIO_MODER_MODER3;
	GPIOA->MODER &= ~GPIO_MODER_MODER4;


	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0; // confirm PUPD for each sensor
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR1_0;


	//configure interrupts
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN; //enable clock for syscfg

	// we are using PA4 to PA8 for sensors 1 to 5 (because they are all on the same interrupt handler line)
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PA;
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PA;
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PA;
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PA;
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PA;

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


void init_GPIOB(void){
	RCC  ->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER |= (GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0|GPIO_MODER_MODER2_0);
	GPIOB->ODR     = 0b0000000000000000;

}

void init_PWM(void){


}



//========================
//INTEERRUPT HANDLERS
//========================

void EXTI4_15_IRQHANDLER(){ //controls lines 4 to 15


}












