



void printEmotions(emotions *eList){
	lcd_home_line1(void);
	lprintf("Health:%d/t", health);
	lprintf("Hapiness:%d", hapiness);
	lcd_home_line2(void)
	lprintf("Hunger:%d/t", hunger);
	lprintf("Thirst:%d", thirst);
	lcd_home_line3(void);
	lprintf("Tiredness:%d/t", tiredness);
	lprintf("Boredom:%d", boredom);
}