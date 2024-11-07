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

//Drawing a card from the list of cards and after drawing it, setting the value to NULL 
Cards * Cards_Pop(Card_List *cards){
    //if(cards->length == 0) return NULL;{
        Cards *off = cards->head;
        cards->length--;
        cards->head = off->next;
        off->next = NULL;
        return off;
}
//}


Cards * Cards_Draw(Card_List *cards, uint8_t CardsElem)
{
if(cards == NULL || cards->head == NULL){
    printf("Drawing Card Failed: Null card list\n");
    return NULL;
}
  /*  pull out a card
    2. find the previous card (if exists)
    1. find the card I want to pull
    3. attach the previous to the next one in the card list 
    5. detach the connection to the card
    4. decrease the length 
    */
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
        free(Cards_Pop(cards));
    }
}



#endif