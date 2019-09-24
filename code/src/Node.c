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
void insertFirst(uint8_t key, uint8_t coordinates[2], junction_type j_type, uint8_t explored, cardinal* path_cardinals[4]){

	struct Node* newNode =  (struct Node*) malloc(sizeof(struct Node));
	newNode->key = key;
	newNode->coordinates[0] = coordinates[0];
	newNode->coordinates[1] = coordinates[1];
	//memcpy(newNode->coordinates, coordinates, sizeof(coordinates)/sizeof(coordinates[0]));
	newNode->j_type = j_type;
	newNode->explored = explored;
	newNode->path_cardinals[0] = path_cardinals[0];
	newNode->path_cardinals[1] = path_cardinals[1];
	newNode->path_cardinals[2] = path_cardinals[2];
	newNode->path_cardinals[3] = path_cardinals[3];


	newNode->prev = NULL;
	newNode->next = head;
	head = newNode;
	total_nodes++;
}

/*Insert new node at end of linked list*/
void insertLast(uint8_t key, uint8_t coordinates[2], junction_type j_type, uint8_t explored, cardinal *path_cardinals[4]){
	struct Node* newNode =  (struct Node*) malloc(sizeof(struct Node));
	newNode->key = key;

	newNode->coordinates[0] = coordinates[0];
	newNode->coordinates[1] = coordinates[1];

	newNode->j_type = j_type;
	newNode->explored = explored;

	newNode->path_cardinals[0] = path_cardinals[0];
	newNode->path_cardinals[1] = path_cardinals[1];
	newNode->path_cardinals[2] = path_cardinals[2];
	newNode->path_cardinals[3] = path_cardinals[3];

	newNode->prev = tail;
	newNode->next = NULL;
	tail = newNode;
	total_nodes++;
}

/*delete and return first (head) element in the list
 * use free() after done with returned value
 * */
struct Node* deleteFirst(){
	struct Node* link = head;
	head = link->next;
	total_nodes--;
	return link;
}

/*delete and return last (tail) element in the list
 * use free() after done with returned value
 * */
struct Node* deleteLast(){
	struct Node* templink = tail; //this is the current last Node
	tail = templink->prev;
	total_nodes--;
	return templink;
}


/*Return the Node with the specified key*/
struct Node* find(uint8_t key){
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
struct Node* delete(uint8_t key){
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
	total_nodes--;
	return current;
}


/*Find if a Node has the same x, y coordinates
 * Will return NULL if not found
 * Will return the Node with the same coordinates if found
 * */
struct Node* find_by_coordinates(uint8_t x, uint8_t y){
	struct Node* current = head;

	while(current->coordinates[0] != x && current->coordinates[1] != y){
		current = current->next;
	}

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
