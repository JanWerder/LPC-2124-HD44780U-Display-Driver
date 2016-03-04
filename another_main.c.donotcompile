#include "stdio.h"
#include <libarm.h>
#include <stdbool.h>
#include <targets/LPC2000.h>

int characterCount = 0;
int characterNewPageDelay = 100;

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
        IO0PIN = ((int) c << 8); //Character Code übertragen, aber um 8 Pins verschoben
	LCD_Enable(); //Schalte kurz den "E" Pin an, um die Instruction zu verarbeiten
}

void LCD_WriteString(char * string)
{
        LCD_Init();
	int c=0;
	while (string[c]!='\0') // Schreibe bis der String am Ende ist
	{
            characterCount++;
            if(characterCount == 17){
              LCD_Linebreak();
            }
            if(characterCount == 33){
              for (int i=0;i<characterNewPageDelay;i++){ //verzögere den Umbruch auf neue Seite
                delay();
              }
              LCD_Init();
              characterCount = 0;
            }
            LCD_WriteChar(string[c]); // int castet den Character automatisch von ascii nach binär und würde damit automatisch auf die Pins 0-7 gemapped werden. Via Bitshift, wird dieses Mapping auf 8-15 erhöht
            c++;
	}
        characterCount = 0;
        
}

void LCD_Init()
{
        //Alle Werte haben einen Offset von 8 um auf Port 8-15 statt 0-7 zu laufen.
        IO0DIR = 0xFF00;// P0.8-P0.15 sind der Data Output  (8 Bit mode)
	IO1DIR |= (1<<16) | (1<<17); //P1.16 und P1.17 sind Control Pins
        IO0PIN = 0x100; //Setze Port9 zurück auf 0.	
        IO1PIN = 0x100;  //Setze Port10 zurück auf 0 .


	//LCD Init Sequenz
	delay(); //Anfangsverzögerung
        LCD_Cmd(0x3C00);//Setzte das "Function Set" : 8 Bit Mode , 2 Zeilen , 5x10 Font
	
        LCD_Cmd(0x0F00);//Setze den "Display Switch" : Display an , Cursor an , Blinken an
        LCD_Cmd(0x0600);//Setze das "Input Set" : Increment Mode 
        LCD_Cmd(0x0100);//Veranlasse einen Reset: Screen clear , Cursor auf Standard
        LCD_Cmd(0x8000);//Wird beim ersten LCD Init nicht benötigt, würde aber den Cursor nochmal zurücksetzen.
	//Done!
}

void LCD_Linebreak()
{
	LCD_Cmd(0x8000 + 0x4000); //Gehe in die zweite Zeile
}

int main(void) {
        
        LCD_Init();
        LCD_WriteString("Life is about making an impact, not making an income. –Kevin Kruse");
        LCD_Linebreak();
	LCD_WriteString(" Two roads diverged in a wood, and I—I took the one less traveled by, And that has made all the difference.  –Robert Frost");


        while(1); 
	return 0; 
}