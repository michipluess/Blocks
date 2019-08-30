

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
#define MITTE	!(PIND &(1<<PD2))
#define UP 1	
#define DOWN 0
#define LEFT 0
#define RIGHT 1
#define wait 0
#define RUN 1
#define GAMEOVER 2
#define RELOAD 2
#define getroffen 0
#define abgenutzt 2
#define ballspeed 5
#define schild 3
#define breiter_balken 4
	char ball_vert;
	char ball_horiz=10;
	char ball_y;
	char ball_x;

/*char Blockfunktion(char level, char blocks)
{

	switch(type)
	{
	
			
		
	case abgenutzt:
		if((ball_y+2<=24)&&(ball_x+2>=2)&&(ball_x+2<=32))
		{
			glcd_draw_rect(20,2,4,12,1);
			if(ball_horiz==UP)
			{
				ball_horiz=DOWN;
			}
			if(ball_horiz==DOWN)
			{
				ball_horiz=UP;
			}
		}
		if((ball_y+2<=52)&&(ball_x+2>=2)&&(ball_x+2<=64))
		{
			glcd_draw_rect(52,2,4,12,1);
			if(ball_horiz==UP)
			{
				ball_horiz=DOWN;
			}
			if(ball_horiz==DOWN)
			{
				ball_horiz=UP;
			}
		}
		glcd_clear_buffer();
		
		glcd_write();
		
	case ballspeed:
		if((ball_y+2<=(yBOX1+4))&&(ball_x+2>=xBOX1)&&(ball_x+2<=(xBOX1+12)))
		{
		
			if(ball_horiz==UP)
			{
				ball_y-2;
			}
			if(ball_horiz==DOWN)
			{
				ball_y+2;
			}
		}
	case schild:
	case breiter_balken:
	break;
	}
	
}
*/

//====================================================================


unsigned char ms10=0; 			
unsigned char ms100=0;			//variabeln f�r Timer erstellen
unsigned char Bewegung=0;
char entprell=3;

