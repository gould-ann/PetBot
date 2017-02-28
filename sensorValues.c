/*
 * CFile1.c
 *
 * Created: 2/21/2017 5:23:22 PM
 *  Author: Talon
 */ 
#include "open_interface.h"
#import <stdio.h> 
#import <stdlib.h>

void main()
{
	oi_t *sensor_data = oi_alloc();
	oi_init(sensor_data);
	
	int number = 0;
	int cnt = 0;
	//update loop
	while(cnt < 20)
	{
		number = sensor_data->cliff_right;
		printf("%d", number);
		cnt ++;
	}
	oi_free(sensor_data)
}