/*
 * Display_Config.h
 *
 * Created: 28/02/2019 15:03:01
 *  Author: Administrator
 */ 


#ifndef DISPLAY_CONFIG_H_
#define DISPLAY_CONFIG_H_
#include "light_ws2812.h"
#define X_length	16
#define Y_length	16
#define Anzahl_LEDs	(X_length * Y_length)


#define LeseLeftPL1		(PINK & 0x01)
#define LeseRightPL1	(PINK & 0x02)
#define LeseSetzenPL1	(PINK & 0x04)
#define LeseLeftPL2		(PINK & 0x08)

#define LeseRightPL2	(PINK & 0x10)
#define LeseSetzenPL2	(PINK & 0x20)



extern struct cRGB led[Anzahl_LEDs];

void SetPixel(uint8_t X_Axis, uint8_t Y_Axis, uint8_t Rot, uint8_t Gruen, uint8_t Blau);
void ShowDisplay(void);
void ResetLeds(void);

#endif /* DISPLAY_CONFIG_H_ */