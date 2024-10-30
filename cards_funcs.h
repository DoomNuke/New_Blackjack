#ifndef CARDS_FUNCS_H
#define CARDS_FUNCS_H


#include <stdlib.h>
#include "cards_l.h"


void Cards_Init(Card_List *cards);//Initializing the cards
void Cards_Add(Card_List *cards, Cards *m_card);//Adding the Cards
Cards * Cards_Pop(Card_List *cards); //Removing the Cards
Cards * Cards_Draw(Card_List *cards); //Removing the Cards
void Cards_Free(Card_List *cards);//Freeing the Cards

#endif