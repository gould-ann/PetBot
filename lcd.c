/**
 * lcd.c: functions for displaying content to the LCD screen
 *
 * @date 06/26/2012
 */

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "lcd.h"


#define HD_LCD_CLEAR 0x01
#define HD_RETURN_HOME 0x02
#define HD_CURSOR_SHIFT_DEC 0x05
#define HD_CURSOR_SHIFT_INC 0x07
#define HD_DISPLAY_CONTROL 3
#define HD_DISPLAY_ON 2
#define HD_CURSOR_ON 1
#define HD_BLINK_ON 0
#define HD_CURSOR_MOVE_LEFT 0x10
#define HD_CURSOR_MOVE_RIGHT 0x14
#define HD_DISPLAY_SHIFT_LEFT 0x18
#define HD_DISPLAY_SHIFT_RIGHT 0x1C

#define LCD_WIDTH 20
#define LCD_HEIGHT 4
#define LCD_TOTAL_CHARS (LCD_WIDTH*LCD_HEIGHT)

void lcd_toggle_clear(char delay);
void lcd_home_anyloc(unsigned char location);

/// Initializes PORTA to communicate with LCD controller
void lcd_init(void) {
	const char enable=0x40; //PA6 is tied to Enable
	const char rs=0x10;		//PA4 is tied to Register Select
	//Assumes Port A is dedicated to the LCD
	//Seven Pins needed, but will assume all 8 are used
	DDRA=0xFF; //Setting Port A for OutPut
	 //Preparing to put HD44780 into 4-bit Mod
	PORTA=0x03;

	PORTA|=enable;
	wait_ms(1);
	PORTA&=~enable;
	wait_ms(5);
	PORTA|=enable;
	wait_ms(1);
	PORTA&=~enable;
	PORTA|=enable;
	wait_ms(1);
	PORTA&=~enable;

	PORTA=0x02;	//setting controller to 4 bit mode
				//Need to set for 2 lines
	lcd_toggle_clear(1);

	PORTA|=0x00;  //setting disp on, cursor on, blink off
	lcd_toggle_clear(1);
	PORTA|=0x0E;
	lcd_toggle_clear(1);

	PORTA|=0x00; //increment cursor, no display shift
	lcd_toggle_clear(1);
	PORTA|=0x06;
	lcd_toggle_clear(1);
	
	PORTA|=0x00; //clear LCD
	lcd_toggle_clear(1);
	PORTA|=0x01;
	lcd_toggle_clear(1);

	PORTA|=rs;	//Setting Register select high to enable character mode
	lcd_home_line1();
}



/// Triggers loading of bits by LCD controller and clears bits after toggle
void lcd_toggle_clear(char delay) {
	const char enable=0x40; //PA6 is tied to Enable

	PORTA|=enable;
	wait_ms(delay);
	PORTA&=~enable;
	PORTA&=0xF0;	
}


/// Submits command to LCD controller
void lcd_command(char data) {
	const char rs=0x10;		//PA4 is tied to Register Select
	PORTA&=~rs;  //Setting register select low for command mode
	PORTA|=(data>>4);
	lcd_toggle_clear(2);
	PORTA|=(data & 0x0F);
	lcd_toggle_clear(2);
	PORTA|=rs;	//Setting register select high for character mode
}



/// Clears the LCD
void lcd_clear(void) {
	lcd_command(HD_LCD_CLEAR);
	wait_ms(1);
}



/// Sets character position to first line first position
void lcd_home_line1(void) {
	lcd_command(HD_RETURN_HOME);
}



/// Sets character position to second line first position
void lcd_home_line2(void) {
	lcd_command(0xC0); //First bit relevant to the set DDRAM address instruction. The remaining bits designate address, which is 0x40.
}



/// Sets character position to third line first position
void lcd_home_line3(void) {
	lcd_command(0x94);	//First bit relevant to the set DDRAM address instruction. The remaining bits designate address, which is 0x14.
}



/// Sets character position to fourth line first position
void lcd_home_line4(void){ //First bit relevant to the set DDRAM address instruction. The remaining bits designate address, which is 0x54.
	lcd_command(0xD4); 
}



/// Sets character position to any valid location
void lcd_home_anyloc(unsigned char location) {
	if (location < 0x68) { //Addresses are limitted to 0x00 - 0x67
		location |= 0x80; //Setting the first bit to one, thus forming a DDRAM set address instruction
		lcd_command(location);
	} 
}



/// Shift display content left
void lcd_display_shift_left(void) {
	lcd_command(HD_DISPLAY_SHIFT_LEFT);
}



/// Prints string to lcd, starting at the current cursor position
void lcd_puts(char * string) {
	int size=strlen(string);
	int i;

	for (i=0;i<size;i++) {		
		lcd_putc(string[i]);
	}
}



/// Prints one character at the current cursor position
void lcd_putc(char data) {
	PORTA|=(data>>4);
	lcd_toggle_clear(1);
	PORTA|=(data & 0x0F);
	lcd_toggle_clear(1);
}

/// Print a formatted string to the LCD screen
/**
 * Mimics the C library function printf for writing to the LCD screen.  The function is buffered; i.e. if you call
 * lprintf twice with the same string, it will only update the LCD the first time.
 *
 * Google "printf" for documentation on the formatter string.
 *
 * Code from this site was also used: http://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html
 * @author Kerrick Staley & Chad Nelson
 * @date 05/16/2012
 */
void lprintf(const char *format, ...) {
	static char lastbuffer[LCD_TOTAL_CHARS + 1];

	char buffer[LCD_TOTAL_CHARS + 1];
	va_list arglist;
	va_start(arglist, format);
	vsnprintf(buffer, LCD_TOTAL_CHARS + 1, format, arglist);
	
	if (!strcmp(lastbuffer, buffer))
		return;
	
	strcpy(lastbuffer, buffer);
	lcd_clear();
	char *str = buffer;
	int charnum = 0;
	while (*str && charnum < LCD_TOTAL_CHARS) {
		if (*str == '\n') {
			/* fill remainder of line with spaces */
			charnum += LCD_WIDTH - charnum % LCD_WIDTH;
		} else {
			lcd_putc(*str);
			charnum++;
		}

		str++;
		
		/*
		 * The LCD's lines are not sequential; for future reference, the address are like
		 * 0x00...0x13 : line 1
		 * 0x14...0x27 : line 3
		 * 0x28...0x3F : random junk
		 * 0x40...0x53 : line 2
		 * 0x54...0x68 : line 4
		 * 
		 * The cursor position must be reset at the end of every line, otherwise, after writing line 1, it writes line 3 and then nothingness
		 */
		
		if (charnum % LCD_WIDTH == 0) { 
			switch (charnum / LCD_WIDTH) {
			case 1:
				lcd_home_line2();
				break;
			case 2:
				lcd_home_line3();
				break;
			case 3:
				lcd_home_line4();
			}
		}
	}
	va_end(arglist);
}
