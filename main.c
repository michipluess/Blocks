

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
#define delete goto


//====================================================================


unsigned char ms10=0; 			
unsigned char ms100=0;			//variabeln für Timer erstellen
unsigned char Bewegung=0;

ISR (TIMER1_COMPA_vect)
{
	ms10++;
	if(ms10==2)
	{
		Bewegung++;
		ms10=0;
		ms100++;				//Timer, zeit bestimmen
	}
	if(ms100==3)
	{
		Bewegung=0;
		ms100=0;
	}
}


//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| START MAIN ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


int main(void)
{



//====================================================================Variabeln erstellen


	char Balken_x;
	char Balken_y;
	char Balkenlaenge;
	char ball_y;
	char ball_x;
	char ball_vert;
	char ball_horiz=10;
	
	unsigned char color1=1;
	unsigned char color2=1;
	unsigned char color3=1;
	unsigned char color4=1;
	unsigned char color5=1;
	
	unsigned char xBOX1=4;
	unsigned char xBOX2=20;
	unsigned char xBOX3=36;
	unsigned char xBOX4=52;
	unsigned char xBOX5=68;
	
	unsigned char yBOX1=2;
	unsigned char yBOX2=2;
	unsigned char yBOX3=2;
	unsigned char yBOX4=2;
	unsigned char yBOX5=2;



//====================================================================Ports für Taster als Input definieren	

	
	DDRD|=(1<<PB2);						//Hintergrundbeleuchtung definieren
	PORTB|=(1<<PB2);						//Hintergrundbeleuchtung ein
	
	DDRD&=~((1<<PD5)|(1<<PD6)|(1<<PD2));	//Taster als Output definieren
	PORTD|=((1<<PD5)|(1<<PD6)|(1<<PD2));	//Pullup aktivieren

	
//====================================================================Timer einstellungen

	
	//Timer 1 Configuration
	OCR1A = 1270;	//OCR1A = 0x3D08;==1sec
	
    TCCR1B |= (1 << WGM12);
    // Mode 4, CTC on OCR1A

    TIMSK1 |= (1 << OCIE1A);
    //Set interrupt on compare match

    TCCR1B |= (1 << CS11)|(1 << CS10);
    // set prescaler to 64 and start the timer


//====================================================================Variabeln Deklarieren

	
	Balken_x=35;
	Balkenlaenge=15;
	ball_y=12;
	ball_x=42;					//Balken und Ball Variabeln Deklarieren
	Balken_y=42;
	ball_vert=DOWN;							
	ball_horiz=10;
	
	
	
	glcd_fill_circle(ball_x,ball_y,2,1); //Startposition Ball Zeichnen

	sei();
	glcd_init();
	glcd_clear();						//Display einstellungen
	glcd_write();
	glcd_tiny_set_font(Font5x7 ,5,7,32,127);


//====================================================================


///////////////////////////////////////////////////////////////// START While //////////////////////////////////////////////////////////////////
	
	
		while(1)
		{
			
		
	
		//----------------------------------------------------------------	
			if(LINKS)
			{
				Balken_x=Balken_x--;
				if(Balken_x < 1)
				{
					Balken_x = 2;
				}						//balkenbewegungen nach Links und rechts
			}
			if(RECHTS)
			{
				Balken_x=Balken_x++;
				if(Balken_x > 68)
				{
					Balken_x = 67;
				}
			}
		//----------------------------------------------------------------------

		

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Ball Bewegungen >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


			if((ball_vert==DOWN)&&(Bewegung==1)) 
			{
				ball_y++;							//Ball Bewegung nach Unten
			}						

		//----------------------------------------------------------------------

			if((ball_vert==UP)&&(Bewegung==1)) 
			{
				ball_y--;							//Ball Bewegung nach Oben
			}

		//----------------------------------------------------------------------

			if((ball_horiz==LEFT)&&(Bewegung==1)) 
			{
				ball_x--;							//Ball Bewegung nach Links
			}

		//----------------------------------------------------------------------

			if((ball_horiz==RIGHT)&&(Bewegung==1)) 
			{
				ball_x++;							//Ball Bewegung nach Rechts
			}

		//---------------------------------------------------------------------		



//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Ball abprallen >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



			if((ball_y+2>=Balken_y)&&(ball_x>=Balken_x+5)&&(ball_x<=(Balken_x+10))&&(ball_vert==DOWN)&&(ball_y+2<=Balken_y+1))	//Abprall nach oben
			{
				ball_vert=UP;
			}else if((ball_vert==UP)&&(ball_y<=2))  														//Abprall an Oberem Rand
					{
						ball_vert=DOWN;
					}


		//---------------------------------Richtungswechsel an Balken------------------------------------		


			if((ball_y+2>=Balken_y)&&(ball_x<=(Balken_x+5))&&(ball_x>=Balken_x)&&(ball_vert==DOWN)&&(ball_y+2<=Balken_y+1)) //Winkel nach links
			{
				ball_horiz=LEFT;
				ball_vert=UP;
			}else if(ball_x<=2)  //Abprall an linker Wand
					{
						ball_horiz=RIGHT;
					}else 
					
			if((ball_x>=(Balken_x+10))&&(ball_x<=(Balken_x+15))&&(ball_y+2>=Balken_y)&&(ball_vert==DOWN)&&(ball_y+2<=Balken_y+1)) //Winkel nach rechts
			{
				ball_horiz=RIGHT;
				ball_vert=UP;
			}else if(ball_x>=82)  //Abprall an Rechter Wand
					{
						ball_horiz=LEFT;
					}
		//***********************************ENDE Bewegungselemente***********************************
			
//__________________________________________________________________________________________________________
											/*Blöcke generieren und dessen Verhalten*/
			 
		//------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX1+4)&&(ball_x>=xBOX1)&&(ball_x<=(xBOX1+12))&&(color1==1))) 
			{
				color1=0;																	//Verschwinden von box1
				ball_vert=DOWN;
			}
		//------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX2+4)&&(ball_x>=xBOX2)&&(ball_x<=(xBOX2+12))&&(color1==1))) 
			{
				color2=0;																	//Verschwinden von box2
				ball_vert=DOWN;
			}
		//------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX3+4)&&(ball_x>=xBOX3)&&(ball_x<=(xBOX3+12))&&(color3==1))) 
			{
				color3=0;																	//Verschwinden von box3
				ball_vert=DOWN;
			}
	 				
	//-----------------------------------------------------------		
		glcd_clear_buffer();
		
		glcd_fill_rect(xBOX1,yBOX1,12,4,color1);	
		glcd_fill_rect(xBOX2,yBOX2,12,4,color2);	
		glcd_fill_rect(xBOX3,yBOX3,12,4,color3);	
		glcd_fill_rect(xBOX4,yBOX4,12,4,color4);	
		glcd_fill_rect(xBOX5,yBOX5,12,4,color5);	
		
		
			
		glcd_draw_rect(Balken_x,Balken_y,Balkenlaenge,3,1);
			
		glcd_fill_circle(ball_x,ball_y,2,1);					//alles wird gezeichned
			
		glcd_draw_rect(00,00,84,47,1);
			
		glcd_write();
	//-----------------------------------------------------------		
	

	
////////////////////////////////////////////////////////////////// ENDE While //////////////////////////////////////////////////////////////////
		
		}//End while		
		
		
		
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| START MAIN ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
} //End main