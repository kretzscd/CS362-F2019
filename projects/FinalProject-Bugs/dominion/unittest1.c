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
	int card, choice1, choice2, choice3, handPos, coin_bonus, returnState, inDiscard;
	int player = 0;

	// initialize game
	initializeGame(numPlayers, k, seed, &base);
	base.hand[player][0] = mine; // load hand with a mine, copper, silver, gold and an estate
	base.hand[player][1] = copper;
	base.hand[player][2] = silver;
	base.hand[player][3] = gold;
	base.hand[player][4] = estate;

	// adjust variables that won't change
	choice3 = 0;
	coin_bonus = 0;
	card = mine;

	// start testing
	printf("************************\n* Bug 1 Unit Test Start *\n************************\n");

	printf("\nTEST 1: Trash a copper to gain a silver\n");
	memcpy(&test, &base, sizeof(struct gameState)); // refresh test state
	choice1 = 1; // hand position of copper to trash
	choice2 = silver; // card to get in exchange
	handPos = 0; // hand position of mine
	inDiscard = 0; // mine not in discard yet
	returnState = cardEffect(card, choice1, choice2, choice3, &test, handPos, &coin_bonus); // play card

	// should return 0
	printf("Valid choice, should return 0\n");
	testAssert(0, returnState);

	// hand contains 1 fewer
	printf("Hand size decreases by 1\n");
	testAssert(base.handCount[player] - 1, test.handCount[player]);

	// trash card should be discarded
	printf("Discard count increases by 1\n");
	testAssert(base.discardCount[player] + 1, test.discardCount[player]);

	// check discarded cards for mine
	printf("Discard pile should contain mine card\n");
	//check through discard pile for mine card
	int i;
	for (i = 0; i < test.discardCount[player]; i++)
	{
		// check if its a mine card
		if (test.discard[player][i] == mine)
		{
			inDiscard = 1;
		}

		printf("%d: %d\n", i, test.discard[player][i]);
	}
	testAssert(1, inDiscard);

	printf("Unit Test Complete\n");

	return 0;
}
