

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
#define GETROFFEN 0
#define abgenutzt 2
#define BALLSPEED 5
#define SCHILD 3


	unsigned char ms10=0; 			
	unsigned char ms100=0;			//variabeln für T imer erstellen
	unsigned char Bewegung=1;
	volatile char ballspeed=6;
	char entprell=3;
	char timer_ballspeed=0;
	char sec=0;
	char zaehler=0;
	char look=0;
															//Verschidene Variabeln für Funktionen und Globale anwendungen
	char remaining_blocks=0;
	char level=0;
	char ball_vert;
	char ball_horiz=10;
	char ball_y;
	char ball_x;
	char blocks[4][11]={{0,0,0,0,0,1,1,2,1,1,1},{1,2,5,2,1,1,0,1,1,0,1},{5,2,3,2,5,1,1,2,2,1,1},{5,1,5,1,5,0,0,3,3,0,0}};  										//Level mit array
	char blocks_koord[11][3]={{4,2,12},{20,2,12},{36,2,12},{52,2,12},{68,2,12},{4,7,4},{12,7,12},{28,7,12},{44,7,12},{60,7,12},{76,7,4}};						//koodinaten für Blöcke     (x Block, y Block, width Block)                     
	
	
char Zeichnen (char Nummer, char look)
{
	switch(look)
	{
		case (0):		//Block wird nicht angezeigt
		break;
		
		case(1):
		
		glcd_draw_rect(blocks_koord[Nummer][0],blocks_koord[Nummer][1],blocks_koord[Nummer][2],4,1);				//Normaler Block
		
		break;
		
		
		case abgenutzt:
		
			glcd_draw_rect(blocks_koord[Nummer][0],blocks_koord[Nummer][1],blocks_koord[Nummer][2],4,1);
			glcd_draw_line(blocks_koord[Nummer][0],blocks_koord[Nummer][1],blocks_koord[Nummer][0]+blocks_koord[Nummer][2],blocks_koord[Nummer][1]+4,1);				//Blöcke mit Kreuz darin, müssen zweimal getroffen werden
			glcd_draw_line(blocks_koord[Nummer][0]+blocks_koord[Nummer][2],blocks_koord[Nummer][1]+4,blocks_koord[Nummer][0],blocks_koord[Nummer][1],1);			
			
		break;
		
		case SCHILD:
		
			glcd_fill_rect(blocks_koord[Nummer][0],blocks_koord[Nummer][1],blocks_koord[Nummer][2],4,1);		//Kann nicht verschwinden und ist Komplett schwarz
		break;
		
		case BALLSPEED:
			
			glcd_draw_rect(blocks_koord[Nummer][0],blocks_koord[Nummer][1],blocks_koord[Nummer][2],4,1);			//Normales Block Verhalten
		break;
			
	}
}


char Blockfunktion(char level)
{
	
	uint8_t anz_blocks=11;
	uint8_t anz_getroffen=0;
	uint8_t anz_schild=0;
	
	for(char x=0;x<12;x++)
	{
		if(blocks[level][x]!=GETROFFEN) //Wenn nicht 0 dann ist Block noch aktiv
		{
			if(blocks[level][x]!=SCHILD){
				anz_blocks--;
			}else anz_schild++;
			
			if((ball_y-2<=(blocks_koord[x][1]+4))&&(ball_x+2>=blocks_koord[x][0])&&(ball_x-2<=(blocks_koord[x][0]+blocks_koord[x][2])&&(blocks[level][x]!=0)&&(ball_vert==UP)))	//block getroffen
			{
				if(blocks[level][x]==BALLSPEED){
					blocks[level][x]=GETROFFEN;
					ballspeed=3;							//Wenn ballspeed blöcke getroffen werden wird geschw. erhöht
					timer_ballspeed=3;
				}
				if(blocks[level][x]==1)
				{
					blocks[level][x]--;				//Farbe/funktion wird auf 1 oder 0 gesetzt
				}
				
				if(blocks[level][x]==2)
				{	
					blocks[level][x]--;
				}
					ball_vert=DOWN;
					anz_getroffen++;
			}	
		}
	}
	
	remaining_blocks= anz_blocks-anz_getroffen-anz_schild;
	
	return  remaining_blocks;					//Rechnet aus wie viele Blöcke noch auf dem Feld sind
	
}
	

