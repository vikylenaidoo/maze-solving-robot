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

struct Node* head;

//========================
//MAIN IMPLEMENTATION
//========================
void main(void){
	init_LCD();
	lcd_command(CLEAR);
	lcd_putstring("Start");
	//head =  (struct Node*) malloc(sizeof(struct Node));
	init_GPIOA();
	init_GPIOB();
	init_PWM();
	lcd_command(LINE_TWO);
	lcd_putstring("initialisation");
	//printf("initialisation done \n");
	while(true){


	}

}











 //========================
 //INITIALIZATION FUNCTIONS
 //========================


void init_GPIOA(void){ //used for outputs
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	//SET PINS TO INPUT MODE
		GPIOA->MODER &= ~GPIO_MODER_MODER4;
		GPIOA->MODER &= ~GPIO_MODER_MODER5;
		GPIOA->MODER &= ~GPIO_MODER_MODER6;
		GPIOA->MODER &= ~GPIO_MODER_MODER7;
		GPIOA->MODER &= ~GPIO_MODER_MODER8;


		//CONFIGURE INTERRUPTS
		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN; //enable clock for syscfg

		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PA; //map pa4 to exti4
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PA; //map pa5 to exti5
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PA;
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PA;
		SYSCFG->EXTICR[3] |= SYSCFG_EXTICR3_EXTI8_PA;


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


}


/* USE LINES PB4 TO PB8 FOR SENSOR INPUTS
 * USE LINES PA8 TO PA11 FOR OUTPUTS TO MOTOR DRIVER
 *
 * */
void init_GPIOB(void){// used for inputs

	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	//use pb0@tim3_ch3 pb1@tim3_ch4 pb4@tim3_ch1 pb5@tim3_ch2 //af1
	GPIOB->MODER |= GPIO_MODER_MODER0_1; //
	GPIOB->MODER |= GPIO_MODER_MODER1_1; //
	GPIOB->MODER |= GPIO_MODER_MODER4_1;
	GPIOB->MODER |= GPIO_MODER_MODER5_1;

	GPIOB->AFR[0] |= 0b0001; //
	GPIOB->AFR[0] |= 0b0001 << (4*1);  //
	GPIOB->AFR[0] |= 0b0001 << (4*4);
	GPIOB->AFR[0] |= 0b0001 << (4*5);


}

/* using TIM1 for PWM: use pa8 - pa11 for motor driver control lines
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
		turnAround(4); //or flash leds maybe?
	}

	sleep(2); //
	brake();

}

void turnAround(int k){
	int speed = 10;
	//left wheel
	TIM1->CCR1 = 0;
	TIM1->CCR2 = 100*speed;
	//right wheel
	TIM1->CCR3 = 100*speed;
	TIM1->CCR4 = 0;

	sleep(5*k); //turn for 5 seconds
	brake();

}

//========================
//INTEERRUPT HANDLERS8
//========================

/*controls lines 4 to 15: mapped to inputs from sensors (PB4-PB8)*/
void EXTI4_15_IRQHandler(){
	//TODO: confirm sensor topological config. Does the tail swing alot?

	// 1st interrupt will trigger and disable other interruots until the PR flag is reset;

		//lines pa4 to pa8 used for sensor inputs
		int s1 = (GPIOA->IDR & GPIO_IDR_4)>>4; //s1==0 means line is low ; s1==1 means line is high
		int s2 = (GPIOA->IDR & GPIO_IDR_5)>>5;
		int s3 = (GPIOA->IDR & GPIO_IDR_6)>>6;
		int s4 = (GPIOA->IDR & GPIO_IDR_7)>>7;
		int s5 = (GPIOA->IDR & GPIO_IDR_8)>>8;

		lcd_command(CLEAR);
		lcd_putstring("interrupt");
		//printf("interrupt\n");
		//GPIOA->ODR &= ~(GPIO_ODR_8|GPIO_ODR_9|GPIO_ODR_10|GPIO_ODR_11);//clear bits 8 to 11 for ODR

		TIM3->CCR1 = 1 * 30;
		TIM3->CCR2 = 1 * 30;
		TIM3->CCR3 = 1 * 30;
		TIM3->CCR4 = 1 * 30;
		//GPIOA->ODR &= ~GPIO_ODR_0;


		if(s1){
			//printf("s1:%d \n", s1);
			TIM3->CCR1 = 40 * 100;
			lcd_command(LINE_TWO);
			lcd_putstring("high");
			//GPIOA->ODR |= GPIO_ODR_0;
		}
		if(s2){
			TIM3->CCR2 = 40 * 100;
			lcd_command(LINE_TWO);
			lcd_putstring("high");
			//printf("s2:%d \n", s2);
			//GPIOA->ODR |= GPIO_ODR_0;
		}
		if(s3){
			TIM3->CCR3 = 40 * 100;
			lcd_command(LINE_TWO);
			lcd_putstring("high");
			//printf("s3:%d \n", s3);
			//GPIOA->ODR |= GPIO_ODR_0;
		}
		if(s4){
			TIM3->CCR4 = 40 * 100;
			lcd_command(LINE_TWO);
			lcd_putstring("high");
			//printf("s4:%d \n", s4);
			//GPIOA->ODR |= GPIO_ODR_0;
		}
		if(s5){
			//GPIOA->ODR = GPIO_ODR_8|GPIO_ODR_9|GPIO_ODR_10|GPIO_ODR_11;
			TIM3->CCR1 = 40 * 100;
			TIM3->CCR2 = 40 * 100;
			TIM3->CCR3 = 40 * 100;
			TIM3->CCR4 = 40 * 100;
			lcd_command(LINE_TWO);
			lcd_putstring("high s5");
			//printf("s5:%d \n", s5);
			//GPIOA->ODR |= GPIO_ODR_0;

		}

		//int irq = NVIC_GetPendingIRQ(EXTI4_15_IRQn);
		//printf("irq: %d\n", irq);
		EXTI->PR |= EXTI_PR_PR0; //clear the interrupt
		//NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
}












