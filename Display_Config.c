/*
 * Display_Config.c
 *
 * Created: 28/02/2019 15:37:13
 *  Author: Administrator
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
struct cRGB led[Anzahl_LEDs];

uint8_t Positions[16] = {71,70,69,68,67,66,65,64,7,6,5,4,3,2,1,0};

/******************************************************************************\
* <<Set_Pixel>>
*
* Schaltet einzelne LED's ein oder aus
*
* Parameter:
* <<x>> = <<x Koordinate>>
* <<y>> = <<y Koordinate>>
* <<rot>> = <<Stärke der Farbe ROT >>
* <<Gruen>> = <<Stärke der Farbe GRUEN >>
* <<Blau>> = <<Stärke der Farbe BLAU >>
*
*
\******************************************************************************/
void SetPixel(uint8_t X_Axis, uint8_t Y_Axis, uint8_t Rot, uint8_t Gruen, uint8_t Blau)
{
	if (Y_Axis>7)
    {
        led[(Positions[X_Axis] + Y_Axis * 8)+64].r = Rot;
        led[(Positions[X_Axis] + Y_Axis * 8)+64].g = Gruen;
        led[(Positions[X_Axis] + Y_Axis * 8)+64].b = Blau;
    } 
    else
    {
	    led[((Positions[X_Axis]) + Y_Axis * 8)].r = Rot;
		led[((Positions[X_Axis]) + Y_Axis * 8)].g = Gruen;
		led[((Positions[X_Axis]) + Y_Axis * 8)].b = Blau;
    }
}

/******************************************************************************\
* <<ShowDisplay>>
*
* Zeigt die LED's auf dem Display an
*
*
\******************************************************************************/
void ShowDisplay(void)
{
	ws2812_setleds(led,Anzahl_LEDs);
}

/******************************************************************************\
* <<ResetLeds>>
*
* LED's löschen
*
\******************************************************************************/
void ResetLeds(void)
{
	for (uint8_t i = 0; i < 16; i++)
	{
        for (uint8_t k = 0; k < 16; k++)
        {
            SetPixel(i,k,0,0,0);
        }
		
	}
}