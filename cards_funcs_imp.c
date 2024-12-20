#ifndef CARDS_FUNC_IMPL
#define CARDS_FUNC_IMPL

#include "cards_funcs.h"

void Cards_Init(Card_List *cards){
    cards->length = 0;
    cards->head = NULL;
}


//Completed the addition function
void Cards_Add(Card_List *cards, Cards *m_card){
    m_card->next = cards->head;
    cards->head = m_card;
    cards->length++;
}



Cards * Cards_Draw(Card_List *cards, uint8_t CardsElem)
{
if(cards == NULL || cards->head == NULL){
    printf("Drawing Card Failed: Null card list\n");
    return NULL;
}

Cards *target = NULL;
if(CardsElem == 0){
    target = cards->head;
    cards->head = target->next;
}
else{
    Cards *prev = cards->head;        
    for(uint8_t i = 1; i < CardsElem; i++)
        {
            prev = prev->next;
        }
    target = prev->next;    
    prev->next = target->next;
}
    target->next = NULL;
    cards->length--; 
    return target;
}


void Cards_Free(Card_List *cards){
    while(cards->length > 0){
        free(Cards_Draw(cards, 0));
    }
}



#endif