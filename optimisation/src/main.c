/*
 * main.c
 *
 *  Created on: 25 Sep 2019
 *      Author: vikylenaidoo
 */
#include "main.h"
#include <stdio.h>


//direction directions_list[] = {LEFT, BACKWARDS, LEFT, LEFT, LEFT, BACKWARDS, FORWARD, BACKWARDS ,LEFT, LEFT, BACKWARDS, FORWARD, LEFT, LEFT};


//direction optimal_path[64] = {STOP};
int optimal_counter = 0;
int length_directions;
//int counter;
int iter;

int main(void){

/*
	directions_list[0] = LEFT;
	directions_list[1] = BACKWARDS;
	directions_list[2] = LEFT;
	directions_list[3] = LEFT;
	directions_list[4] = LEFT;
	directions_list[5] = BACKWARDS;
	directions_list[6] = FORWARD;
	directions_list[7] = BACKWARDS;
	directions_list[8] = LEFT;
	directions_list[9] = LEFT;
	directions_list[10] = BACKWARDS;
	directions_list[11] = FORWARD;
	directions_list[12] = LEFT;
	directions_list[13] = LEFT;
*/

	direction directions_list[] = {LEFT, BACKWARDS, LEFT, LEFT, LEFT, BACKWARDS, FORWARD, BACKWARDS ,LEFT, LEFT, BACKWARDS, FORWARD, LEFT, LEFT};
	length_directions = 14;
	iter = 0;

	direction temp_array[64] = {STOP};
	//counter=0;
	//optimise(directions_list, temp_array);

	//while(1){}

	printArray(directions_list);
	int size = (int)(sizeof(directions_list)/sizeof(directions_list[0]));
	printf("size: %d \n", size);
	printf("\n done");
	return 0;

}

void printArray(direction* array){
	for(int i=0; i<(int)(sizeof(array)/sizeof(array[0])); i++){
		printf("%d \n", (int)array[i]);
	}

	int size = (int)(sizeof(array)/sizeof(array[0]));
	printf("size: %d\n", size);
	printf("array CoMplete \n\n");

}



void clearArray(direction * array){
	for(int i=0; i<sizeof(array)/sizeof(direction); i++){
		array[i] = STOP;
	}

}

void optimise(direction* input_array, direction* output_array){
		clearArray(&output_array[0]);
		int counter = 0;

		direction combo[3] = {STOP};
		direction newDirection = STOP;

		if(sizeof(input_array)/sizeof(direction) < 3){
			return;
		}

		for(int i=0; i<3; i++){
			combo[i] = input_array[i];
		}

		if(combo[3]==STOP){ //exit case

			return;
		}

		if(combo[0]==LEFT && combo[1]==BACKWARDS && combo[2]==RIGHT){ //B
			newDirection = BACKWARDS;

		}
		else{
			if(combo[0]==LEFT && combo[1]==BACKWARDS && combo[2]==FORWARD){//R
				newDirection = RIGHT;

			}
			else{
				if(combo[0]==RIGHT && combo[1]==BACKWARDS && combo[2]==LEFT){ //B
					newDirection = BACKWARDS;


				}
				else{
					if(combo[0]==FORWARD && combo[1]==BACKWARDS && combo[2]==LEFT){ //R
						newDirection = RIGHT;

					}
					else{
						if(combo[0]==FORWARD && combo[1]==BACKWARDS && combo[2]==FORWARD){ //B
							newDirection = BACKWARDS;

						}
						else{
							if(combo[0]==LEFT && combo[1]==BACKWARDS && combo[2]==LEFT){ //F
								newDirection = FORWARD;

							}
						}

					}

				}

			}

		}

		if(newDirection != STOP){ //put the better direction in the array
			output_array[counter] = newDirection;

		}
		else{
			for(int i=0; i<3; i++){ //put the original combination
				output_array[counter] = combo[i];
				counter++;
			}
			int i = 3;
			while(input_array[i] != STOP){
				output_array[counter] = input_array[i];
				i++;
				counter++;

			}

		}
		printArray(output_array);
		optimise(output_array, input_array);


}