//====================================================================




ISR (TIMER1_COMPA_vect)
{
	ms10++;							//Alle 10 Milisekunden
	if(entprell!=0)entprell--;
		
		zaehler++;				//Timer, zeit bestimmen
		if(zaehler==ballspeed){
			Bewegung=1;
			zaehler=0;
		}	
	if(ms10==10) //alle 100 Milisekunden
	{
		
		ms10=0;
		ms100++;
				
	}
	
	
	if(ms100==10){		//Alle Sekunden
		sec++;
		if(timer_ballspeed!=0){
			timer_ballspeed--;
		}else ballspeed=6;
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
	
	unsigned char Status=0;
	
	char buffer[10];
	char buffer1[10];
	char Start=0;
	
	
//====================================================================Ports für Taster als Input definieren	
//Hintergrundbeleuchtung definieren
	
	DDRB |= (1<<PB2);
	PORTB |= (1<<PB2);						//Hintergrundbeleuchtung ein
	
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
	ball_y=13;
	ball_x=42;									//Balken und Ball Variabeln Deklarieren
	Balken_y=42;
	ball_vert=DOWN;							
	ball_horiz=10;
	
	
	
	glcd_fill_circle(ball_x,ball_y,2,1); 		//Startposition Ball Zeichnen
	
	sei();
	glcd_init();
	glcd_clear();								//Display einstellungen
	glcd_write();
	glcd_tiny_set_font(Font5x7 ,5,7,32,127);


//====================================================================



///////////////////////////////////////////////////////////////// START While //////////////////////////////////////////////////////////////////
	
	
		while(1)
		{
		
		
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\Start switch/case////////////////////////////////////////////////////////////////
			switch(Status)
			{
			case wait: 
		
				glcd_tiny_draw_string(7, 2, "Press yellow   to start");				//Warte Bildschrim um zu starten
				glcd_draw_rect(00,00,84,47,1);
				glcd_write();
							
							
							
							glcd_fill_circle(ball_x,ball_y,2,1);
						
																				//Startposition Ball Zeichnen

				if ((MITTE)&&(entprell==0))
						{
							entprell=RELOAD;
							
							Status=RUN;					//Wenn Startknopf gedrückt
							Start=1;
							ball_horiz=10;
						}
						break;

			
			case RUN:
			
		
			
			
			
			if(Start==1)
			{
				ball_vert=DOWN;
				Start=0;									//Ball fällt von Mitte nach Unten zum Starten
				ball_y=20;
			}
						
			
			glcd_write();
		
		//----------------------------------------------------------------	
			if((LINKS)&&(entprell==0))
			{
				entprell=RELOAD;
				Balken_x=Balken_x--;
				if(Balken_x < 1)
				{
					Balken_x = 2;
				}										//balkenbewegungen nach Links und rechts
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
				ball_y--;	
													//Ball Bewegung nach Oben
			}

		//----------------------------------------------------------------------

			if((ball_horiz==LEFT)&&(Bewegung==1)) 
			{
				ball_x--;
													//Ball Bewegung nach Links
			}

		//----------------------------------------------------------------------

			if((ball_horiz==RIGHT)&&(Bewegung==1)) 
			{
				ball_x++;
													//Ball Bewegung nach Rechts
			}

		//---------------------------------------------------------------------		

				Bewegung=0;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Ball abprallen >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



			if((ball_y+2>=Balken_y)&&(ball_x>=Balken_x+5)&&(ball_x<=(Balken_x+10))&&(ball_vert==DOWN)&&(ball_y+2<=Balken_y+1))	//Abprall nach oben
			{
				ball_vert=UP;
			}else if((ball_vert==UP)&&(ball_y<=2))  														//Abprall an Oberem Rand
					{
						ball_vert=DOWN;
					}


		//---------------------------------Richtungswechsel an Balken------------------------------------		


			if((ball_y+2>=Balken_y)&&(ball_x<=(Balken_x+5))&&(ball_x>=Balken_x)&&(ball_vert==DOWN)) //Winkel nach links
			{
				ball_horiz=LEFT;
				ball_vert=UP;
			}else if(ball_x<=2)  //Abprall an linker Wand
					{
						ball_horiz=RIGHT;
					}
					
					if((ball_x>=(Balken_x+10))&&(ball_x<=(Balken_x+15))&&(ball_y+2>=Balken_y)&&(ball_vert==DOWN)) //Winkel nach rechts
					{
						ball_horiz=RIGHT;
						ball_vert=UP;
					}else if(ball_x>=82)  //Abprall an Rechter Wand
							{
								ball_horiz=LEFT;
							}
		//***********************************ENDE Bewegungselemente***********************************
			
			
//_____________________________________________________________________________________________________________________________________
											
												/*Level aufstieg und Gamerover System*/
//_____________________________________________________________________________________________________________________________________	 
		
		
		if(Blockfunktion(level)==11){
			level++;									//wenn alle Blöcke getroffen dann --> Level aufstieg
			
			glcd_clear_buffer();
			Leben=3;
			ball_x=40;
			ball_y=24;
			ball_vert=DOWN;
			ball_horiz=2;
			Balken_x=35;
			Status=RUN;
		}											
	//----------------------------------------------------------------------------------------	
		if(ball_y+2>=48)
		{
			Leben--;
			ball_y=24;															//wenn alle Leben aufgebraucht wurden dann --> Gameover
			ball_x=40 ;
			ball_vert=DOWN;
			ball_horiz=2;
		}else if(Leben==0)
				{
					Status=GAMEOVER;
				}
	//______________________________________________________________________________________________________________________________________
															/*Zeichnen*/
	//______________________________________________________________________________________________________________________________________
		
		glcd_clear_buffer();
		
		for(char x=0;x<12;x++){
			
			Zeichnen(x,blocks[level][x]); 			//Alles wird gezeichnet
		}
		
	//-------------------------------------------------------------------------------------	
		if(Leben==3){
			glcd_set_pixel((Balken_x+17),Balken_y,1);
			glcd_set_pixel((Balken_x+19),Balken_y,1);
			glcd_set_pixel((Balken_x+21),Balken_y,1);
		}														//Lebensanzeige
		if(Leben==2){
			glcd_set_pixel((Balken_x+19),Balken_y,1);
			glcd_set_pixel((Balken_x+17),Balken_y,1);
		}
		if(Leben==1){
			glcd_set_pixel((Balken_x+17),Balken_y,1);
		}
		
	//-------------------------------------------------------------------------------------
	
			sprintf(buffer,"%d",sec);
			glcd_tiny_draw_string(Balken_x-12,5,buffer);
			
			sprintf(buffer1,"%d",Blockfunktion(level));
			glcd_tiny_draw_string(3,3,buffer1);
			
		glcd_fill_rect(Balken_x,Balken_y,Balkenlaenge,3,color_balken);
																			//Balken und Ball sowie Zeit anzeigen
		glcd_fill_circle(ball_x,ball_y,2,color_ball);					
			
		glcd_draw_rect(00,00,84,47,1);
			
		glcd_write();
	//-------------------------------------------------------------------------------------	
		
			break;
//______________________________________________________________________________________________________________________________________________
																/*Gameover*/
//______________________________________________________________________________________________________________________________________________
		case GAMEOVER:
			
			Leben=3;
						
				glcd_clear_buffer();
				glcd_tiny_draw_string(16, 1, "GAME OVER");
				glcd_tiny_draw_string(7, 3, "Press yellow  to restart");
				glcd_write();
				
				if((MITTE)&&(entprell==0))
				{
					entprell=RELOAD;
					Status=RUN;
					sec=0;
					level=0;
					
				}
		
		break;
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\END switch/case/////////////////////////////////////////////////////////////////
		
			}//End Switch, Case
	 
		
////////////////////////////////////////////////////////////////// ENDE While //////////////////////////////////////////////////////////////////
	
	
		}//End while		


//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| ENDE MAIN |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


} //End main