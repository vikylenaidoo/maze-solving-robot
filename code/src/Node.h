/*
 * Node.h
 *
 *  Created on: 19 Sep 2019
 *      Author: vikylenaidoo
 */

#ifndef NODE_H_
#define NODE_H_

#include "maze_solver.h"


//=============================
//CREATE STRUCT
//======================

struct Node{

	uint8_t key; //unique identifier /primary key

	uint8_t coordinates[2]; // x and y coordinates
	junction_type j_type;
	uint8_t explored; //how many times has this been differently explored

	cardinal* path_cardinals[4]; //{W, N, E, S} dependin on type. if cardinal not available for this node then set to NULL;

	struct Node *prev;
	struct Node *next; //links to the next node in the linked list

};






//================================
//DECLARE NODE FUNCTIONS
//============================

void insertFirst(uint8_t key, uint8_t coordinates[2], junction_type j_type, uint8_t explored, cardinal* path_cardinals[4]);
void insertLast(uint8_t key, uint8_t coordinates[2], junction_type j_type, uint8_t explored, cardinal* path_cardinals[4]);
struct Node* deleteFirst();
struct Node* delete(uint8_t key);
struct Node* find(uint8_t key);
struct Node* find_by_coordinates(uint8_t x, uint8_t y);
struct Node* findNext();
struct Node* findPrev();






#endif /* NODE_H_ */
