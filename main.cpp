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


#define ILE_OPCJI 3 //ilosc opcji w menu

#ifndef cbi
#define cbi(reg, bit) reg &= ~(_BV(bit))
#endif

#ifndef sbi
#define sbi(reg, bit) reg |= (_BV(bit))
#endif


#define SERV_MIN 31
#define SERV_MAX 63

bool do_Menu = true;
bool angleOk = false;
bool rotation = false;

int tikRemain=0; //ile krokow serwa zostalo do wynonania
int step = 1; // kierunek obrotu (wartosc 1 lub -1 jest dodawana do OCR1A co powoduje obrot serwa)

int num_round(float number) { //zaokrąglanie floatów
   return (int) (number < 0 ? number - 0.5 : number + 0.5);
}

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
            if (i == 3) //EXIT function
            {
                {
                	*choice = -69; //powrot do menu
                    //LCD_HD44780::writeText("/");
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
                //LCD_HD44780::writeText("*");
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
                //LCD_HD44780::writeText("+");
            }
        }
        cbi(PORTC, i);
    }
}

void init_PWM()
{
	TCCR1A |= (1 << WGM10) | (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << CS12); //prescaler na 256 bitów
}

/* funkcja realizujaca "odbijanie" serwa
 * odbijanie moze zostac przerwane jesli zostanie wcisniety
 * przycisk powrotu do MENU
 */
void Bounce(int *choice){
	LCD_HD44780::clear();
	LCD_HD44780::writeText("Bounce, bounce");
	LCD_HD44780::goTo(0,1);
	while((*choice)!=-69){
		getKey(choice);
		OCR1A+=step;
		_delay_ms(100);
		if(OCR1A == SERV_MAX)
			step=-1;
		if(OCR1A == SERV_MIN)
			step=1;
	}
	//LCD_HD44780::clear();
}

/* funkcja odpowiedzialna za obracanie serwa o podany przez uzytkownika kat.
 * Kat musi byc dodatni. Serwo obraca sie w zakresie 0-180 stopni,
 * wiec jesli podany kat przekroczy ten zakres, serwo odbije sie od gornej
 * lub dolnej granicy i obroci w druga strone o wartosc kata jaka pozostala.
 */

void Rotate(int *choice, char *kat){ // Obrót serwa o dowolny kat
	char c; //zmienna potrzebna do konwersji int na char
	int angle = -1;
	int pom = angle;
	int rot_angle;
	int rot_serv;
	LCD_HD44780::clear();

	getKey(choice);
	if(*choice!=-69){
		if((*choice)!=11 && angleOk == false)
		{
			LCD_HD44780::writeText("What's the angle: ");
			LCD_HD44780::goTo(0,1);
			angle=(*choice);
			if(angle!=pom){ //sprawdzenie czy przycisk zostal wcisniety
				c = angle+'0'; 		//zamiana liczby na char
				strncat(kat, &c, 1);	//dopisanie (cyfry) znaku do dotychczasowego łańcucha (liczby)
			}
			LCD_HD44780::showNumber(atoi(kat));
		}
		else{
			angleOk = true;
			rot_angle = atoi(kat);
			memset( kat, 0, sizeof(kat) ); //wyzerowanie wartosci kata
			*choice = -1;
			//angle = -1;
		}

		if(angleOk == true) //wartośc choice==11 oznacza, ze akceptujemy wybrany kat
		{
			LCD_HD44780::writeText("Rotating by: ");
			LCD_HD44780::showNumber(rot_angle);
			LCD_HD44780::goTo(0,1);
			LCD_HD44780::writeText("Rotating...");

			/*tutaj servo wchodzi*/
			rot_serv = num_round( (SERV_MAX-SERV_MIN)*rot_angle/180.0); //kat obrotu w jednostkach serwomechanizmu
			/* przeliczanie */
			tikRemain+=rot_serv;

			while(tikRemain>0){
				OCR1A+=step;
				_delay_ms(200);
				tikRemain--;
				if(OCR1A == SERV_MAX)
					step=-1;
				if(OCR1A == SERV_MIN)
					step=1;
			}
			angleOk = false;
		}
	}
	_delay_ms(200);
}




void Exit(){ // Wylaczenie serwa i ekranu
	LCD_HD44780::clear();
	LCD_HD44780::writeText("So long...");
	LCD_HD44780::goTo(0,1);
	LCD_HD44780::writeText("SUCKERS");
	_delay_ms(2000);
	LCD_HD44780::writeCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF); // wylaczenie ekranu
	OCR1A = SERV_MIN;
}

void Menu(int *choice,int *page ,char *opcje[]){

	LCD_HD44780::clear();
	getKey(choice); // jesli w menu wybieranie opcji(3 mozliwosci: pokaz nastepna opcje, pokaz poprzednia opcje, wybierz opcje)
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
	LCD_HD44780::goTo(0,1);
	LCD_HD44780::writeText(opcje[*page]); //tu wypisywana jest aktualnie przegladana opcja

	if(*choice == 5){
		do_Menu = false;
	}

	_delay_ms(200);
}
int main()
{
	int wybor=0, strona = 0; char kat[]="";
	char *opcje1[]= {"0 - Motor Bounce ",
					 "1 - Rotate ",
					 "2 - Exit " }; //tablica opcji

    DDRC = 0b00001111;
    PORTC = 0xff;
    init_PWM();

    OCR1A = SERV_MIN; //początkowo servo w pozycji 0
    DDRD|=(1<<PD4)|(1<<PD5);   //Piny od PWM ustawiamy na wyjścia
    LCD_HD44780::init();

    while (1){
    	if(do_Menu == true)
    	Menu(&wybor,&strona,opcje1);
    	else{
    		switch (strona)
    		{
    		    case 0: Bounce(&wybor);
    		    if(wybor == -69){
    		    	  do_Menu = true;
    		       }
    		        break;
    		    case 1: Rotate(&wybor,kat);
    		    		if(wybor == -69){
    		    			do_Menu = true;
    		    		}
    		    	break;
    		    case 2: Exit();
    		    	return 0;
    		        break;
    		    default:
    		    	break;
    		}
    	}
    	wybor = -1;
    }
}
