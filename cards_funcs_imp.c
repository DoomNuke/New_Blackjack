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


Cards * Card_Draw(Card_List *cards)
{

Cards *curr = NULL;        
Cards *drawn = cards->head;
    
        for(int i = 0; i < cards->length; i++)
        {
            curr = drawn;
            curr->next = drawn;
            drawn->next = curr->next;
            cards->head = drawn;   
            cards->length--;
        }
    
    drawn->next = NULL;
    return drawn;
}


void Cards_Free(Card_List *cards){
    while(cards->length > 0){
        free(Cards_Pop(cards));
    }
}



#endif