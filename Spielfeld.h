/*
 * Spielfeld.h
 *
 * Created: 06.11.2019 15:21:18
 *  Author: Batuhan Ayfer
 */ 


#ifndef ANZEIGEMODUL_H_
#define ANZEIGEMODUL_H_

typedef enum
{
    Frei, Besetzt
}state_typ;
state_typ state;

#define X_achse 7
#define Y_achse 6

uint8_t Spielfeld[X_achse][Y_achse];



void Clear_Spielfeld(void);
uint8_t Get_Fall_Step(uint8_t x);
uint8_t Pos_X_Free(uint8_t x);
void Set_Spieler( uint8_t x,uint8_t fall,uint8_t spieler);
uint16_t Check_Winner(void);
uint16_t get_Info(void);
uint8_t Spielfeld_voll(void);


#endif /* ANZEIGEMODUL_H_ */