/*
 * Taste.c
 *
 * Created: 11.11.2019 08:12:59
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


/******************************************************************************\
* <<get_Taster>>
*
* Taster einlesen
*
* Parameter:
* <<PARAMETER1>> = <<BEDEUTUNG>>
* <<PARAMETER2>> = <<BEDEUTUNG>>
*
* Rückgabewert: <<gibt 0 bis 7 oder 99 wenn nichts gedrückt ist>>
*
\******************************************************************************/

uint8_t get_Taster(void)
{
   for (uint8_t i=0;i<8;i++)
   {
        if (!((PINL>>i)&0x01))
        {
            return i;
        }
       
   }
   return 99;
}