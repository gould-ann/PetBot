int lastShortUpdate = 0;
int lastLongUpdate = 0;

void updateEmotions(emotions e, int seconds, bool bumpedWall){ //after bumpedWall has been passed in TRUE once, should not be passed in true again until a completely separate bump has happened
	if((seconds - lastShortUpdate) > 10){ //updates every 10 seconds
		lastShortUpdate = seconds;
		if(e.hunger >= 80){
			e.health -= 3;
		}
		if(e.thirst >= 80){
			e.health -= 3;
		}
		if(e.tiredness >= 80){
			e.happiness -= 5;
			e.anger += 3;
		}
	}
	
	if((seconds - lastLongUpdate) > 45){ //updates every 45 seconds
		lastLongUpdate = seconds;
		e.hunger += (rand() % 4) + 1;  //(between 1 and 4 hunger per 45 seconds)
		e.thirst += (rand() % 5); //gets thirsty kinda often (between 0 and 4 thirst per 45 secs)
		e.tiredness += (rand() % 4) + 3; //gets tired the fastest (between 3 and 6 tiredness per 45 seconds)
		e.fear -= 2;
	}
}