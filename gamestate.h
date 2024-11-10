#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdint.h>

#include "cards_l.h"

typedef enum Round_Outcomes{
    Lose = 2,
    Outcome_Broke = -1,
    Outcome_Quit = 0,
    Outcome_Blackjack = 5,
    Win = 4,
    Tie = 3,
    Outcome_TBD = 1
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