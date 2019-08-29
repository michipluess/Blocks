

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
#define LEFT 0
#define RIGHT 1


unsigned char ms10=0;
unsigned char ms100=0;
unsigned char Bewegung=0;


ISR (TIMER1_COMPA_vect)
{
	ms10++;
	if(ms10==2)
	{
		Bewegung++;
		ms10=0;
		ms100++;
	}
	if(ms100==3)
	{
		Bewegung=0;
		ms100=0;
	}
}

int main(void)
{
	char Balken_x;
	char Balken_y;
	char Balkenlaenge;
	char ball_y;
	char ball_x;
	char ball_vert;
	char ball_horiz=10;
	
	
	
/*	
	char buffer[10];
	char buffer1[10];
	char buffer2[10];
	*/
	
	
	
	DDRB|=(1<<PB2);
	DDRC|=(1<<PC3);
	PORTC&=~(1<<PC3);

	
	DDRD&=~((1<<PD5)|(1<<PD6)|(1<<PD2));
	PORTD|=((1<<PD5)|(1<<PD6)|(1<<PD2));
//====================================================================
	
	//Timer 1 Configuration
	OCR1A = 1270;	//OCR1A = 0x3D08;==1sec
	
    TCCR1B |= (1 << WGM12);
    // Mode 4, CTC on OCR1A

    TIMSK1 |= (1 << OCIE1A);
    //Set interrupt on compare match

    TCCR1B |= (1 << CS11)|(1 << CS10);
    // set prescaler to 64 and start the timer
//====================================================================
	
	Balken_x=35;
	Balkenlaenge=15;
	ball_y=12;
	ball_x=42;
	Balken_y=42;
	ball_vert=DOWN;
	ball_horiz=10;
	
	
	glcd_fill_circle(ball_x,ball_y,2,1);

	sei();
	glcd_init();
	glcd_clear();
	glcd_write();
	glcd_tiny_set_font(Font5x7 ,5,7,32,127);
	
	
										/*while (1){
										glcd_clear_buffer();
										sei(); //enable global interrupt
										
										sprintf(buffer2,"%d",Milisec);
										glcd_tiny_draw_string(30,1,buffer2);
										
										
										sprintf(buffer,"%d",Sekunde);
										glcd_tiny_draw_string(30,2,buffer);
										
										sprintf(buffer1,"%d",Minute);
										glcd_tiny_draw_string(30,3,buffer1);
										glcd_write();*/

											
		
		while(1)
		{
//======================================================================			
			if(LINKS)
			{
				Balken_x=Balken_x--;
				if(Balken_x < 1)
				{
					Balken_x = 2;
				}							//balkenbewegungen nach Links und rechts
			}
			if(RECHTS)
			{
				Balken_x=Balken_x++;
				if(Balken_x > 68)
				{
					Balken_x = 67;
				}
			}

//======================================================================

			if((ball_vert==DOWN)&&(Bewegung==1)) 
			{
				ball_y++;			//Ball Bewegung nach Unten
			}						

//--------------------------------------------------------------

			if((ball_vert==UP)&&(Bewegung==1)) 
			{
				ball_y--;						//Ball Bewegung nach Oben
			}

//----------------------------------------------------------------------

			if((ball_horiz==LEFT)&&(Bewegung==1)) 
			{
				ball_x--;						//Ball Bewegung nach Links
			}

//----------------------------------------------------------------------

			if((ball_horiz==RIGHT)&&(Bewegung==1)) 
			{
				ball_x++;						//Ball Bewegung nach Rechts
			}

//--------------------------------------------------------			

			if((ball_y+2>=Balken_y)&&(ball_x>=Balken_x+5)&&(ball_x<=(Balken_x+10))&&(ball_vert==DOWN)) //Abprall nach oben
			{
				ball_vert=UP;
			}else if((ball_vert==UP)&&(ball_y<=2))  //Abprall an Oberem Rand
					{
						ball_vert=DOWN;
					}

//--------------------------------------------------------			

			if((ball_y+2>=Balken_y)&&(ball_x<=(Balken_x+5))&&(ball_x>=Balken_x)&&(ball_vert==DOWN)) //Winkel nach links
			{
				ball_horiz=LEFT;
				ball_vert=UP;
			}else if(ball_x<=2)  //Abprall an linker Wand
					{
						ball_horiz=RIGHT;
					}else 
					
			if((ball_x>=(Balken_x+10))&&(ball_x<=(Balken_x+15))&&(ball_y>=Balken_y)&&(ball_vert==DOWN)) //Winkel nach rechts
			{
				ball_horiz=RIGHT;
				ball_vert=UP;
			}else if(ball_x>=82)  //Abprall an Rechter Wand
					{
						ball_horiz=LEFT;
					}
//---------------------------------------------------------
			
				
				
			glcd_clear_buffer();											//alles wird gezeichned
			
			glcd_draw_rect(Balken_x,Balken_y,Balkenlaenge,3,1);
			
			glcd_fill_circle(ball_x,ball_y,2,1);	
			
			glcd_draw_rect(00,00,84,47,1);
			
			glcd_write();
			
			
		}
				
		
	
}