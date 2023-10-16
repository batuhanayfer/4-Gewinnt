/*
 * Spielfeld.c
 *
 * Created: 06.11.2019 15:21:01
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



typedef enum
{
    NONE, PLAYER1, PLAYER2
}
player_t; //player_t+
player_t Zug, Winner;

/******************************************************************************\
* <<Clear_Spielfeld>>
*
* Spielfeld löschen
*
\******************************************************************************/
void Clear_Spielfeld(void)
{
    for (uint8_t i=0;i<X_achse;i++)
    {
        for (uint8_t t=0;t<Y_achse;t++)
        {
            Spielfeld[i][t] = NONE;
        }
    }
}

/******************************************************************************\
* <<Get_Fall_Step>>
*
* Schaut bis wo die Steine fallen darf
*
* Parameter:
* <<x>> = <<x koordinate>>
*
* Rückgabewert: <<gibt fallgrenze aus>>
*
\******************************************************************************/    
uint8_t Get_Fall_Step(uint8_t x)
{
    uint8_t fallgrenze;
    if (Spielfeld[x][5] == NONE)
    {
        fallgrenze = 5;
    }
    for (uint8_t i=0; i<Y_achse;i++)
    {
        if (Spielfeld[x][i]!=NONE)
        {
            fallgrenze = i-1;
            break;
        }
    }
    return fallgrenze;
}

/******************************************************************************\
* <<Pos_x_Free>>
*
* Überprüft ob die x Koordinate frei ist
*
* Parameter:
* <<x>> = <<x Koordinate>>
*
* Rückgabewert: <<Frei oder Besetzt>>
*
\******************************************************************************/
uint8_t Pos_X_Free(uint8_t x)
{
    uint8_t status;
    if (Spielfeld[x][0] == NONE)
    {
         status = Frei;
    }
    else
    {
        status = Besetzt;
    }
    return status;
    
}

/******************************************************************************\
* <<Set_Spieler>>
*
* Spieler in entsprechende Spielfeld setzen
*
* Parameter:
* <<x>> = <<x Koordinate>>
* <<fall>> = <<fallgrenze>>
* <<spieler>> = <<Welche Spieler>>
*
\******************************************************************************/
void Set_Spieler( uint8_t x,uint8_t fall,uint8_t spieler)
{
    Spielfeld[x][fall] = spieler;
}

/******************************************************************************\
* <<Check_Winner>>
*
* überprüfen wer gewonnen hat
*
* Rückgabewert: <<x-y Koordinate wer und wie gewonnen hat>>
*
\******************************************************************************/

uint16_t Check_Winner(void)
{
    //oben nach unten
    for (uint8_t x = 0; x<X_achse;x++)
    {
        for (uint8_t y=0; y<3;y++)
        {
            if (Spielfeld[x][y]==PLAYER1&&Spielfeld[x][y+1]==PLAYER1&&Spielfeld[x][y+2]==PLAYER1&&Spielfeld[x][y+3]==PLAYER1)
            {
                 Winner = PLAYER1;
                return (x|(y<<4)|(1<<8));
            }
        }
        
    }
    for (uint8_t x = 0; x<X_achse;x++)
    {
        for (uint8_t y=0; y<3;y++)
        {
            if (Spielfeld[x][y]==PLAYER2&&Spielfeld[x][y+1]==PLAYER2&&Spielfeld[x][y+2]==PLAYER2&&Spielfeld[x][y+3]==PLAYER2)
            {
                Winner = PLAYER2;
                return (x|(y<<4)|(1<<8));
            }
        }
    }
    //rechts nach links
    for (uint8_t y = 0; y<Y_achse;y++)
    {
        for (uint8_t x=0; x<4;x++)
        {
            if (Spielfeld[x][y]==PLAYER1&&Spielfeld[x+1][y]==PLAYER1&&Spielfeld[x+2][y]==PLAYER1&&Spielfeld[x+3][y]==PLAYER1)
            {
                Winner = PLAYER1;
                return (x|(y<<4)|(2<<8));
            }
        }
    }
    for (uint8_t y = 0; y<Y_achse;y++)
    {
        for (uint8_t x=0; x<4;x++)
        {
            if (Spielfeld[x][y]==PLAYER2&&Spielfeld[x+1][y]==PLAYER2&&Spielfeld[x+2][y]==PLAYER2&&Spielfeld[x+3][y]==PLAYER2)
            {
                Winner = PLAYER2;
                return (x|(y<<4)|(2<<8));
            }
        }
    }
    //link oben nach rechts unten
    for (uint8_t y = 0; y<3;y++)
    {
        for (uint8_t x=0; x<4;x++)
        {
            if (Spielfeld[x][y]==PLAYER1&&Spielfeld[x+1][y+1]==PLAYER1&&Spielfeld[x+2][y+2]==PLAYER1&&Spielfeld[x+3][y+3]==PLAYER1)
            {
                Winner = PLAYER1;
                return (x|(y<<4)|(3<<8));
            }
        }
    }
    for (uint8_t y = 0; y<3;y++)
    {
        for (uint8_t x=0; x<4;x++)
        {
            if (Spielfeld[x][y]==PLAYER2&&Spielfeld[x+1][y+1]==PLAYER2&&Spielfeld[x+2][y+2]==PLAYER2&&Spielfeld[x+3][y+3]==PLAYER2)
            {
                Winner = PLAYER2;
                return (x|(y<<4)|(3<<8));
            }
        }
    }
    //links unten rechts oben
   for (uint8_t y = 5; y>2;y--)
   {
       for (uint8_t x=0; x<4;x++)
       {
           if (Spielfeld[x][y]==PLAYER1&&Spielfeld[x+1][y-1]==PLAYER1&&Spielfeld[x+2][y-2]==PLAYER1&&Spielfeld[x+3][y-3]==PLAYER1)
           {
               Winner = PLAYER1;
               return (x|(y<<4)|(4<<8));
           }
       }
   }
    for (uint8_t y = 5; y>2;y--)
    {
        for (uint8_t x=0; x<4;x++)
        {
            if (Spielfeld[x][y]==PLAYER2&&Spielfeld[x+1][y-1]==PLAYER2&&Spielfeld[x+2][y-2]==PLAYER2&&Spielfeld[x+3][y-3]==PLAYER2)
            {
                Winner = PLAYER2;
                return (x|(y<<4)|(4<<8));
            }
        }
    }
    Winner=NONE;
    
    
}


/******************************************************************************\
* <<Spielfeld_voll>>
*
* Überprüft ob das Feld voll ist
*
* Rückgabewert: <<Feld voll (1) Feld nicht voll (0)>>
*
\******************************************************************************/
uint8_t Spielfeld_voll(void)
{
    for (uint8_t i=0;i<X_achse;i++)
    {
        for (uint8_t t=0;t<Y_achse;t++)
        {
            
           if (Spielfeld[i][t] == NONE)
           {
               return 0;
           }
            
        }
    }
    return 1;
}