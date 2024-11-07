#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// Including all of the headers
#include "cards_funcs.h"
#include "cards_l.h"
#include "gamestate.h"

// Defining ranks and suites
#define RANKS (13)
#define SUITES (4)

const char Ranks[RANKS][6] =
    {
        "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Prince", "Queen", "King"};

const char Suites[SUITES][8] =
    {
        "Spades", "Diamonds", "Hearts", "Clubs"};

const uint8_t Total_Cards = RANKS * SUITES; // 52 Cards

// Functions

// Initialzing the game
void init_game(Gamestate *gameState);
// Starting the game
void New_Game(Gamestate *gameState);
// Initializing the rounds with the starting values, 2 cards for each player - player & CPU
void RoundInit(Gamestate *gameState);
// The actual hit or stand
void HitOrStand(Gamestate *gameState);
// Outcome handler
bool outcome(Gamestate *gameState);
// A function that shows both player's and dealer's hands
int8_t showhands(Card_List *hand, bool showhand);

int main()
{
    // Gamestate Init
    Gamestate gameState;
    init_game(&gameState);

    // Randomizing new seeds each round
    srand(time(NULL));

    while (gameState.outcomes > -1)
    {
        New_Game(&gameState);
        if (outcome(&gameState))
            continue;
        RoundInit(&gameState);

        HitOrStand(&gameState);
        outcome(&gameState);
    }

    // Freeing all of the dynamic allocated memory
    Cards_Free(&gameState.Deck);
    Cards_Free(&gameState.Player);
    Cards_Free(&gameState.Dealer);

    return 0;
}

// Initializing all the members of the struct
void init_game(Gamestate *gameState)
{

    gameState->cash = 1000;
    gameState->pot = 0;

    Cards_Init(&gameState->Deck);
    Cards_Init(&gameState->Player);
    Cards_Init(&gameState->Dealer);

    // Cards in hand
    Cards *Hand_Card = NULL;

    for (int RanksIND = 0; RanksIND < RANKS; RanksIND++)
    {
        for (int SuitIND = 0; SuitIND < SUITES; SuitIND++)
        {
            Hand_Card = malloc(sizeof(Cards));
            Hand_Card->data = RanksIND;
            Hand_Card->data <<= 4;
            Hand_Card->data |= (1 << SuitIND);
            Cards_Add(&(gameState->Deck), Hand_Card);
        }
    }
}

// New_Game - Done