ISR (TIMER1_COMPA_vect)
{
	ms10++;
	if(entprell!=0)entprell--;
		
		
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
	
	unsigned char color_ball=1;
	unsigned char color_balken=1;
	
	unsigned char Leben=3;
	
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
	
	unsigned char xBOX6=4;
	unsigned char xBOX7=12;
	unsigned char xBOX8=28;
	unsigned char xBOX9=44;
	unsigned char xBOX10=60;
	unsigned char xBOX11=76;
	
	unsigned char yBOX6=7;
	unsigned char yBOX7=7;
	unsigned char yBOX8=7;
	unsigned char yBOX9=7;
	unsigned char yBOX10=7;
	unsigned char yBOX11=7;
	
	unsigned char Status=0;
	char level=0;
	
	uint8_t blocks[4][11]={{1,1,1,1,1,1,1,1,1,1,1},{1,2,5,2,1,1,0,3,3,0,1},{5,2,4,2,5,1,1,4,4,1,1},{5,1,5,1,5,0,0,3,3,0,0}};   //Level mit array

	
//====================================================================Ports f�r Taster als Input definieren	

	
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
			switch(Status)
			{
			case wait: 
		
				glcd_tiny_draw_string(7, 2, "Press yellow   to start");				//Warte Bildschrim um zu starten
				glcd_draw_rect(00,00,84,47,1);
				glcd_write();
							
							
							
							glcd_fill_circle(ball_x,ball_y,2,1);		 //Startposition Ball Zeichnen

				if ((MITTE)&&(entprell==0))
						{
							entprell=RELOAD;
							
							Status=RUN;
						}
						break;

			
			case RUN:
			
			
			glcd_write();
		
		//----------------------------------------------------------------	
			if((LINKS)&&(entprell==0))
			{
				entprell=RELOAD;
				Balken_x=Balken_x--;
				if(Balken_x < 1)
				{
					Balken_x = 2;
				}						//balkenbewegungen nach Links und rechts
			}
			if((RECHTS)&&(entprell==0))
			{
				entprell=RELOAD;
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
			
			
//_____________________________________________________________________________________________________________________________________
											
											/*Bl�cke generieren und dessen Verhalten*/
//_____________________________________________________________________________________________________________________________________	 
		
		
		//---------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX1+4)&&(ball_x+2>=xBOX1)&&(ball_x+2<=(xBOX1+12))&&(blocks[level][0]==1))) 
			{
				blocks[level][0]=0;																	//Verschwinden von box1
				ball_vert=DOWN;
			}
		//---------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX2+4)&&(ball_x+2>=xBOX2)&&(ball_x+2<=(xBOX2+12))&&(blocks[level][1]==1))) 
			{
				blocks[level][1]=0;																	//Verschwinden von box2
				ball_vert=DOWN;
			}
		//---------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX3+4)&&(ball_x+2>=xBOX3)&&(ball_x+2<=(xBOX3+12))&&(blocks[level][2]==1))) 
			{
				blocks[level][2]=0;																	//Verschwinden von box3
				ball_vert=DOWN;
			}
	 	//---------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX4+4)&&(ball_x+2>=xBOX4)&&(ball_x+2<=(xBOX4+12))&&(blocks[level][3]==1))) 
			{
				blocks[level][3]=0;																	//Verschwinden von box4
				ball_vert=DOWN;
			}
		//---------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX5+4)&&(ball_x+2>=xBOX5)&&(ball_x+2<=(xBOX5+12))&&(blocks[level][4]==1))) 
			{
				blocks[level][4]=0;																	//Verschwinden von box5
				ball_vert=DOWN;
			}
		//---------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX6+4)&&(ball_x+2>=xBOX6)&&(ball_x+2<=(xBOX6+4))&&(blocks[level][5]==1))) 
			{
				blocks[level][5]=0;																	//Verschwinden von box6
				ball_vert=DOWN;
			}
		//---------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX7+4)&&(ball_x+2>=xBOX7)&&(ball_x+2<=(xBOX7+12))&&(blocks[level][6]==1))) 
			{
				blocks[level][6]=0;																	//Verschwinden von box7
				ball_vert=DOWN;
			}
		//---------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX8+4)&&(ball_x+2>=xBOX8)&&(ball_x+2<=(xBOX8+12))&&(blocks[level][7]==1))) 
			{
				blocks[level][7]=0;																	//Verschwinden von box8
				ball_vert=DOWN;
			}
		//---------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX9+4)&&(ball_x>=xBOX9)&&(ball_x<=(xBOX9+12))&&(blocks[level][8]==1))) 
			{
				blocks[level][8]=0;																	//Verschwinden von box9
				ball_vert=DOWN;
			}
		//---------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX10+4)&&(ball_x>=xBOX10)&&(ball_x<=(xBOX10+12))&&(blocks[level][9]==1))) 
			{
				blocks[level][9]=0;																	//Verschwinden von box10
				ball_vert=DOWN;
			}
		//---------------------------------------------------------------------------------------
			if((ball_y+2<=(yBOX11+4)&&(ball_x>=xBOX11)&&(ball_x<=(xBOX11+4))&&(blocks[level][10]==1))) 
			{
				blocks[level][10]=0;																	//Verschwinden von box11
				ball_vert=DOWN;
			}
		//---------------------------------------------------------------------------------------
	
	
	//_____________________________________________________________________________________________________________________________________
														
														/*Game Over and winning system*/
	//_____________________________________________________________________________________________________________________________________												
			
			if((blocks[level][0]==0)&&(blocks[level][1]==0)&&(blocks[level][2]==0)&&(blocks[level][3]==0)&&(blocks[level][4]==0)&&(blocks[level][5]==0)&&(blocks[level][6]==0)&&(blocks[level][7]==0)&&(blocks[level][8]==0)&&(blocks[level][9]==0)&&(blocks[level][10]==0))
			{
				level=1;
				Status=RUN;
				
			
			
			}
			if(ball_y+2>=48)
			{
				Leben--;
				ball_y=24;															//wenn alle Bl�cke getroffen sind oder wenn alle Leben aufgebraucht wurden dann --> Raise Gameover
				ball_x=40 ;
				ball_vert=DOWN;
				ball_horiz=2;
			}else if(Leben==0)
					{
						Status=GAMEOVER;
					}
	
	//-----------------------------------------------------------		
		glcd_clear_buffer();
		
		glcd_fill_rect(xBOX1,yBOX1,12,4,blocks[level][0]);	
		glcd_fill_rect(xBOX2,yBOX2,12,4,blocks[level][1]);	
		glcd_fill_rect(xBOX3,yBOX3,12,4,blocks[level][2]);	
		glcd_fill_rect(xBOX4,yBOX4,12,4,blocks[level][3]);	
		glcd_fill_rect(xBOX5,yBOX5,12,4,blocks[level][4]);

		glcd_fill_rect(xBOX6,yBOX6,4,4,blocks[level][5]);
		glcd_fill_rect(xBOX7,yBOX7,12,4,blocks[level][6]);
		glcd_fill_rect(xBOX8,yBOX8,12,4,blocks[level][7]);
		glcd_fill_rect(xBOX9,yBOX9,12,4,blocks[level][8]);							//alles wird gezeichned
		glcd_fill_rect(xBOX10,yBOX10,12,4,blocks[level][9]);
		glcd_fill_rect(xBOX11,yBOX11,4,4,blocks[level][10]);
		
		
		
			
		glcd_fill_rect(Balken_x,Balken_y,Balkenlaenge,3,color_balken);
			
		glcd_fill_circle(ball_x,ball_y,2,color_ball);					
			
		glcd_draw_rect(00,00,84,47,1);
			
		glcd_write();
	//-----------------------------------------------------------		
		
			break;
			
		case GAMEOVER:
			
			Leben=3;
			glcd_clear_buffer();			
			
				glcd_tiny_draw_string(16, 1, "GAME OVER");
				glcd_tiny_draw_string(7, 3, "Press yellow  to restart");
				glcd_write();
				
				if((MITTE)&&(entprell==0))
				{
					entprell=RELOAD;
					Status=wait;
				}
		
		break;
			}
	 
	 
////////////////////////////////////////////////////////////////// ENDE While //////////////////////////////////////////////////////////////////
	
	
		}//End while		
		
		
//=============================================================== RAISE GAMEOVER ===============================================================
		
		
		
//==============================================================================================================================================


//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| ENDE MAIN |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

} //End main