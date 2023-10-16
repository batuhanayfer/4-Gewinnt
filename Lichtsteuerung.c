/*
 * Lichtsteuerung.c
 *
 * Created: 06.11.2019 15:21:46
 *  Author: Batuhan Ayfer
 */ 


#include <avr/io.h>
#define F_CPU	16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include "Display_Config.h"
#include "ws2812_config.h"
#include "light_ws2812.h"
#include "Spielfeld.h"
#include "Lichtsteuerung.h"
#include <util/atomic.h>
#define HELLIGKEIT 5    
#define WARTEZEIT 300
#define FALLSPEED 50

typedef enum
{
    NONE, PLAYER1, PLAYER2
}
player_t; //player_t+
player_t Zug, Winner;

/******************************************************************************\
* <<Spielstein_Fallen>>
*
* Fall animation
*
* Parameter:
* <<x>> = <<x koordinate>>
* <<y>> = <<y koordinate>>
* <<farbe>> = <<farbe der LED's>>
*
*
\******************************************************************************/
void Spielstein_Fallen(uint8_t x, uint8_t y, uint8_t farbe)
{
    
    for (uint8_t i = 0;i<=y;i++)
    {
        Setze_Steine(x,i,farbe,HELLIGKEIT);
        _delay_ms(FALLSPEED);
        if (i>2)
        {
            Setze_Steine(x,i-3,0,0);
        }        
        if (i>1)
        {
           Setze_Steine(x,i-2,farbe,HELLIGKEIT/3);
        }        
        if (i>0)
        {
            Setze_Steine(x,i-1,farbe,HELLIGKEIT/2);
        }
        
    }
    if (y>0)
    {
        Setze_Steine(x,y-1,farbe,0);
    }
    if (y>1)
    {
        Setze_Steine(x,y-2,farbe,0);
    }
    
}

/******************************************************************************\
* <<Setze_Steine>>
*
* Den entsprechenden Spielstein einschalten
*
* Parameter:
* <<x>> = <<x koordinate>>
* <<y>> = <<y koordinate>>
* <<farbe>> = <<farbe der LED's>>
* <<helligkeit>> = <<heilligkeit der LED's>>
*
\******************************************************************************/
void Setze_Steine(uint8_t x, uint8_t y, uint8_t farbe,uint8_t helligkeit)
{
        x=(x*2)+1;
        y=(5-y);
        y=(y*2)+4;    
    
    
    
    if (farbe==Blau)
    {
        SetPixel(x,y,0,0,helligkeit);
        SetPixel(x+1,y,0,0,helligkeit);
        SetPixel(x,y+1,0,0,helligkeit);
        SetPixel(x+1,y+1,0,0,helligkeit);
    }
    if (farbe==Rot)
    {
        SetPixel(x,y,helligkeit,0,0);
        SetPixel(x+1,y,helligkeit,0,0);
        SetPixel(x,y+1,helligkeit,0,0);
        SetPixel(x+1,y+1,helligkeit,0,0);
    }
    if (farbe==Gruen)
    {
        SetPixel(x,y,0,helligkeit,0);
        SetPixel(x+1,y,0,helligkeit,0);
        SetPixel(x,y+1,0,helligkeit,0);
        SetPixel(x+1,y+1,0,helligkeit,0);
    }
    if (farbe==Gelb)
    {
        SetPixel(x,y,helligkeit,helligkeit,0);
        SetPixel(x+1,y,helligkeit,helligkeit,0);
        SetPixel(x,y+1,helligkeit,helligkeit,0);
        SetPixel(x+1,y+1,helligkeit,helligkeit,0);
    }
    if (farbe==Cyan)
    {
        SetPixel(x,y,0,helligkeit,helligkeit);
        SetPixel(x+1,y,0,helligkeit,helligkeit);
        SetPixel(x,y+1,0,helligkeit,helligkeit);
        SetPixel(x+1,y+1,0,helligkeit,helligkeit);
    }
     if (farbe==Magenta)
     {
         SetPixel(x,y,helligkeit,0,helligkeit);
         SetPixel(x+1,y,helligkeit,0,helligkeit);
         SetPixel(x,y+1,helligkeit,0,helligkeit);
         SetPixel(x+1,y+1,helligkeit,0,helligkeit);
     }
     if (farbe==Weiss)
     {
         SetPixel(x,y,helligkeit,helligkeit,helligkeit);
         SetPixel(x+1,y,helligkeit,helligkeit,helligkeit);
         SetPixel(x,y+1,helligkeit,helligkeit,helligkeit);
         SetPixel(x+1,y+1,helligkeit,helligkeit,helligkeit);
     }
     if (farbe==Aus)
     {
         SetPixel(x,y,0,0,0);
         SetPixel(x+1,y,0,0,0);
         SetPixel(x,y+1,0,0,0);
         SetPixel(x+1,y+1,0,0,0);
     }
    
    ShowDisplay();
    
}

