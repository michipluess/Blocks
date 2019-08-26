

#include <avr/io.h>
#include "glcd/glcd.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "glcd/fonts/Liberation_Sans15x21_Numbers.h"
#include "glcd/fonts/font5x7.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#define F_CPU 16000000UL  // 1 MHz
#define	LINKS	!(PIND &(1<<PD6))
#define RECHTS	!(PIND &(1<<PD5))
#define UP 1	
#define DOWN 0
#define LEFT 1
#define RIGHT 0

ISR (TIMER1_COMPA_vect)
{
	
}

int main(void)
{
	char Balken_x;
	char Balken_y;
	char Balkenlaenge;
	char ball_y;
	char start;
	char ball_x;
	char ball_vert;
	char ball_horiz;
	
	DDRB|=(1<<PB2);
	DDRC|=(1<<PC3);
	PORTC&=~(1<<PC3);

	
	DDRD&=~((1<<PD5)|(1<<PD6)|(1<<PD2));
	PORTD|=((1<<PD5)|(1<<PD6)|(1<<PD2));
	
	//Timer 1 Configuration
	OCR1A = 0x009C;	//OCR1A = 0x3D08;==1sec
	
    TCCR1B |= (1 << WGM12);
    // Mode 4, CTC on OCR1A

    TIMSK1 |= (1 << OCIE1A);
    //Set interrupt on compare match

    TCCR1B |= (1 << CS12) | (1 << CS10);
    // set prescaler to 1024 and start the timer

	
	Balken_x=35;
	Balkenlaenge=15;
	ball_y=30;
	ball_x=50;
	Balken_y=42;
	start=0;
	ball_vert=DOWN;
	ball_horiz=RIGHT;
	
	
	sei();

	glcd_init();
	glcd_clear();
	glcd_write();
	glcd_tiny_set_font(Font5x7 ,5,7,32,127);
	glcd_clear_buffer();
	
	glcd_write();	
		
		
		while(1)
		{
			if(LINKS)
			{
				Balken_x=Balken_x--;
				if(Balken_x < 1)
				{
					Balken_x = 2;
				}
			}
			if(RECHTS)
			{
				Balken_x=Balken_x++;
				if(Balken_x > 68)
				{
					Balken_x = 67;
				}
			}
			
			if(ball_vert==DOWN)
			{
				ball_y++;
			}else ball_y--;
			
			/*if(start==0)
			{
				ball_y++;
				
				if (ball_y+2>42)
				{
					start++;
				}
			}*/
			
			
			if((ball_y+2>=Balken_y)&&(ball_x>=Balken_x)&&(ball_x<=(Balken_x+Balkenlaenge))&&(ball_vert==DOWN)) //Abprall ab Balken
			{
				ball_vert=UP;
			}else if((ball_vert==UP)&&(ball_y<=2))  //Abprall an Oberem Rand
					{
						ball_vert=DOWN;
					}
				
				
			glcd_clear();
			
			glcd_draw_rect(Balken_x,Balken_y,Balkenlaenge,3,1);
			
			glcd_fill_circle(ball_x,ball_y,2,1);	
			
			glcd_draw_rect(00,00,84,47,1);
			
			glcd_write();
			
			_delay_ms(10);
		}
				
	
}