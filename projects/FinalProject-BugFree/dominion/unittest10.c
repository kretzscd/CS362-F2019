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
	int card, choice1, choice2, choice3, handPos, coin_bonus, returnState, numEstates;
	int currentPlayer = 0;

	// initialize game
	initializeGame(numPlayers, k, seed, &base);
	base.hand[currentPlayer][0] = ambassador; // set currentPlayer's position 0 to ambassador
	base.hand[currentPlayer][1] = estate; // set currentPlayer's position 1 to estate
	base.hand[currentPlayer][2] = estate; // set currentPlayer's position 2 to estate
        base.hand[currentPlayer][3] = copper; // set the last 2 cards to coppers
        base.hand[currentPlayer][4] = copper;
        base.supplyCount[estate] = 8;

	// start testing
        printf("\n");
	printf("**************************\n* Bug 10 Unit Test Start *\n**************************\n");

	printf("\nTEST 1: Player chooses to return 2 estate cards to supply\n"); 

	memcpy(&test, &base, sizeof(struct gameState)); // refresh test state
	// set up other variables that will be needed to call case ambassador when 
        // card effect is called.
        numEstates = 0;
	handPos = 0;
        choice1 = 1;
        choice2 = 2;
        choice3 = -1;
	coin_bonus = 0;
	card = ambassador;
	// check number of estates in currentPlayer's hand
	printf("Verify player's hand contains 2 estate cards\n");
	//check through player's hand for estate cards
	int i;
	for (i = 0; i < test.handCount[currentPlayer]; i++)
	{
		// check if card is an estate
		if (test.hand[currentPlayer][i] == estate)
		{
			numEstates += 1;
		}
	}
       
	testAssert(2, numEstates);
	returnState = cardEffect(card, choice1, choice2, choice3, &test, handPos, &coin_bonus); // play card

	// should return 0
	printf("Function counts estates in hand, should count 2 and return 0\n");
	testAssert(0, returnState);

	// check number of estates returned to supply is 2, -1 given to nextPlayer
	printf("Verify number of estates in supply at end of play is correct\n");
       
	testAssert(base.supplyCount[estate] + 1, test.supplyCount[estate]);

	printf("Unit Test Complete\n\n");

	return 0;
}