/******************************************************************************\
* <<Izipizilemonskuizi>>
*
* Steine des Gewinners blinken lassen
*
* Parameter:
* <<x>> = <<x koordinate>>
* <<y>> = <<y koordinate>>
* <<farbe>> = <<farbe der LED's>>
* <<win_style>> = <<mit welche von 4 Variante man gewonnen hat>>
*
\******************************************************************************/
void Izipizilemonskuizi(uint8_t x, uint8_t y, uint8_t win_style,uint8_t farbe )
{
    if (Winner==PLAYER1)
   {
	
	    if (win_style == 1)
	    {
	        for (uint8_t i=0;i<8;i++)
	        {
                Setze_Steine(x,y,farbe,HELLIGKEIT);
                Setze_Steine(x,y+1,farbe,HELLIGKEIT);
                Setze_Steine(x,y+2,farbe,HELLIGKEIT);
                Setze_Steine(x,y+3,farbe,HELLIGKEIT);
                _delay_ms(WARTEZEIT);
                Setze_Steine(x,y,Aus,HELLIGKEIT);
                Setze_Steine(x,y+1,Aus,HELLIGKEIT);
                Setze_Steine(x,y+2,Aus,HELLIGKEIT);
                Setze_Steine(x,y+3,Aus,HELLIGKEIT);
                _delay_ms(WARTEZEIT);
	        }
	    }
	    if (win_style ==2)
	    {
	        for (uint8_t i=0;i<8;i++)
	        {
    	        Setze_Steine(x,y,farbe,HELLIGKEIT);
    	        Setze_Steine(x+1,y,farbe,HELLIGKEIT);
    	        Setze_Steine(x+2,y,farbe,HELLIGKEIT);
    	        Setze_Steine(x+3,y,farbe,HELLIGKEIT);
    	        _delay_ms(WARTEZEIT);
    	        Setze_Steine(x,y,Aus,HELLIGKEIT);
    	        Setze_Steine(x+1,y,Aus,HELLIGKEIT);
    	        Setze_Steine(x+2,y,Aus,HELLIGKEIT);
    	        Setze_Steine(x+3,y,Aus,HELLIGKEIT);
    	        _delay_ms(WARTEZEIT);
	        }
	    }
	    if (win_style ==3)
	    {
	        for (uint8_t i=0;i<8;i++)
	        {
    	        Setze_Steine(x,y,farbe,HELLIGKEIT);
    	        Setze_Steine(x+1,y+1,farbe,HELLIGKEIT);
    	        Setze_Steine(x+2,y+2,farbe,HELLIGKEIT);
    	        Setze_Steine(x+3,y+3,farbe,HELLIGKEIT);
    	        _delay_ms(WARTEZEIT);
    	        Setze_Steine(x,y,Aus,HELLIGKEIT);
    	        Setze_Steine(x+1,y+1,Aus,HELLIGKEIT);
    	        Setze_Steine(x+2,y+2,Aus,HELLIGKEIT);
    	        Setze_Steine(x+3,y+3,Aus,HELLIGKEIT);
    	        _delay_ms(WARTEZEIT);
	        }
	    }
	    if (win_style ==4)
	    {
	       for (uint8_t i=0;i<8;i++)
	        {
    	        Setze_Steine(x,y,farbe,HELLIGKEIT);
    	        Setze_Steine(x+1,y-1,farbe,HELLIGKEIT);
    	        Setze_Steine(x+2,y-2,farbe,HELLIGKEIT);
    	        Setze_Steine(x+3,y-3,farbe,HELLIGKEIT);
    	        _delay_ms(WARTEZEIT);
    	        Setze_Steine(x,y,Aus,HELLIGKEIT);
    	        Setze_Steine(x+1,y-1,Aus,HELLIGKEIT);
    	        Setze_Steine(x+2,y-2,Aus,HELLIGKEIT);
    	        Setze_Steine(x+3,y-3,Aus,HELLIGKEIT);
    	        _delay_ms(WARTEZEIT);
	        }
	    }
  }
   if (Winner==PLAYER2)
   {
	
	    if (win_style == 1)
	    {
	        for (uint8_t i=0;i<8;i++)
	        {
                Setze_Steine(x,y,farbe,HELLIGKEIT);
                Setze_Steine(x,y+1,farbe,HELLIGKEIT);
                Setze_Steine(x,y+2,farbe,HELLIGKEIT);
                Setze_Steine(x,y+3,farbe,HELLIGKEIT);
                _delay_ms(WARTEZEIT);
                Setze_Steine(x,y,Aus,HELLIGKEIT);
                Setze_Steine(x,y+1,Aus,HELLIGKEIT);
                Setze_Steine(x,y+2,Aus,HELLIGKEIT);
                Setze_Steine(x,y+3,Aus,HELLIGKEIT);
                _delay_ms(WARTEZEIT);
	        }
	    }
	    if (win_style ==2)
	    {
	        for (uint8_t i=0;i<8;i++)
	        {
    	        Setze_Steine(x,y,farbe,HELLIGKEIT);
    	        Setze_Steine(x+1,y,farbe,HELLIGKEIT);
    	        Setze_Steine(x+2,y,farbe,HELLIGKEIT);
    	        Setze_Steine(x+3,y,farbe,HELLIGKEIT);
    	        _delay_ms(WARTEZEIT);
    	        Setze_Steine(x,y,Aus,HELLIGKEIT);
    	        Setze_Steine(x+1,y,Aus,HELLIGKEIT);
    	        Setze_Steine(x+2,y,Aus,HELLIGKEIT);
    	        Setze_Steine(x+3,y,Aus,HELLIGKEIT);
    	        _delay_ms(WARTEZEIT);
	        }
	    }
	    if (win_style ==3)
	    {
	        for (uint8_t i=0;i<8;i++)
	        {
    	        Setze_Steine(x,y,farbe,HELLIGKEIT);
    	        Setze_Steine(x+1,y+1,farbe,HELLIGKEIT);
    	        Setze_Steine(x+2,y+2,farbe,HELLIGKEIT);
    	        Setze_Steine(x+3,y+3,farbe,HELLIGKEIT);
    	        _delay_ms(WARTEZEIT);
    	        Setze_Steine(x,y,Aus,HELLIGKEIT);
    	        Setze_Steine(x+1,y+1,Aus,HELLIGKEIT);
    	        Setze_Steine(x+2,y+2,Aus,HELLIGKEIT);
    	        Setze_Steine(x+3,y+3,Aus,HELLIGKEIT);
    	        _delay_ms(WARTEZEIT);
	        }
	    }
	    if (win_style ==4)
	    {
	       for (uint8_t i=0;i<8;i++)
	        {
    	        Setze_Steine(x,y,farbe,HELLIGKEIT);
    	        Setze_Steine(x+1,y-1,farbe,HELLIGKEIT);
    	        Setze_Steine(x+2,y-2,farbe,HELLIGKEIT);
    	        Setze_Steine(x+3,y-3,farbe,HELLIGKEIT);
    	        _delay_ms(WARTEZEIT);
    	        Setze_Steine(x,y,Aus,HELLIGKEIT);
    	        Setze_Steine(x+1,y-1,Aus,HELLIGKEIT);
    	        Setze_Steine(x+2,y-2,Aus,HELLIGKEIT);
    	        Setze_Steine(x+3,y-3,Aus,HELLIGKEIT);
    	        _delay_ms(WARTEZEIT);
	        }
	    }
  }
    
    
    
}

