/*
* 4Gewinnt.c
*
* Created: 06.11.2019 15:19:19
* Author : Batuhan Ayfer
*/

//Initialisierung
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU	16000000UL
#include <util/delay.h>
#include "Display_Config.h"
#include "ws2812_config.h"
#include "light_ws2812.h"
#include "Spielfeld.h"
#include "Taste.h"
#include "Lichtsteuerung.h"
uint8_t timer_turn;

ISR(TIMER1_COMPA_vect)
{
    timer_turn++;
    
}

init_timer()
{
    TCCR1A = 0x00;        //CTC Modus // PORTB Toggle at Compare match
    TCCR1B = 0x0D;        //Teiler 1024
    OCR1A  = 15625/3;     // 10kHz*3 (T = 1s/3)
    TIMSK1 = 0x02;        //Compare Match A Interrupt Aktivieren
}

//Case erstellen
typedef enum
{
    STANDBY, SELECT_SPALTE, FALLEN, CHECK, SKOR, END
}
state_type;
state_type Status;

typedef enum
{
    NONE, PLAYER1, PLAYER2
}
player_t; //player_t+
player_t Zug, Winner;



int main(void)
{
    init_timer();
    //am Portl die Pull-ups einstellen
    DDRL = 0x00;    
    PORTL = 0xFF;
    //Spielfeld löschen
    Clear_Spielfeld();
    Status = STANDBY;
    Zug=PLAYER1;
    //LED's löschen
    ResetLeds();
    ShowDisplay();
    //Variabel initialisieren
    uint8_t x,fallgrenze,x_achse,y_achse,player,flanke=0,farbe,score_player1=0,score_player2=0;
    uint16_t winner_info;
    
    // Endlose Schleife
    while (1)
    {
        switch (Status)
        {
            case STANDBY:
            Score_Anzeige(score_player1,score_player2);
            //Taster Entprellen und einlesen
            if((get_Taster()<8) && flanke == 0)
            {
                sei();
                _delay_ms(100);
                flanke = 1;

                x=(7-get_Taster());
                if (x<8)                        //wenn einer Taste von 0-7 gedrückt worden ist
                {
                    timer_turn = 0;
                    Status=SELECT_SPALTE;       //status auf Select_spalte setzen
                }
                if (x==7)
                {
                    Status = STANDBY;
                }
                
            }
            else if((get_Taster()>8) && flanke == 1)
            {
                _delay_ms(100);
                flanke = 0;
            }
            
            if (Counter_Turn_Timer(timer_turn))
            {
                if (Zug==PLAYER1)
                {
                    Winner=PLAYER2;
                }
                else
                {
                     Winner=PLAYER1;
                }
                Status=END;                                      //Status auf END setzen
                break;
            }
            
            break;
            
            case SELECT_SPALTE:
            
            if (Pos_X_Free(x)==Frei)            //Wenn pos x frei ist
            {
                fallgrenze = Get_Fall_Step(x);  //falgrenze bestimmen
                Set_Spieler(x,fallgrenze,Zug);  //Spieler in spielfeld setzen
                Status=FALLEN;                  //status auf fallen setzen
            }
            else                                //sonst
            {
                Status = STANDBY;               //status auf standby setzen
            }
            
            break;
            
            case FALLEN:
            if (Zug==PLAYER1)           //ist Player 1 dran
            {
                farbe = Rot;            //farbe auf Rot setzen
            }
            else                       //sonst
            {
                farbe=Gelb;             //Farbe auf Gelb setzen
            }
            
            Spielstein_Fallen(x,fallgrenze,farbe);  //richtige Spielstein einschalten mit Fall animation 
           
            flanke = 1;
            Status = CHECK;                         //status auf Check setzen
            break;  
            
            case CHECK: 
            
            winner_info = Check_Winner();           //Winner info holen
            if (Winner!=NONE)                       //Wenn jemand gewonnen hat
            {
                cli();
                Izipizilemonskuizi(winner_info&0x000F,(winner_info&0x00F0)>>4,(winner_info&0x0F00)>>8,farbe);   //Steine des Gewinners blinken lassen
                Status=END;                                      //Status auf END setzen
                break;
            }
            if (Spielfeld_voll())       //Wenn spielfeld voll ist
            {
                Status= END;            //Status auf END setzen
                _delay_ms(2000);        //2Sekunde warten
                break;  
            }
             if (Zug==PLAYER1)                       //Wenn der Player1 dran ist
            {
                Zug = PLAYER2;                      //Player2's Zug
            }
            else
            {
                Zug=PLAYER1;                        //Player1's Zug
            }
            
            Status=STANDBY;     //Status auf Standby setzen
            break;
            
            case SKOR: 
            if (Winner==PLAYER1)
            {
                score_player1++;
            }
            else
            {
                score_player2++;                
            }
            Status=STANDBY;
            
            break;
            
            case END:
            cli();
            Clear_Spielfeld();   //Spielfeld löschen
            ResetLeds();                    //LED's ausschalten
            ShowDisplay();
            Status=SKOR;                 //Status auf Standby setzen
            break;
            
        }
    }
}

