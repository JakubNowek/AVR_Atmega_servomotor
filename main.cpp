/*
v

bool do_Menu = true;
bool angleOk = false;



void getKey(int *choice)
{
    for (int i = 0; i < 4; i++)
    {
        sbi(PORTC, i);
        if (bit_is_set(PINC, 4))
        {
            cbi(PORTC, i); // nr 7 na klawiaturze
            if (i == 0)
            {
                //LCD_HD44780::showNumber(7);
            	*choice = 7;
            }
            if (i == 1) // nr 8 na klawiaturze
            {
                //LCD_HD44780::showNumber(8);
            	*choice = 8;

            }
            if (i == 2) // nr 9 na klawiaturze
            {
                {
                    //LCD_HD44780::showNumber(9);
                	*choice = 9;
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
                //LCD_HD44780::showNumber(4);
            	*choice = 4;
            }
            if (i == 1) // nr 5 na klawiaturze
            {
                //LCD_HD44780::showNumber(5);
            	//LCD_HD44780::clear();
            	*choice = 5;

            }
            if (i == 2) // nr 6 na klawiaturze
            {
                //LCD_HD44780::showNumber(6);
            	*choice = 6;
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
                //LCD_HD44780::showNumber(1);
            	*choice = 1;
            }
            if (i == 1) // nr 2 na klawiaturze
            {
                //LCD_HD44780::showNumber(2);
            	*choice = 2;

            }
            if (i == 2) // nr 3 na klawiaturze
            {
                //LCD_HD44780::showNumber(3);
            	*choice = 3;
            }
            if (i == 3) // nr 4 na klawiaturze
            {
                //LCD_HD44780::writeText("-");
            }
        }
        else if (bit_is_set(PINC, 7))
        {
            if (i == 0) // nr 0 na klawiaturze
            {
                //LCD_HD44780::showNumber(0);

            	*choice = 0;
            }
            if (i == 1)
            {
                //LCD_HD44780::clear();
            }
            if (i == 2)
            {

            	*choice = 11;
            }

            if (i == 3)
            {
                LCD_HD44780::writeText("+");
            }
        }
        cbi(PORTC, i);
    }
}

void init_PWM()
{
	TCCR1A |= (1 << WGM10) | (1 << WGM11) | (1 << COM1A1); // TOP - 1023
	TCCR1B |= (1 << WGM12) | (1 << CS12);				   // prescaler 256 bits
}
int low = 1023 * 0.061, high = 1023 * 0.125, middle = 65; // low -51 high -123

void Bounce(){ //  "Odbijanie" serwa
	LCD_HD44780::clear();
	LCD_HD44780::writeText("Bounce, bounce");
	LCD_HD44780::goTo(0,1);
	//LCD_HD44780::clear();
}

void Rotate(int *choice, char *kat){ // Obrót serwa o dowolny k¹t
	char c;
	int angle = -1;
	int pom = angle;
	int rot_angle;
	LCD_HD44780::clear();

	getKey(choice);
	if((*choice)!=11 && angleOk == false)
	{
		LCD_HD44780::writeText("What's the angle: ");
		LCD_HD44780::goTo(0,1);
		angle=(*choice);
		if(angle!=pom){
			c = angle+'0';
			strncat(kat, &c, 1);
		}
		LCD_HD44780::showNumber(atoi(kat));
	}
	else{
		angleOk = true;
		rot_angle = atoi(kat);
	}

	if(angleOk == true) //wartoœc choice==11 oznacza, ¿e akceptujemy wybrany kat
	{
		LCD_HD44780::writeText("Angle is: ");
		LCD_HD44780::showNumber(rot_angle);
		LCD_HD44780::goTo(0,1);
		LCD_HD44780::writeText("Rotating...");
		servo

	}
	_delay_ms(200);
}

chyba wywalic
void Spin(){ // Sta³e obracanie serwa
	LCD_HD44780::clear();
	LCD_HD44780::writeText("You spin me");
	LCD_HD44780::goTo(0,1);
	//LCD_HD44780::clear();
	LCD_HD44780::writeText("Right round");
}

void Exit(){ // Wy³¹czenie serwa i ekranu
	LCD_HD44780::writeCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF); // wy³¹czenie ekranu
}

void Menu(int *choice,int *page ,char *opcje1[]){

	LCD_HD44780::clear();

	getKey(choice); //wybieranie opcji(3 mozliwosci: pokaz nastepna opcje, pokaz poprzednia opcje, wybierz opcje)
	//LCD_HD44780::showNumber(*choice); // do debuggowania w razie problemow

	if(*choice == 8){
		if(--(*page)<0)
			(*page) = ILE_OPCJI-1;
	}
	if(*choice == 2){
		if(++(*page)>(ILE_OPCJI-1))
			(*page) = 0;
	}

	LCD_HD44780::writeText("MENU");
	//LCD_HD44780::showNumber(*choice); // do debuggowania w razie problemow
	LCD_HD44780::goTo(0,1);
	LCD_HD44780::writeText(opcje1[*page]); //tu wypisywana jest aktualnie przegl¹dana opcja

	if(*choice == 5){
		//do_Func = true;
		do_Menu = false;
	}

	_delay_ms(200);
}


int main()
{
	int wybor=0, strona = 0; char kat[]="";
	char *opcje1[]= {"0 - Motor Bounce ",
					 "1 - Rotate ",
					 "2 - Spin ",
					 "3 - Exit "
	}; //tablica opcji

    DDRC = 0b00001111;
    PORTC = 0xff;
    LCD_HD44780::init();
    while (1){

    	if(do_Menu == true)
    	Menu(&wybor,&strona,opcje1);
    	else{
    		switch (strona)
    		{
    		    case 0: Bounce();
    		        break;
    		    case 1: Rotate(&wybor,kat);
    		    	break;
    		    case 2: Spin();
    		        break;
    		    case 3: Exit();
    		    return 0;
    		        break;
    		    default:
    		    	break;
    		}
    	}
    	//_delay_ms(200);
    	//wybor = 0;
    	//kat = 0;
    	wybor = -1;

    }
}



*/



#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <util/delay.h>
#include "LCD_HD44780.h"
#include <math.h>

#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))


#define ILE_OPCJI 4

#ifndef cbi
#define cbi(reg, bit) reg &= ~(_BV(bit))
#endif

#ifndef sbi
#define sbi(reg, bit) reg |= (_BV(bit))
#endif


void init_PWM() // zadanie 1
{
	TCCR1A |= (1 << WGM10) | (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << CS12); //ustawienie prescalera na 256 bitow
}



/* void init_PWM() // zadanie 1
{
	TCCR1A |= (1 << WGM10) | (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << CS11) ; //ustawienie prescalera na 8 bitow
}*/

/* void init_PWM() // zadanie 1
{
	TCCR1A |= (1 << WGM10) | (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10); //ustawienie prescalera na 64 bitow
}*/
/*
void init_PWM(){
	TCCR1A |= (1 << WGM10) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << CS11); //ustawienie prescalera na 256 bitow
}
*/

int main()
{
int min = 31;
   init_PWM();
   DDRD|=(1<<PD4)|(1<<PD5);   //PWM Pins as Output
   //int i = 31 // wartosc minimalna;
   //int i = 63;// wartosc max;
   	  for(int i=min;i<64;i++)
   	   {
		 OCR1A=i;
		  _delay_ms(200);
		  if (i == 63){
			  i=min;
			  OCR1A=i;
			  _delay_ms(1000);
		  }
   	   }
}



