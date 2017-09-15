/*
 * main.h
 *
 * Created: 2/28/2017 5:58:04 PM
 */ 
 //This should contain a struct declaration for the statistics
 //Contains the prototypes for the functions that are used in main
 //Contains other include.h files

 #include <avr/io.h>
 #include <stdlib.h>
 #include "open_interface.h"
 #include "lcd.h"
 #include "util.h"
 #include "movement.h"
 #include <avr/interrupt.h>
 

#ifndef MAIN_H_
#define MAIN_H_
#define timeInciment 0.5
#define numOfMoveFunctions 3

//define the thresholds for each action

typedef enum {EXPLORE, EAT, DRINK, SLEEPS, SOLVE, BARK, HURT_BRYAN, DIE, FLEE, NONE} actions;


typedef struct 
{
	int hunger;
	int thirst;
	int happiness;
	int fear;
	int anger;
	int tiredness;
	int boredom;
	int health;
} emotions;



/*
*	initializes all emotions
*/
void initalize(emotions *eList);

/*
* selects a move function from the case statement with the given number
*/
void chooseToMove(int num, oi_t *sensor);

/*
* updates the emotions if the sensor values, or the time falls within a given range
*/
void updateEmotions(emotions *eList, oi_t *sensor, double seconds, double *lastShortUpdate, double *lastLongUpdate);

/*
* looks if any actions need to occur and returns it
*/
actions checkForAction(emotions *eList);

/*
* takes in an action and based on a case statement it will run the appropriate function for that action
*/
void doAction(actions currAction);

/*
* Prints out the relevant variables from the emotions struct
*/
void printEmotions(emotions *eList);

/*
* returns the number of milliseconds in seconds
*/
double getSeconds(int milliseconds);

void bark(int numBarks);

/*
* chooses a movement patter from a list of movement
void movmentPatter(int movement, long time);
*/

#endif /* MAIN_H_ */
