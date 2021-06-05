#define F_CPU 16000000

#include <avr/io.h>

float water_temperature;

float ref_temperature;


void loopPeltier(){
	if (water_temperature > ref_temperature){
		coolPeltier();
	}
	
	if (water_temperature < ref_temperature){
		heatPeltier();
	}
}

int main(void)
{
    //setup
	
	DDRC |= ((1<<PB0) + (1<<PB1) + (1<<PB2)); //configure a0, a1 and a2 to output
	DDRC &= ~(1<<PB3); //configure a3 to input
	
	
	
    while (1) 
    {
    }
}

