#include <avr/io.h>
#include "glcd/glcd.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "glcd/fonts/Liberation_Sans15x21_Numbers.h"
#include "glcd/fonts/font5x7.h"
#include <avr/pgmspace.h>

	
int main(void)
{	
	
	/* Backlight pin PL3, set as output, set high for 100% output */
	DDRB |= (1<<PB2);
	//PORTB |= (1<<PB2);

    sei();
	
	glcd_init();

	glcd_clear();
	glcd_write();
	
	// Display
	glcd_tiny_set_font(Font5x7,5,7,32,127);
	glcd_clear_buffer();
	glcd_tiny_draw_string(3, 2, "Ich Bin Lucas");
	glcd_write();
	
	DDRD &= ~(1<<PD5);
	PORTD |= (1<<PD5);
	
	DDRD &= ~(1<<PD2);
	PORTD |= (1<<PD2);
	
	DDRD &= ~(1<<PD6);
	PORTD |= (1<<PD6);
	
	DDRC |= (1<<PC3);
	PORTC &= ~(1<<PC3);
	
	DDRB |= (1<<PB2);
	PORTB &= ~(1<<PB2);

	while(1)
	{ 
		
		if(!(PIND &(1<<PD5)))
			{
				PORTB |= (1<<PB2);	
			}
		if(!(PIND &(1<<PD6)))
			{
				PORTB &= ~(1<<PB2);
			}
			
		if(PIND &(1<<PD2))
			{
				PORTC |= (1<<PC3);	
			}else
			{
				PORTC &= ~(1<<PC3);
			}
		
	} //end while

	return 0;
}//end of main