void New_Game(Gamestate *gameState)
{
    char answer;
    int input = 0;
    uint16_t bet = 0;

    // If broke
    if (gameState->cash < 10 && gameState->pot == 0)
    {
        printf("Broke, Run it to play again :D\n");
        gameState->outcomes = Outcome_Broke;
        return;
    }

    printf("Hello, Welcome to blackjack! would you like to start playing? y/n\n");
    input = scanf("%c", &answer);

    if (answer == 'n' || answer == 'N')
    {
        printf(":( Okay then, see you next time\n");
        gameState->outcomes = Outcome_Quit;
        return;
    }

    else

    {
        while (input == 0 || (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n'))
        {
            printf("Invalid answer, Please try again\n");
            scanf("%c", (&answer));
        }
    }

    printf("Great! So you have $%u and the pot right now is $%u\n", gameState->cash, gameState->pot);

    printf("How much would you like to bet? in multiplications of 10's\n");
    input = scanf("%hu", &bet);
    bet *= 10;

    while (bet < 10 && bet + gameState->pot < 0)
    {
        printf("Not the right amount, Please insert the value again\n");
        input = scanf("%hu", &bet);
        bet *= 10;
    }

    gameState->cash -= bet;
    gameState->pot += bet;

    gameState->outcomes = Outcome_TBD;
}

void RoundInit(Gamestate *gameState)
{
    uint8_t DrawInd;
    uint8_t Players_val;
    uint8_t Dealers_val;
    // Drawing cards for the players
    for (int i = 0; i < 2; i++)
    {
        if (gameState->Deck.length == 0)
        {
            printf("Couldn't Add The Cards To The Player: Deck Empty\n");
            return;
        }
        DrawInd = rand() % gameState->Deck.length;
        Cards_Add(&gameState->Player, (Cards_Draw(&gameState->Deck, DrawInd)));

    }
    Players_val = showhands(&gameState->Player, true);

    // Drawing cards for the CPU
    for (int i = 0; i < 2; i++)
    {
        if (gameState->Deck.length == 0)
        {
            printf("Couldn't Add The Cards To The Dealer: Deck Empty\n");
            return;
        }
        DrawInd = rand() % gameState->Deck.length;
        Cards_Add(&gameState->Dealer, (Cards_Draw(&gameState->Deck, DrawInd)));
    }

    Dealers_val = showhands(&gameState->Dealer, false);

    // Showing the players hand

    if (Players_val == 21)
    {
        printf("Congratulations!, Blackjack!\n");
        gameState->outcomes = Outcome_Blackjack;
        return;
        // Round Loss Implementation prolly in enum
    }
    else if (Players_val > 21)
    {
        printf("You've lost\n");
        gameState->outcomes = Outcome_Lose;
        return;
    }
}

void HitOrStand(Gamestate *gameState)
{
    const char hit = 'h';
    const char stand = 's';
    uint8_t cardpick = 0;
    uint8_t PlayersValue = 0;
    uint8_t DealersValue = 0;

    while (true)
    {
        printf("Would you like to hit or stand?\n");
        getchar();

        if (getchar() == hit)
        {
            if (gameState->Deck.length == 0)
            {
                printf("Couldn't Add The Cards To The Player: Deck Empty\n");
                return;
            }
            printf("Hit\n");
            cardpick = rand() % gameState->Deck.length;
            Cards_Add(&gameState->Deck, (Cards_Draw(&gameState->Player, cardpick)));
            PlayersValue = showhands(&gameState->Player, 1);

            if (PlayersValue > 21)
            {
                gameState->outcomes = Outcome_Lose;
                return;
            }

            if (PlayersValue == 21)
            {
                gameState->outcomes = Outcome_Blackjack;
                return;
            }

            DealersValue = showhands(&gameState->Dealer, 0);
        }
        else if (getchar() == stand)
        {
            break;
        }
        else
        {
            printf("Invalid, Please try putting a valid input\n");
        }
    }

    while (true)
    {
        PlayersValue = showhands(&gameState->Player, 1);
        printf("Dealer Draws a card\n");
        DealersValue = showhands(&gameState->Dealer, 1);
        if (DealersValue >= 17 || DealersValue > PlayersValue)
            break;
        cardpick = rand() % gameState->Deck.length;
        Cards_Add(&gameState->Deck, (Cards_Draw(&gameState->Dealer, cardpick)));
    }
    if (DealersValue > 21)
    {
        printf("Dealer lost and you've won!\n");
        gameState->outcomes = Outcome_Win;
        return;
    }
    else if (DealersValue > PlayersValue)
    {
        printf("You've lost\n");
        gameState->outcomes = Outcome_Lose;
        return;
    }
    else if (DealersValue == PlayersValue)
    {
        printf("Tie game\n");
        gameState->outcomes = Outcome_Tie;
        return;
    }
    gameState->outcomes = Outcome_Win;
}

bool outcome(Gamestate *gameState)
{
    uint16_t winning = 0;

    switch (gameState->outcomes)
    {
    case Outcome_Broke:

    case Outcome_Quit:

    case Outcome_Blackjack:
        winning = gameState->pot * 2.5;
        gameState->cash += winning;
        gameState->pot = 0;
        printf("You've won $%u", winning);
        break;
    case Outcome_Win:
        winning = gameState->pot * 2;
        gameState->cash += winning;
        gameState->pot = 0;
        printf("You've won $%u", winning);
        break;
    case Outcome_Lose:
        printf("You've lost, Yikes\n");
        gameState->pot = 0;
        break;
    case Outcome_Tie:
        printf("No worries, it's a tie, the round continues\n");
        break;
    case Outcome_TBD: // Undetermined
        return 0;

    default:
        break;
    }
    printf("Round is over\n");
    return 1;
}

int8_t showhands(Card_List *hand, bool showhand)
{

    uint8_t total = 0;
    uint8_t aces = 0;

    Cards *curr = hand->head;
    while (curr != NULL)
    {

        uint8_t rank = curr->data >> 4;
        uint8_t suitb = curr->data << 4;
        uint8_t suit = 0;
        uint8_t value = rank + 1;

        if (total >= 10)
        {
            aces = (value == 1);
        }
        else
        {
            while (total < 13 && aces > 0)
            {
                total += 9;
                aces--;
            }
        }
        total += value;

        while (suitb > 16)
        {
            suitb /= 2;
            suit++;
        }

        if (showhand)
        {
            printf("%s of %s\n", Ranks[rank], Suites[suit]);
        }
        else
        {
            printf("? of ?\n");
        }
        curr = curr->next;
    }
    if (showhand)
    {
        printf("Total is[%hu]\n", total);
    }
    else
    {
        printf("Total: ???\n");
    }

    return total;
}