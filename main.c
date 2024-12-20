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
        "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};

const char Suites[SUITES][8] =
    {
        "Spades", "Diamond", "Hearts", "Clubs"};

const uint8_t Total_Cards = RANKS * SUITES; // 52 Cards

const char input;

// Functions

// Initialzing the game
void init_game(Gamestate *gameState);
// Starting the game
void Pre_Game(Gamestate *gameState);
// Initializing the rounds with the starting values, 2 cards for each player - player & CPU
void RoundInit(Gamestate *gameState);
// Resetting the cards back
void ResetDeck(Gamestate *gameState);
// The actual hit or stand
void HitOrStand(Gamestate *gameState);
// Outcome handler
bool outcome(Gamestate *gameState);
// A function that shows both player's and dealer's hands
int8_t showhands(Card_List *hand, bool showhand);
// Clearing the buffer
void empty_stdin(void);

// Additional Note, Wanted to include a delay func but since it works differently on Linux, decided not to

int main()
{
    // Gamestate Init
    Gamestate gameState;
    init_game(&gameState);

    // Randomizing new seeds each round
    srand(time(NULL));

    while (gameState.outcomes > -1)
    {
        Pre_Game(&gameState);
        if (outcome(&gameState))
            continue;
        RoundInit(&gameState);
        if (outcome(&gameState))
            continue;

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
    gameState->outcomes = TBD;
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

void Pre_Game(Gamestate *gameState)
{
    char answer[5];
    const char *yesans = "yes";
    const char *noans = "no";
    int input = 0;
    uint16_t bet = 0;

    // If broke
    if (gameState->cash < 10 && gameState->pot == 0)
    {
        gameState->outcomes = Broke;
        return;
    }

    printf("Hello, Welcome to blackjack! would you like to start playing? 'yes' or 'no'\n");
    while (true)
    {
        scanf("%4s", answer);
        empty_stdin();
        if (0 == strcmp(answer, yesans))
        {
            printf("Great! So you have $%u and the pot right now is $%u\n", gameState->cash, gameState->pot);

            do
            {
                printf("How much would you like to bet? in multiplications of 10's\n");
                input = scanf("%hu", &bet);
                empty_stdin();
                bet *= 10;
            } while (input == 0 || (bet < 10 && bet + gameState->pot <= 0) || bet > gameState->cash);

            gameState->cash -= bet;
            gameState->pot += bet;
            printf("Your bet is %hu\n\n", bet);
            gameState->outcomes = TBD;
            break;
        }

        else if (0 == strcmp(answer, noans))
        {
            gameState->outcomes = Quit;
            break;
        }
        // In the while block - Added else in order to not keep the scanning of the answer in the questioning of the starting game

        printf("Invalid answer, You need to type in exactly the words 'yes' or 'no' \n");
    }
}

void RoundInit(Gamestate *gameState)
{
    uint8_t DrawInd;
    uint8_t Players_val;
    uint8_t Dealers_val;
    ResetDeck(gameState);
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
    printf("Players Hand:\n");
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

    printf("Dealers Hand:\n");
    Dealers_val = showhands(&gameState->Dealer, false);

    // Showing the players hand

    if (Players_val == 21)
    {
        gameState->outcomes = Blackjack;
        return;
        // Round Loss Implementation prolly in enum
    }
    else if (Players_val >= 21 || Dealers_val == 21)
    {
        gameState->outcomes = Lose;
        return;
    }
}

void ResetDeck(Gamestate *gameState)
{
    while (gameState->Player.length > 0)
    {
        Cards_Add(&gameState->Deck, Cards_Draw(&gameState->Player, 0));
    }
    while (gameState->Dealer.length > 0)
    {
        Cards_Add(&gameState->Deck, Cards_Draw(&gameState->Dealer, 0));
    }
}

void HitOrStand(Gamestate *gameState)
{
    const char *hit = "hit";
    const char *stand = "stand";
    char input[6];
    uint8_t cardpick = 0;
    uint8_t PlayersValue = 0;
    uint8_t DealersValue = 0;

    do
    {
        printf("Would you like to hit or stand?\n");
        scanf("%5s", input);
        empty_stdin();
        if (0 == strcmp(input, hit))
        {
            if (gameState->Deck.length == 0)
            {
                printf("Couldn't Add The Cards To The Player: Deck Empty\n");
                return;
            }
            cardpick = rand() % gameState->Deck.length;
            Cards_Add(&gameState->Player, (Cards_Draw(&gameState->Deck, cardpick)));
            printf("Players Hand:\n");
            PlayersValue = showhands(&gameState->Player, 1);
            printf("Dealers Hand:\n");
            DealersValue = showhands(&gameState->Dealer, 0);
        }

        else if (0 == strcmp(input, stand))
        {
            break;
        }

    } while ((0 != strcmp(input, hit) || (0 != strcmp(input, stand))) && (PlayersValue < 21));

    if (PlayersValue > 21 || DealersValue == 21)
    {
        gameState->outcomes = Lose;
        return;
    }

    else if (PlayersValue == 21)
    {
        gameState->outcomes = Blackjack;
        return;
    }

    while (true)
    {
        printf("Players Hand:\n");
        PlayersValue = showhands(&gameState->Player, 1);
        printf("- Dealer Draws a card -\n\n");
        printf("Dealers Hand:\n\n");
        DealersValue = showhands(&gameState->Dealer, 1);
        
        printf("Press enter to continue\n");
        empty_stdin();
        
        if (DealersValue >= 17 || DealersValue > PlayersValue)
            break;
        cardpick = rand() % gameState->Deck.length;
        Cards_Add(&gameState->Dealer, (Cards_Draw(&gameState->Deck, cardpick)));
    }
    if (DealersValue > 21)
    {
        gameState->outcomes = Win;
        return;
    }
    if (DealersValue == 21)
    {
        gameState->outcomes = Lose;
        return;
    }
    else if (DealersValue > PlayersValue)
    {
        gameState->outcomes = Lose;
        return;
    }
    else if (DealersValue == PlayersValue)
    {
        gameState->outcomes = Tie;
        return;
    }
    gameState->outcomes = Win;
}

bool outcome(Gamestate *gameState)
{
    uint16_t winning = 0;

    switch (gameState->outcomes)
    {
    case Broke:

        printf("Broke, Run it to play again :D\n");
        break;

    case Quit:

        printf(":( Okay then, see you next time\n");
        break;

    case Blackjack:

        winning = gameState->pot * 2.5;
        gameState->cash += winning;
        gameState->pot = 0;
        printf("Congratulations! You hit a blackjack\n");
        printf("You've won $%u\n\n", winning);
        break;

    case Win:

        winning = gameState->pot * 2;
        gameState->cash += winning;
        gameState->pot = 0;
        printf("You've won $%u\n\n", winning);
        break;

    case Lose:

        printf("You've lost, Yikes\n\n");
        gameState->pot = 0;
        break;

    case Tie:

        printf("No worries, it's a tie, the round continues\n\n");
        break;

    case TBD: // Undetermined
        return 0;

    default:
        break;
    }
    printf("Round is over\n\n");
    return 1;
}

int8_t showhands(Card_List *hand, bool showhand)
{

    uint8_t total = 0;
    uint8_t aces = 0;
    uint8_t count = 0;

    Cards *curr = hand->head;
    while (curr != NULL)
    {

        uint8_t rank = curr->data >> 4;
        uint8_t suitb = curr->data << 4;
        uint8_t suit = 0;
        uint8_t value = rank + 1;

        aces += (value == 1);

        total += value;

        while (suitb > 16)
        {
            suitb /= 2;
            suit++;
        }

        if (showhand || count == 0)
        {
            printf(" - %s of %s\n", Ranks[rank], Suites[suit]);
        }
        else
        {
            printf(" - ? of ?\n");
        }
        curr = curr->next;
        count++;
    }
    while (total < 13 && aces > 0)
    {
        total += 9;
        aces--;
    }
    printf("\n");
    if (showhand)
    {
        printf("\t Total is[%hu]\n\n", total);
    }
    else
    {
        printf("\t Total: ???\n");
    }

    return total;
}

void empty_stdin(void)
{
    int c = getchar();
    while (c != '\n' && c != EOF)
        c = getchar();
}