#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//Including all of the headers
#include "cards_funcs.h"
#include "cards_l.h"
#include "gamestate.h"

//Defining ranks and suites
#define RANKS (13)
#define SUITES (4)

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
    int bet = 0;

    //If broke
     if (gameState -> cash < 10 && gameState->pot == 0){
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
    scanf("%d", &bet);
    bet*=10;
    
    while(input == 0 || bet > gameState->pot || bet + gameState->pot < 0)
    {
    printf("Not the right amount, Please insert the value again\n");
    input = scanf("%d", &bet);
    bet *= 10;
    }

    gameState->cash -= bet;
    gameState->pot += bet;
 }



void RoundInit(Gamestate* gameState)
{
    uint8_t CardsDraw;
    uint8_t Players_val;
//Drawing cards for the player
    for(int i = 0; i < 2; i++)
    {
        CardsDraw = rand() % gameState->Deck.length;
        Cards_Add(&gameState->Deck, 0), Cards_Draw(&gameState->Player.length);
    }

    
    for(int i = 0; i < 2; i++){
        CardsDraw = rand() % gameState->Deck.length;
        Cards_Add(&gameState->Deck, 0), Cards_Draw(&gameState->Dealer.length);
    }
    


}
//Drawing cards for the CPU



/* TODO: wtf is going on switch (outcome)
{

    case OutcomeWin:
    if
}*/

    
    
    
    
    
    
    
    
    //Aces Sumup
    /*if (total >= 10)
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
}*/