#define F_CPU 16000000

#include <avr/io.h>
#include <avr/delay.h>

#define dataPIN 3

void pulso_reset();
void pulso_presenca();
void escreve_zero();
void escreve_um();
char le_bit();
char le_byte();
void escreve_comando (char comando);

void configura_precisao(char precisao);
int le_temperatura();

void pulso_reset()
{
	DDRC |= (1<<dataPIN); //output ds18b20 pin
	PINC &= ~(1<<dataPIN); //coloca em nivel logico baixo
	_delay_us(500); //delay 500 us
	PINC |= (1<<dataPIN); //coloca em nivel alto
	_delay_us(60); //delay 60 us
}

void pulso_presenca()
{
	DDRC &= ~(1<<dataPIN); //input ds18b20 pin
	while (PINC & (1<<dataPIN)); //laco enquanto nivel baixo nao for detectado
	while (~PINC & (1<<dataPIN)); //laco enquanto nivel alto nao for detectado
	_delay_us(500); //delay 500 us	
}

void escreve_zero()
{
	DDRC |= (1<<dataPIN); //output
	PINC &= ~(1<<dataPIN); //coloca em nivel logico baixo
	_delay_us(90); //delay 90 us
	PINC |= (1<<dataPIN); //coloca em nivel alto
	_delay_us(10);
}

void escreve_um()
{
	DDRC |= (1<<dataPIN); //output
	PINC &= ~(1<<dataPIN); //coloca em nivel logico baixo
	_delay_us(5); //delay 5 us
	PINC |= (1<<dataPIN); //coloca em nivel alto
	_delay_us(90); //delay 90 us	
}

char le_bit()
{
	char _bit = 0;
	DDRC |= (1<<dataPIN); //output
	PINC &= ~(1<<dataPIN); //coloca em nivel logico baixo
	_delay_us(3);
	DDRC &= ~(1<<dataPIN); //input
	_delay_us(3);
	_bit = (PINC & (1<<dataPIN));
	_delay_us(90);
	_bit = (_bit>>dataPIN);
	return _bit;
}

char le_byte()
{
	char _byte = 0;
	for (int 1 = 0; i < 8; i++)
	{
		_byte = _byte | (le_bit() << i); 
	}
	return _byte;
}

void escreve_comando (char comando)
{
	for (int i=0; i < 8; i++)
	{
		if (comando & 0x01) 
		{
			escreve_um();
		}
		else 
		{
			escreve_zero();
		}
		comando = comando >> i;
	}
	_delay_us(10);
}

void configura_precisao (char precisao)
{
	char valor = 0;
	switch (precisao)
	{
		case 9:
			valor = 0;
			break;
		case 10:
			valor = 32;
			break;
		case 11:
			valor = 64;
			break;
		case 12:
			valor = 96;
			break;
		default:
			valor = 96;
	}
	pulso_reset();	//envia reset
	pulso_presenca(); //detecta presenca do sensor
	
	escreve_comando(0xCC); //skip ROM
	
	escreve_comando(0x4E); //comando de config
	escreve_comando(0x00); //registrador de alarme
	escreve_comando(0x00); //registrador de alarme
	
	escreve_comando(valor); //valor da precisao
	
}

int le_temperatura()
{
	int temperatura = 0;
	
	pulso_reset();	//envia reset
	pulso_presenca(); //detecta presenca do sensor
		
	escreve_comando(0xCC); //skip ROM
	
	escreve_comando(0x44);
	_delay_ms(800);
	
	pulso_reset();	//envia reset
	pulso_presenca(); //detecta presenca do sensor
		
	escreve_comando(0xCC); //skip ROM
	
	escreve_comando(0xBE);
	
	temperatura = le_byte();
	temperatura |= (int)(le_byte() << 8);
	
	pulso_reset();	//envia reset
	pulso_presenca(); //detecta presenca do sensor
	
	temperatura = temperatura >> 4; //ignora valores não inteiros
	
	return temperatura;
}