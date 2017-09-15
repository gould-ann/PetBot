
if(hunger >= 60){
	result = EAT;
}
if(thirst >= 60){
	result = DRINK;
}
if(tiredness >= 60){
	result = EAT + DRINK;
}
if(health <= 0){
	result = DIE;
}
if(boredom >= 60){
	result = EXPLORE;
}