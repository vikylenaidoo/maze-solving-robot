/*
 * Node.c
 *
 *  Created on: 19 Sep 2019
 *      Author: vikylenaidoo
 */
#include "maze_solver.h"
#include "Node.h"



//=====================================
//IMPLEMENT NODE FUNCTIONS
//===================================

//NOTE: "first" = head	"last" = tail

/*Insert new node at beginning of linked list*/
void insertFirst(int key, int coordinates[2], junction_type j_type, int explored){

	struct Node* newNode =  (struct Node*) malloc(sizeof(struct Node));
	newNode->key = key;
	newNode->coordinates[0] = coordinates[0];
	newNode->coordinates[1] = coordinates[1];
	//memcpy(newNode->coordinates, coordinates, sizeof(coordinates)/sizeof(coordinates[0]));
	newNode->j_type = j_type;
	newNode->explored = explored;

	newNode->prev = NULL;
	newNode->next = head;
	head = newNode;
}

/*Insert new node at end of linked list*/
void insertLast(int key, int coordinates[2], junction_type j_type, int explored){
	struct Node* newNode =  (struct Node*) malloc(sizeof(struct Node));
	newNode->key = key;
	newNode->coordinates[0] = coordinates[0];
	newNode->coordinates[1] = coordinates[1];
	newNode->j_type = j_type;
	newNode->explored = explored;

	newNode->prev = tail;
	newNode->next = NULL;
	head = newNode;
}

/*delete and return first (head) element in the list*/
struct Node* deleteFirst(){
	struct Node* link = head;
	head = link->next;

	return link;
}

/*delete and return last (tail) element in the list*/
struct Node* deleteLast(){
	struct Node* link = tail; //this is the current last Node
	tail = link->prev;

	return link;
}


/*Return the Node with the specified key*/
struct Node* find(int key){
	struct Node* current = head;

	while(current->key != key){
		if(current->next == NULL){ //if we reach the end of the list
			return NULL;
		}
		current = current->next;

	}

	return current;
}



/*delete and Return the Node with the specified key TODO: Maybe free(current) to clear memory?*/
struct Node* delete(int key){
	//start at beginning
	struct Node* current = head;
	struct Node* previous = current->prev; //should be NULL

	while(current->key != key){
		if(current->next == NULL){ //if we reach the end of the list
			return NULL;
		}
		previous = current;
		current = current->
				next;
	}

	previous->next = current->next;
	current->next->prev = previous;

	return current;
}



/*find and return the next node*/
struct Node* findNext(struct Node *current){
	return current->next;

}


/*find and return the prev node*/
struct Node* findPrev(struct Node *current){
	return current->prev;
}
