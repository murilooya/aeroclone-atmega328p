//****************** Biblioteca LCD 16X2 ATMEGA328 **********************
//    Autor: Bruno Pohlot Ricobom
//    Pinos utilizados:
//    D7...D4:	PD7...PD4
//    RS:	PB1
//    E:	PB0
//********************************************************************
//    DATA      |   Descrição
//********************************************************************
// 29/05/2021   |   Criação da biblioteca   
//              |
//	            |
//              | 
//              | 	
//              |
//              | 
//----------------------------------------------------------------------

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include "lcd_16X2_ATMEGA328.h"

#define RS 1
#define E  0

void  pulso_E()
{
	PORTB &= ~(1<<E);
	PORTB |= (1<<E);
	PORTB &= ~(1<<E);
	return;
}

void envia_dados(unsigned char comando)
{
	PORTD = ((comando & 0xF0) | (PORTD & 0x0F));
	pulso_E();
	
	PORTD = (((comando << 4) & 0xF0) | (PORTD & 0x0F));
	pulso_E();
	return;
}

void Lcd_cmd(unsigned char comando)
{
	_delay_ms(1);               // Temporizar 1ms
	PORTB &= ~(1<<RS);          // RS = 0
	envia_dados(comando);
	return;
}


void Lcd_out(char linha_lcd, char coluna_lcd, char *ponteiro)
{
	//Seta endereço da DDRAM
	if((linha_lcd > 0) && (linha_lcd < 3))  // verifica se a linha é valida
	if((coluna_lcd > 0) && (coluna_lcd < 41))  //verifica se a coluna é valida
	{
		Lcd_cmd(128 + (coluna_lcd - 1) + ((linha_lcd - 1) * 64));
		//Envia o texto
		while (*ponteiro)
		{
			_delay_ms(1);               // Temporizar 1ms
			PORTB |= (1<<RS);           // RS = 1
			envia_dados(*ponteiro);
			ponteiro++;
		}
	}
	return;
}


void  Lcd_init()
{
	//configura portas de saida
	DDRB = DDRB | ((1<<E) + (1<<RS));   // PB1 e PB0 como saida (RS/E)
	DDRD = DDRD | ((1<<PD4)+(1<<PD5)+(1<<PD6)+(1<<PD7));  // PD7 a PD4 Como saida (D7...D4)
	          
	
	//inicialiaza o lcd
	PORTB &= ~(1<<RS);                                   // RS = 0
	_delay_ms(15);                                       // Temporizar 15 ms
	PORTD = ((0x30 & 0xF0) | (PORTD & 0x0F));            // Enviar 0x30 ao LCD
	pulso_E();                                           // Pulso no pino E
	
	_delay_ms(5);                                        // Temporizar 5 ms
	pulso_E();                                           // Pulso no pino E
	
	_delay_ms(1);                                        // Temporizar 1 ms
	pulso_E();                                           // Pulso no pino E
	
	_delay_ms(1);                                        // Temporizar 1 ms
	PORTD = ((0x20 & 0xF0) | (PORTD & 0x0F));            // Enviar 0x20 ao LCD
	pulso_E();                                           // Pulso no pino E
	
	Lcd_cmd(0x28);                                      //Especifica a interface de comunicação 4 bits, 2 linhas 5X8 pixels
	Lcd_cmd(0x08);                                      // Desliga o display
	Lcd_cmd(0x01);                                      // Limpa o diplay
	Lcd_cmd(0x06);                                      // Não desloca o display, incrementa o AC (posição do cursor)
	Lcd_cmd(DISPLAY_ON);
	
	return;

}



