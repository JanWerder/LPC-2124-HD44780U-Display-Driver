#include "stdio.h"
#include <libarm.h>
#include <stdbool.h>
#include <targets/LPC2000.h>
#include "arraylist.h"

static ArrayList *list;

void LCD_Cmd(unsigned int cmd)
{
	IO1PIN = 0x0; //Gehe in den Instruction Mode
	IO0PIN = cmd; //Stelle den Instruction/Command Code bereit
	LCD_Enable(); //Schalte kurz den "E" Pin an, um die Instruction zu verarbeiten
}

void LCD_Enable(void)
{
	delay();
	IO1PIN |=  (1<<17);//High
	delay();
	IO1PIN &= ~(1<<17);//Low
	delay();
}

void delay(void)
{
	int i=0,x=0;
	for(i=0; i<19999; i++){ x++; }
}

void LCD_WriteChar(char c)
{
	IO1PIN |= (1<<16); //Wechsele zum "Data Mode"
	IO0PIN = (int) c; //Character Code übertragen
	LCD_Enable(); //Schalte kurz den "E" Pin an, um die Instruction zu verarbeiten
}

void LCD_WriteString(char * string)
{
	int c=0;
	while (string[c]!='\0') // Schreibe bis der String am Ende ist
	{
            list->add( list, string[c] );
            if(list->length == 16){
              LCD_Linebreak();
            }
            if(list->length == 32){
              delay();
              LCD_Init();
            }
            LCD_WriteChar(string[c]);
            c++;
	}
}

void LCD_Init()
{
        list = newArrayList(sizeof(char));

        IO0DIR = 0xFF; //P0.0 bis P0.7 sind der Data Output  (8 Bit mode)
	IO1DIR |= (1<<16) | (1<<17) | (1<<18); //P1.16 und P1.17 sind Control Pins
	IO0PIN = 0x0; //Setze Port0 zurück auf 0.	
	IO1PIN = 0x0; //Setze Port1 zurück auf 0 .

        IO1PIN |=  (1<<18);

	//LCD Init Sequenz
	delay(); //Anfangsverzögerung
	LCD_Cmd(0x3C); //Setzte das "Function Set" : 8 Bit Mode , 2 Zeilen , 5x10 Font
	LCD_Cmd(0x0F); //Setze den "Display Switch" : Display an , Cursor an , Blinken an
	LCD_Cmd(0x06); //Setze das "Input Set" : Increment Mode 
	LCD_Cmd(0x01); //Veranlasse einen Reset: Screen clear , Cursor auf Standard
	LCD_Cmd(0x80); //Wird beim ersten LCD Init nicht benötigt, würde aber den Cursor nochmal zurücksetzen.
	//Done!
}

void LCD_Linebreak()
{
	LCD_Cmd(0x80 + 0x40); //Gehe in die zweite Zeile
}

int main(void) {
        
        LCD_Init();
        LCD_WriteString("TechnischeInfoInfoInfoInfoInfo");
        LCD_Linebreak();
	LCD_WriteString("Info");


        while(1); 
	return 0; 
}