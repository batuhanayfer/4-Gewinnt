/*
 * Lichtsteuerung.h
 *
 * Created: 06.11.2019 15:22:12
 *  Author: Batuhan Ayfer
 */ 


#ifndef LICHTSTEUERUNG_H_
#define LICHTSTEUERUNG_H_

typedef enum
{
    Aus,Blau, Gruen, Rot, Gelb, Magenta, Cyan, Weiss
}
type_farbe;


void Setze_Steine(uint8_t x, uint8_t y, uint8_t farbe,uint8_t helligkeit);
void Winnerwinner_Chickendinner(void);
void Izipizilemonskuizi(uint8_t x, uint8_t y, uint8_t win_style,uint8_t farbe );
uint8_t Counter_Turn_Timer(uint8_t timer);
void Score_Anzeige(uint8_t score1,uint8_t score2);

#endif /* LICHTSTEUERUNG_H_ */