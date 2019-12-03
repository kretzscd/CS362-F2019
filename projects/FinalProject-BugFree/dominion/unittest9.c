#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "interface.h"

int main()
{
	// setup variables
	struct gameState base, test;
	int numPlayers = 2;
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	int seed = 1000;
	int card, choice1, choice2, choice3, handPos, coin_bonus;
	int currentPlayer = 0;
        int nextPlayer = currentPlayer + 1;

	// initialize game
	initializeGame(numPlayers, k, seed, &base);
	base.hand[currentPlayer][0] = tribute; // set currentPlayer's position 0 to tribute
	base.deckCount[nextPlayer] = 10;      // set nextPlayer's deckCount to 10
	base.deck[nextPlayer][8] = estate;    // set the nexPlayer's top 3 cards to estate
        base.deck[nextPlayer][9] = estate;
        base.deck[nextPlayer][7] = estate; 
        base.numActions = 1;

	// start testing
        printf("\n");
	printf("*************************\n* Bug 9 Unit Test Start *\n*************************\n");

	printf("\nTEST 1: \n"); 
        
        memcpy(&test, &base, sizeof(struct gameState)); // refresh test state

	// set up other variables that will be needed to call case tribute when 
        // card effect is called.
	handPos = 0;
        choice1 = -1;  //choice1, choice2, and choice3 are not used in case tribute so set to -1
        choice2 = -1;  
        choice3 = -1;
	coin_bonus = 0; // ensure bonus is set to 0
	card = tribute;
      
	cardEffect(card, choice1, choice2, choice3, &test, handPos, &coin_bonus); // play card

        printf("Player's handCount should increase by 2\n");
       
	testAssert(base.handCount[currentPlayer] + 2, test.handCount[currentPlayer]);

        printf("Player's numActions should be 1\n");
       
	testAssert(base.numActions, test.numActions);

	printf("Unit Test Complete\n\n");

	return 0;
}
