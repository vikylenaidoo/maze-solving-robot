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

	int key; //unique identifier /primary key

	int coordinates[2]; // x and y coordinates
	junction_type j_type;
	int explored; //how many times has this been differently explored

	struct Node *prev;
	struct Node *next; //links to the next node in the linked list

};






//================================
//DECLARE NODE FUNCTIONS
//============================

void insertFirst(int key, int coordinates[2], junction_type j_type, int explored);
void insertLast(int key, int coordinates[2], junction_type j_type, int explored);
struct Node* deleteFirst();
struct Node* delete(int key);
struct Node* find(int key);
struct Node* findNext();
struct Node* findPrev();






#endif /* NODE_H_ */
