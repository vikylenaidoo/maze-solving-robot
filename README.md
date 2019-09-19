# maze-solving-robot
//TODO

Things to consider: // discuss before implementing it
-----------------------
*think about printing a log for each run (for debugging purposes)

*consider using a class to represent a Junction. then each junction will become an instance with:

    -coordinate
    
    -junction type (how many directions it has)
    
    -point counter
    
    -etc
    
    that means we only need to keep an array of Junctions rather than 4 seperate arrays



functionality
--------------
*GPIO initialisation

*interrupts
    
    -sensors
    
    -pushbuttons

*functions for movement control
    
    -PWM for motor drivers
    
    -turning left/right
    
    -stopping
    
    -going forward
    
    -reverse?

*Sensor logic:
    
    -check left turn
    
    -check right turn
    
    -check cross road
    
    -check T-junction
    
    -check end of maze (circle)
    
    -check drifting left
    
    -check drifting right
    
*function to measure distance based on time
    
    -each time goForward() is called reset a timing variable to zero. 
    
    -use time until next sensor direction interrupt to calculate a "distance"





 memory constructs
-------------------

*2D coordinate array
    
    -for mapping x-y cordinates of maze

    -think about specifying x-y cordinates in time rather than distance ?

*Junction type array
    
    -counts how many times the point needs to be explored based on the nature of the junction

*point/node array 
    
    -unique identifier for each node/point in the sequnce that they are exlored

*explored array
    
    -keeps record how many times a junction point has been differently explored by the robot.

*counting variable: total points discovered

*direction variable: N W E S = 0 1 2 3 (possibly)








optimisation
-------------

*Djikstra's algorithm










