/*
 * petRobot.c
 *
 * Created: 2/28/2017 5:40:10 PM
 */ 

 #include "main.h"

//actions list: EXPLORE, EAT, DRINK, SLEEPS, SOLVE, BARK, HURT_BRYAN, DIE, FLEE
int main(void)
{
	int number = rand() % numOfMoveFunctions;
	int timerCheck = 0, milli = 0;
	double lastShortUpdate = 0;
	double lastLongUpdate = 0;


	lcd_init();
	lcd_clear();

	oi_t *sensor_data = oi_alloc();
	oi_init(sensor_data);
   
	emotions dogEmotions;
	initalize(&dogEmotions);
	unsigned char notes[] = {81, 76};
	unsigned char duration[] = {16, 8};
    oi_load_song(0, 2, notes, duration);
	while (sensor_data->bumper_left == 0) 
    {
		oi_update(sensor_data);
		wait_ms(10);
		timerCheck += 10;
		milli += 10;
		if(timerCheck >= 200)
		{
			printEmotions(&dogEmotions);
			number = rand() % numOfMoveFunctions;

			//choose a movement function between 0 and upper bound
			chooseToMove(number, sensor_data);	

			//updateEmotions, based on the current sensor values it will look to see if any emotions need to be updated
			updateEmotions(&dogEmotions, sensor_data, getSeconds(milli), &lastShortUpdate, &lastLongUpdate);

			//checkForAction based on the emotions check to see if any have reached the threshold for an action these are set with if statements
			//doAction receives the action from the checkForAction. Then in the do action there is a case statement that holds the functions for the actions
			doAction(checkForAction(&dogEmotions));

			timerCheck = 0;
		}

    }
	lprintf("eStoped");
}
void initalize(emotions *eList)
{
	eList->hunger = 0;
	eList->thirst = 0; 
	eList->happiness = 100;
	eList->fear = 0;
	eList->anger = 0;
	eList->tiredness = 10;
	eList->boredom = 15;
	eList->health = 100;
}
void chooseToMove(int num, oi_t *sensor)
{
	//lprintf("accessed: %d", num);
	switch(num)
	{
		case 1:
		turn(sensor, 10);
		break;
		case 2:
		moveForward(sensor, 100);
		break;
		case 3:
		moveForward(sensor, 10);
		turn(sensor, 10);
		break;
	}
}
void updateEmotions(emotions *eList, oi_t *sensor, double seconds, double *lastShortUpdate, double *lastLongUpdate)
{
		if((seconds - *lastShortUpdate) > 10){ //updates every 10 seconds
			*lastShortUpdate = (int)seconds;
			if(eList->hunger >= 80){
				eList->health -= 3;
			}
			if(eList->thirst >= 80){
				eList->health -= 3;
			}
			if(eList->tiredness >= 80){
				eList->happiness -= 5;
				eList->anger += 3;
			}
		}
		
		if((seconds - *lastLongUpdate) > 15){ //updates every 45 seconds
			*lastLongUpdate = seconds;
			eList->hunger += (rand() % 4) + 1;  //(between 1 and 4 hunger per 45 seconds)
			eList->thirst += (rand() % 5); //gets thirsty kinda often (between 0 and 4 thirst per 45 secs)
			eList->tiredness += (rand() % 4) + 3; //gets tired the fastest (between 3 and 6 tiredness per 45 seconds)
			eList->fear -= 2;
		}
}

actions checkForAction(emotions* eList)
{
	actions result = NONE;
	result = BARK;
	if(eList->hunger >= 60){
		result = EAT;
	}
	if(eList->thirst >= 60){
		result = DRINK;
	}
	if(eList->tiredness >= 60){
		result = EAT + DRINK;
	}
	if(eList->health <= 0){
		result = DIE;
	}
	if(eList->boredom >= 60){
		result = EXPLORE;
	}
	return result; 
}

void doAction(actions currAction)
{
		switch(currAction)
		{
			case EXPLORE: 
				break;
			case EAT:
				break;
			case DRINK:
				break;
			case SLEEPS:
				break;
			case SOLVE:
				break;
			case BARK:
				bark(1);
				break;
			case HURT_BRYAN:
				break;
			case DIE:
				break;
			case FLEE:
				break;
			case NONE:
				break;
		}
}

double getSeconds(int milliseconds)
{
	return ((double)milliseconds) / 100;
}

void printEmotions(emotions *eList){
	lcd_home_line1();
	lprintf("Helth:%3d Happy:%3d\nHungr:%3d Thrst:%3d\nTired:%3d Bored:%3d",eList->health, eList->happiness, eList->hunger,eList->thirst, eList->boredom, eList->tiredness);
}

void bark(int numBarks)
	{
		int i;
		for(i = 0; i < numBarks; i++)
		{
			oi_play_song(0);
		}
	}


