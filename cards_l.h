#ifndef CARDS_L_H
#define CARDS_L_H

#include <stdio.h>
#include <stdint.h>


//The Data Of The Card Itself
typedef struct Card{
    uint8_t data;
    struct Card * next;
}Cards;

//List of Cards
typedef struct Cards_List{
    Cards *head;
    size_t length;
}Card_List;


#endif