/******************************************************************************\
* <<Izipizilemonskuizi>>
*
* Steine des Gewinners blinken lassen
*
* Parameter:
* <<x>> = <<x koordinate>>
* <<y>> = <<y koordinate>>
* <<farbe>> = <<farbe der LED's>>
* <<win_style>> = <<mit welche von 4 Variante man gewonnen hat>>
*
\******************************************************************************/
uint8_t Counter_Turn_Timer(uint8_t timer)
{

   uint8_t countdown = 16-timer;
    if (timer==0)
    {
        
        for (uint8_t i=15;i>3;i--)
        {
            SetPixel(0,i,0,5,5);
            SetPixel(15,i,0,5,5);
        }

    }
    if (timer)
    {   
        SetPixel(0,countdown+1,0,0,0);
        SetPixel(15,countdown+1,0,0,0);
    }
    if (timer==13)
    {
        return 1;
    }
    ShowDisplay();
   
   
    
    
}

/******************************************************************************\
* <<Score_anzeige>>
*
* Steine des Gewinners blinken lassen
*
* Parameter:
* <<x>> = <<x koordinate>>
* <<y>> = <<y koordinate>>
* <<farbe>> = <<farbe der LED's>>
* <<win_style>> = <<mit welche von 4 Variante man gewonnen hat>>
*
\******************************************************************************/
void Score_Anzeige(uint8_t score1,uint8_t score2)
{
     if (score1==0)
    {
        
        SetPixel(2,2,0,10,0);
        SetPixel(2,1,0,10,0);
        SetPixel(2,0,0,10,0);
        SetPixel(2,3,0,10,0);
        SetPixel(2,0,0,10,0);
        SetPixel(3,3,0,10,0);
        SetPixel(3,0,0,10,0);
        SetPixel(4,3,0,10,0);
        SetPixel(4,2,0,10,0);
        SetPixel(4,1,0,10,0);
        SetPixel(4,0,0,10,0);
        
    }
    if (score1==1)
    {
        SetPixel(1,2,0,10,0);
        SetPixel(2,3,0,10,0);
        SetPixel(3,3,0,10,0);
        SetPixel(3,2,0,10,0);
        SetPixel(3,1,0,10,0);
        SetPixel(3,0,0,10,0);
    }
    if (score1==2)
    {
        
        SetPixel(2,3,0,10,0);
        SetPixel(3,3,0,10,0);
        SetPixel(3,2,0,10,0);
        SetPixel(2,1,0,10,0);
        SetPixel(2,0,0,10,0);
        SetPixel(3,0,0,10,0);
    }
    if (score1==3)
    {
        
        SetPixel(2,3,0,10,0);
        SetPixel(3,3,0,10,0);
        SetPixel(3,2,0,10,0);
        SetPixel(4,3,0,10,0);
        SetPixel(4,2,0,10,0);
        SetPixel(4,1,0,10,0);
        SetPixel(4,0,0,10,0);
        SetPixel(2,0,0,10,0);
        SetPixel(3,0,0,10,0);
    }
     if (score2==0)
     {
         
         SetPixel(2+9,2,0,10,0);
         SetPixel(2+9,1,0,10,0);
         SetPixel(2+9,0,0,10,0);
         SetPixel(2+9,3,0,10,0);
         SetPixel(2+9,0,0,10,0);
         SetPixel(3+9,3,0,10,0);
         SetPixel(3+9,0,0,10,0);
         SetPixel(4+9,3,0,10,0);
         SetPixel(4+9,2,0,10,0);
         SetPixel(4+9,1,0,10,0);
         SetPixel(4+9,0,0,10,0);
         
     }
     if (score2==1)
     {
         SetPixel(1+9,2,0,10,0);
         SetPixel(2+9,3,0,10,0);
         SetPixel(3+9,3,0,10,0);
         SetPixel(3+9,2,0,10,0);
         SetPixel(3+9,1,0,10,0);
         SetPixel(3+9,0,0,10,0);
     }
     if (score2==2)
     {
         
         SetPixel(2+9,3,0,10,0);
         SetPixel(3+9,3,0,10,0);
         SetPixel(3+9,2,0,10,0);
         SetPixel(2+9,1,0,10,0);
         SetPixel(2+9,0,0,10,0);
         SetPixel(3+9,0,0,10,0);
     }
     if (score2==3)
     {
         
         SetPixel(2+9,3,0,10,0);
         SetPixel(3+9,3,0,10,0);
         SetPixel(3+9,2,0,10,0);
         SetPixel(4+9,3,0,10,0);
         SetPixel(4+9,2,0,10,0);
         SetPixel(4+9,1,0,10,0);
         SetPixel(4+9,0,0,10,0);
         SetPixel(2+9,0,0,10,0);
         SetPixel(3+9,0,0,10,0);
     }
    
    
    for (uint8_t i = 0;i<4;i++)
    {
        SetPixel(7,i,10,10,10);
        SetPixel(8,i,10,10,10);
    }
    ShowDisplay();
}