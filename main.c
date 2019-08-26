

#include <avr/io.h>
#include "glcd/glcd.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "glcd/fonts/Liberation_Sans15x21_Numbers.h"
#include "glcd/fonts/font5x7.h"
#include <avr/pgmspace.h>
#define F_CPU 16000000UL  // 1 MHz
#define	LINKS	!(PIND &(1<<PD6))
#define RECHTS	!(PIND &(1<<PD5))
int main(void)
{
	char Richtung;
	char hoehe;
	
	DDRB|=(1<<PB2);
	DDRC|=(1<<PC3);
	PORTC&=~(1<<PC3);

	
	DDRD&=~((1<<PD5)|(1<<PD6)|(1<<PD2));
	PORTD|=((1<<PD5)|(1<<PD6)|(1<<PD2));
	Richtung=35;
	hoehe=0;
	
	sei();

	glcd_init();
	glcd_clear();
	glcd_write();
	glcd_tiny_set_font(Font5x7 ,5,7,32,127);
	glcd_clear_buffer();

	glcd_draw_rect(00,00,84,47,1);
	glcd_draw_rect(48,42,15,3,1);
	
	glcd_write();	
		
		
		while(1)
		{
			if(LINKS)
			{
				glcd_clear();
				
				Richtung--;
				glcd_draw_rect(Richtung,42,15,3,1);
				glcd_write();
				
				if(Richtung <= 1)
				{
					Richtung = 2;
				}
			}
			if(RECHTS)
			{
				glcd_clear();
				Richtung++;
				glcd_draw_rect(Richtung,42,15,3,1);
				glcd_write();
				if(Richtung >= 68)
				{
					Richtung = 67;
				}
			}
			/*if(!(PIND&(1<<PD2)))
			{												//später hinzufügen (Höhe)
				while(!(PIND&(1<<PD2)))
				glcd_clear();
				hoehe--;
				glcd_draw_rect(Richtung,hoehe,15,3,1);
				glcd_write();
				if(hoehe<30)
				{
					hoehe=29;
				}
				
				
			}*/
				
			
				
			glcd_draw_rect(00,00,84,47,1);
			glcd_write();
			
		}
				
	
}