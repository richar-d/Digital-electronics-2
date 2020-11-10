/***********************************************************************
 * 
 * Stopwatch with LCD display output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>			// C library. Needed for conversion function


/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Update stopwatch
 * value on LCD display when 8-bit Timer/Counter2 overflows.
 */

/* Variables ---------------------------------------------------------*/
// Custom character definition using https://omerk.github.io/lcdchargen/
uint8_t customChar[] = 
{
	0b00000, // _
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	
	0b10000, // |
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	
	0b11000, // ||
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	
	0b11100, // |||
	0b11100,
	0b11100,
	0b11100,
	0b11100,
	0b11100,
	0b11100,
	0b11100,
	
	0b11110, // ||||
	0b11110,
	0b11110,
	0b11110,
	0b11110,
	0b11110,
	0b11110,
	0b11110,
	
	0b11111, // |||||
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};

int main(void)
{
	// Initialize LCD display
	lcd_init(LCD_DISP_ON);
	
	// Put string(s) at LCD display
	lcd_gotoxy(1,0);
	lcd_puts("00:00.0");
	lcd_gotoxy(11,2);
	lcd_putc('D');
		
	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);
	
	// Set pointer to beginning of CGRAM memory
	lcd_command(1 << LCD_CGRAM);
	
	for (uint8_t i = 0; i < 48; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(customChar[i]);
	}
	


    // Configure 8-bit Timer/Counter2 for Stopwatch
    // Set prescaler and enable overflow interrupt every 16 ms
	TIM2_overflow_16ms()
	TIM2_overflow_interrupt_enable()
	
	// Configure 8-bit Timer/Counter0 for Stopwatch
	// Set prescaler and enable overflow interrupt every 16 ms
	TIM0_overflow_16ms()
	TIM0_overflow_interrupt_enable()

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter2 overflows. Update the stopwatch on
 * LCD display every sixth overflow, ie approximately every 100 ms
 * (6 x 16 ms = 100 ms).
 */
 
ISR(TIMER2_OVF_vect)
{
	static uint8_t number_of_overflows = 0;
	static uint8_t tenths = 0;		    // Tenths of a second
	static uint8_t secs = 0;		    // Seconds
	static uint8_t minutes = 0;			// Minutes
	static uint16_t secsSquared = 0;	// Seconds squared
	char lcd_string[2] = "  ";				    // String for converting numbers by itoa()
	char lcd_secsSquared[4] = "    ";		    
	number_of_overflows++;
	if (number_of_overflows >= 6) 		//  Update the stopwatch every 6th overflow / 100ms
	{
		number_of_overflows = 0;	
		tenths++;
		if (tenths > 9)
		{
			tenths = 0;
			secs++;
			if (secs > 59)
			{
				secs = 0;
				minutes++;
				if (minutes > 59)
				{
					minutes = 0;
				}
			}
		}
		//conversion to strings
		itoa(tenths, lcd_string, 10); 
		lcd_gotoxy(7,0);
		lcd_puts(lcd_string);
		
		
		itoa(secs, lcd_string, 10); //1 digit or 2 digits
		if(secs < 10)					
		{
			lcd_gotoxy(4,0); //if 1 digit
			lcd_puts("0");   //reset 1st digit of seconds
			lcd_gotoxy(5,0);  //go to 2nd  digit of seconds
	
		}
		else
		{
			lcd_gotoxy(4,0); //if 2 digits
		}
		lcd_puts(lcd_string);
		
		
		itoa(minutes, lcd_string, 10); //1 digit or 2 digits
		if(minutes < 10)
		{
			lcd_gotoxy(1,0); //if 1 digit
			lcd_puts("0");	 //reset 1st digit of mins
			lcd_gotoxy(2,0); // go to 2nd digit of mins
		}
		else
		{
			lcd_gotoxy(1,0); //if 2 digits
		}
		lcd_puts(lcd_string);
		
		// seconds squared:
		secsSquared = secs * secs; 
		if (secs == 0)				//go to pos 11;0, set 1st digit as 0 and reset 2-4th digit 
		{
			lcd_gotoxy(11,0);
			lcd_puts("0   ");
		}
		else
		{
			itoa(secsSquared, lcd_secsSquared, 10);
			lcd_gotoxy(11, 0);
			lcd_puts(lcd_secsSquared);
		}
	}
}

//progress bar
ISR(TIMER0_OVF_vect)
{
	static uint8_t symbol = 0;
	static uint8_t position = 0;
	
	
	lcd_gotoxy(1 + position, 1);
	lcd_putc(symbol);

	symbol++;
	if(symbol > 5)
	{
		symbol = 0;
		position++;
		
		if (position > 9) //reset to empty bar
		{
			lcd_gotoxy(1, 1);
			lcd_puts("          ");		
			position = 0;
		}
	}
	
}