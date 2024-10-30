#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdint.h>

#include "cards_l.h"

typedef struct Gamestate{
    uint16_t cash;
    uint16_t pot;
    Card_List Deck;
    Card_List Player;
    Card_List Dealer;
}Gamestate;

#endif