#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include<avr/interrupt.h>

#include "LCD_HD44780.h"

#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))

#ifndef cbi
#define cbi(reg, bit) reg &= ~(_BV(bit))
#endif

#ifndef sbi
#define sbi(reg, bit) reg |= (_BV(bit))
#endif


void getKey()
{
    for (int i = 0; i < 4; i++)
    {
        sbi(PORTC, i);
        if (bit_is_set(PINC, 4))
        {
            cbi(PORTC, i);
            if (i == 0)
            {
                LCD_HD44780::showNumber(7);
            }
            if (i == 1)
            {
                LCD_HD44780::showNumber(8);
            }
            if (i == 2)
            {
                {
                    LCD_HD44780::showNumber(9);
                }
            }
            if (i == 3)
            {
                {
                    LCD_HD44780::writeText("/");
                }
            }
        }
        else if (bit_is_set(PINC, 5))
        {
            cbi(PORTC, i);
            if (i == 0)
            {
                LCD_HD44780::showNumber(4);
            }
            if (i == 1)
            {
                LCD_HD44780::showNumber(5);
            }
            if (i == 2)
            {
                LCD_HD44780::showNumber(6);
            }
            if (i == 3)
            {
                LCD_HD44780::writeText("*");
            }
        }
        else if (bit_is_set(PINC, 6))
        {
            cbi(PORTC, i);
            if (i == 0)
            {
                LCD_HD44780::showNumber(1);
            }
            if (i == 1)
            {
                LCD_HD44780::showNumber(2);
            }
            if (i == 2)
            {
                LCD_HD44780::showNumber(3);
            }
            if (i == 3)
            {
                LCD_HD44780::writeText("-");
            }
        }
        else if (bit_is_set(PINC, 7))
        {
            if (i == 0)
            {
                LCD_HD44780::showNumber(0);
            }
            if (i == 1)
            {
                LCD_HD44780::clear();
            }
            if (i == 2)
            {

            }

            if (i == 3)
            {
                LCD_HD44780::writeText("+");
            }
        }
        cbi(PORTC, i);
    }
}

void Menu(){
	LCD_HD44780::clear();
	LCD_HD44780::writeText("MENU");
	LCD_HD44780::goTo(0,1);
	getKey(); //wybieranie opcji(3 mozliwosci: pokaz nastepna opcje, pokaz poprzednia opcje, wybierz opcje)
	//LCD_HD44780::writeText();
}

ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	TCNT1 = 64755;   // for 1 sec at 8 MHz
	Menu();
}


int main()
{


    DDRC = 0b00001111;
    PORTC = 0xff;
    LCD_HD44780::init();
    char str[] = "";
	TCCR1A = 0x00;
	TCCR1B = (1<<CS10) | (1<<CS12);;  // Timer mode with 1024 prescler
	TIMSK = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG
    while (1)
    {}
}
