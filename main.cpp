#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <util/delay.h>
#include "LCD_HD44780.h"

#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))


#define ILE_OPCJI 4

#ifndef cbi
#define cbi(reg, bit) reg &= ~(_BV(bit))
#endif

#ifndef sbi
#define sbi(reg, bit) reg |= (_BV(bit))
#endif



void getKey(int *wybor)
{
    for (int i = 0; i < 4; i++)
    {
        sbi(PORTC, i);
        if (bit_is_set(PINC, 4))
        {
            cbi(PORTC, i); // nr 7 na klawiaturze
            if (i == 0)
            {
                LCD_HD44780::showNumber(7);
            }
            if (i == 1) // nr 8 na klawiaturze
            {
                //LCD_HD44780::showNumber(8);
            	if(--(*wybor)<0)
            		(*wybor) = ILE_OPCJI-1;
            }
            if (i == 2) // nr 9 na klawiaturze
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
            if (i == 0) // nr 4 na klawiaturze
            {
                LCD_HD44780::showNumber(4);
            }
            if (i == 1) // nr 5 na klawiaturze
            {
                LCD_HD44780::showNumber(5);
            }
            if (i == 2) // nr 6 na klawiaturze
            {
                LCD_HD44780::showNumber(6);
            }
            if (i == 3)  // * na klawiaturze
            {
                LCD_HD44780::writeText("*");
            }
        }
        else if (bit_is_set(PINC, 6))
        {
            cbi(PORTC, i);
            if (i == 0) // nr 1 na klawiaturze
            {
                LCD_HD44780::showNumber(1);
            }
            if (i == 1) // nr 2 na klawiaturze
            {
                //LCD_HD44780::showNumber(2);
            	if(++(*wybor)>(ILE_OPCJI-1))
            		(*wybor) = 0;


            }
            if (i == 2) // nr 3 na klawiaturze
            {
                LCD_HD44780::showNumber(3);
            }
            if (i == 3) // nr 4 na klawiaturze
            {
                LCD_HD44780::writeText("-");
            }
        }
        else if (bit_is_set(PINC, 7))
        {
            if (i == 0) // nr 0 na klawiaturze
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

void Menu(int *choice, char *opcje[]){

	LCD_HD44780::clear();
	getKey(choice); //wybieranie opcji(3 mozliwosci: pokaz nastepna opcje, pokaz poprzednia opcje, wybierz opcje)
	LCD_HD44780::writeText("MENU");
	//LCD_HD44780::showNumber(*choice); // do debuggowania w razie problemow
	LCD_HD44780::goTo(0,1);
	LCD_HD44780::writeText(opcje[*choice]); //tu wypisywana jest aktualnie przegl¹dana opcja

}


int main()
{
	int wybor=0;
	char *opcje[]= {"Opcja 0","Opcja 1","Opcja 2","Opcja 3"}; //tablica opcji
    DDRC = 0b00001111;
    PORTC = 0xff;
    LCD_HD44780::init();
    while (1){

    	Menu(&wybor,opcje);
    	_delay_ms(200);
    }
}
