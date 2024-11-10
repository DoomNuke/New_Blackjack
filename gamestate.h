#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdint.h>

#include "cards_l.h"

typedef enum Round_Outcomes{
    Lose = 2,
    Broke = -1,
    Quit = 0,
    Blackjack = 5,
    Win = 4,
    Tie = 3,
    TBD = 1 //To be decided/Undetermined
}Outcomes;

typedef struct Gamestate{
    Outcomes outcomes;
    uint16_t cash;
    uint16_t pot;
    Card_List Deck;
    Card_List Player;
    Card_List Dealer;
}Gamestate;

#endif