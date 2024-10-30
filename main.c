#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//Including all of the headers
#include "cards_funcs_imp.c"
#include "cards_funcs.h"
#include "cards_l.h"
#include "gamestate.h"

//Defining ranks and suites
#define RANKS (13)
#define SUITES (4)
#define Move(src, dst, indx) Cards_Add(dst, Card_Draw(src, indx))

const char Ranks[RANKS][6] = 
{
    "Ace","2","3","4","5","6","7","8","9","10","Prince","Queen","King"
};

const char Suites[SUITES][8] = 
{
    "Spades","Diamonds","Hearts","Clubs"
};



const uint8_t Total_Cards = RANKS * SUITES; //52 Cards

//Functions

//Initialzing the game
Gamestate init_game(void);

//Starting the game 
void GameStart(Gamestate *gameState);




int main() 
{
    //Gamestate Init
    Gamestate gameState;
    gameState = init_game();
    
    //Randomizing new seeds each round
    srand(time(NULL));
    

    //Freeing all of the dynamic allocated memory
    Cards_Free(&gameState.Deck);
    Cards_Free(&gameState.Player);
    Cards_Free(&gameState.Dealer);
    
}



//Initializing all the members of the struct 
Gamestate init_game(void)
{
    Gamestate gameState;

    gameState.cash = 1000;
    gameState.pot = 0;

    Cards_Init(&gameState.Deck);
    Cards_Init(&gameState.Player);
    Cards_Init(&gameState.Dealer);


//Cards in hand
Cards *Hand_Card = NULL;

for (int RanksIND = 0; RanksIND < RANKS; RanksIND++)
    {
        for (int SuitIND = 0; SuitIND< SUITES; SuitIND++)
            {
            Hand_Card = malloc(sizeof(Cards));
            Hand_Card->data = RanksIND;
            Hand_Card->data <<=4;
            Hand_Card->data |= (1 << SuitIND);
            }
    
    Cards_Add(&gameState.Deck, Hand_Card);
    }
    return gameState;
}


//New_Game - Done

void New_Game(Gamestate *gameState){
    char answer;
    int input = 0;
    uint8_t bet = 0;

    //If broke
     if (gameState -> cash < 10){
        printf("Broke, Run it to play again :D\n");
        return;
    }

    printf("Hello, Welcome to blackjack! would you like to start playing? y/n\n");
    
    if(scanf("%c", &answer) == 'y' || (scanf("%c", &answer)) == 'Y'){
        printf("Great! Let's start playing!\n");
    }
    if(scanf("%c", &answer) == 'n' || scanf("%c", &answer) == 'N'){
        printf(":( Okay then, see you next time\n");
        return;
    }
    while(scanf("%c", &answer) != 'n' || scanf("%c", &answer) !=  'N' || scanf("%c", &answer) !=  'y' || scanf("%c", &answer) !=  'Y')
    {
        printf("Invalid answer, Please try again\n");
        scanf("%c", (&answer));    
    }

    printf("Great! So you have $%u and the pot right now is $%u\n", gameState -> cash, gameState -> pot);
    
    printf("How much would you like to bet? in multiplications of 10's\n");
    scanf("%hu", &bet);
    bet*=10;
    
    while(input == 0 || bet > gameState->pot || bet + gameState->pot < 0)
    {
    printf("Not the right amount, Please insert the value again\n");
    input = scanf("%hu", &bet);
    bet *= 10;
    }

    gameState->cash -= bet;
    gameState->pot += bet;
 }


void RoundStart(Gamestate *gameState){
    uint8_t drawCard;
    uint8_t  Value;

    //Player hand draw
    for(int i = 0; i < 2; i++){
        drawCard = rand() % gameState->Deck.length;
        Move(&gameState->Deck, &gameState->Player, 0);
    }

    //Dealer hand draw
    for(int i = 0; i < 2; i++){
        drawCard = rand() % gameState->Deck.length;
        Move(&gameState->Deck, &gameState->Dealer, 0);
    }
}

void HitOrStand(Gamestate* gameState)
{
    uint8_t CardPick = 0;
    uint8_t Player_Hand = 0;
    uint8_t Dealer_Hand = 0;

    while(true){
        int a = fgetc(stdin);
        scanf("Would you like to Hit or Stand?\n", a);
        {
        if(a == 10){
            CardPick = rand() % gameState->Deck.length;
            //So you've chose to hit huh? SO HIT IT IS, Enter for HIT
            printf("HIT\n");
            Move(&gameState -> Deck, &gameState -> Player, CardPick);
            }
        }
    }
}




/* TODO: wtf is going on switch (outcome)
{

    case OutcomeWin:
    if
}*/





uint8_t hand_show(Card_List *hand, bool show){
    uint8_t total = 0;
    uint8_t aces = 0;
    uint8_t count = 0;

    Cards *curr = hand->head;

    while (curr != NULL){
        uint8_t rank = curr->data >> 4;
        uint8_t suitebyte = (uint8_t)curr->data << 4;
        uint8_t suite = 0;
        uint8_t value = rank+1;
    
    
    while(suitebyte > 16)
        {
            suitebyte /= 2;
            suite++;
        }

    if(show || count == 0){
         printf("%s of %s (%2d)\n", RANKS[rank], SUITES[suite], value);
    }
    else //TODO: check why the fuck it doesn't work
    {
        printf("????\n");
    }


    //Aces Sumup
    if (total >= 10)
    {
        aces = (value == 1);
    }
    else
        {
            while(total < 13 && aces > 0)
            {
            total+=9;
            aces--;
            }
        }
        total+=value;
    }
}