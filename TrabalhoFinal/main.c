#define F_CPU 16000000

#include <avr/io.h>
#include "lcd_16X2_ATMEGA328.h"
#include <stdio.h>

float water_temperature;
float ref_temperature;

enum{	EQUAL = 0,
		HOT,
		COLD,
	};

uint8_t comparisonTag = EQUAL;  //1 heat up, 2 cool down, 0 equal

char buffer [20];

void refComparison(){
	if (water_temperature > ref_temperature){
		Lcd_out(1, 14, "-")
		comparisonTag = HOT;
		return;		
	}
	
	if (water_temperature < ref_temperature){
		Lcd_out(1, 14, "+");
		comparisonTag = COLD;
		return;
	}
	else{
		Lcd_out(1, 14, "=");
		comparisonTag = EQUAL;
		return
	}
}

void HBridgeController(uint8_t tag){
	switch(tag){
		case HOT:
			coolPeltier();
			break;
		case COLD:
			heatPeltier();
			break;
		case EQUAL:
			break;
		default:
			break;
	}
}

void turnOnPump(){
	PINC |= (1<<PB0); //one logic in pump pin
}

void enableHBridge(){
	PINC |= (1<<PB1); //enable h-bridge
}

void disableHBridge(){
	PINC &= ~(1<<PB1); //disable h-bridge
}

void writeDisplay(){
	sprintf(buffer, "Refe: %.2f C", ref_temperature);
	Lcd_out (1,1, buffer);
	sprintf(buffer, "Agua: %f C", water_temperature);
	Lcd_out(2,1, buffer);
}

int main(void)
{
    //setup
	
	DDRC |= (1<<PB0); //output pump pin
	DDRC |= (1<<PB1); //output enable h-bridge
	DDRC |= (1<<PB2); //output peltier
	
	DDRC |= (1<<PB3); //output h-brigde controller
	
	
	DDRC &= ~(1<<PB3); //input ds18b20
	PORTC &= ~(1<<PB3); //pull-up off
		
	PINC &= ~((1<<PB0) + (1<<PB1) + (1<<PB2) + (1<<PB3)); //zero logic output pin
	
	Lcd_init(); //Lcd setup
	
	
    while (1) 
    {
		DS18Read();
		writeDisplay();
		refComparison();
		HBridgeController(comparisonTag);
    }
}

