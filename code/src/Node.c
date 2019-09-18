/*
 * Node.c
 *
 *  Create a struct to represent a node/ point of turning on the maze.
 *  A linked list of these nodes will be present in the main program to keep a record of all discovered
 */

#include "maze_solver.h";

//TODO

struct Node{

	int key; //unique identifier /primary key

	int coordinates[2]; // x and y coordinates
	junction_type j_type;
	int explored; //how many times has this been differently explored

	struct Node *next;

};




