#define F_CPU 16000000

#include <avr/io.h>
#include "lcd_16X2_ATMEGA328.h"
#include "ds18b20.h"

#include <stdio.h>
#include <stdbool.h>

int water_temperature;
int ref_temperature = 18;
int histerese = 2;

char buffer [20];

void writeDisplay(){
	sprintf(buffer, "Refe: %d C", ref_temperature);
	Lcd_out (1,1, buffer);
	sprintf(buffer, "Agua: %d C", water_temperature);
	Lcd_out(2,1, buffer);
}

bool verifyHisteresis()
{
	if (water_temperature <= (ref_temperature + histerese) && water_temperature >= (ref_temperature - histerese))
	{
		return true;
	}
	return false;
}

void turnOnPump()
{
	PINC |= (1<<PB0);
}

void turnOnPeltier()
{
	PINC |= (1<<PB2);
}

void turnOffPeltier()
{
	PINC &= ~(1<<PB2);
}

int main(void)
{
    //setup
	
	DDRC |= (1<<PB0); //output pump pin
	DDRC |= (1<<PB2); //output peltier A side
		
	configura_precisao(12);		

	PINC &= ~((1<<PB0) + (1<<PB1) + (1<<PB2)); //zero logic output pin
	
	Lcd_init(); //Lcd setup

	turnOnPump();
	
    while (1) 
    {
		water_temperature = le_temperatura();
		writeDisplay();
		if ((water_temperature <= ref_temperature) && !verifyHisteresis())
		{
			turnOnPeltier();
		}
		else{
			turnOffPeltier();
		}
    }
}

