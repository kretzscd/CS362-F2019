<<<<<<< HEAD
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
	int card, choice1, choice2, choice3, handPos, coin_bonus, returnState, inDiscard, inHand;
	int player = 0;

	// initialize game
	initializeGame(numPlayers, k, seed, &base);
	base.hand[player][0] = mine; // load hand with a mine and copper
	base.hand[player][1] = copper;
	base.handCount[player] = 2;

	// adjust variables that won't change
	choice3 = 0;
	coin_bonus = 0;
	card = mine;

	// start testing
	printf("\n*************************\n* Bug 1 Unit Test Start *\n*************************\n");

	printf("\nTEST 1: Trash a copper to gain a silver\n");
	memcpy(&test, &base, sizeof(struct gameState)); // refresh test state
	choice1 = 1; // hand position of copper to trash
	choice2 = silver; // card to get in exchange
	handPos = 0; // hand position of mine
	inDiscard = 0; // discard pile doesn not contain copper
	inHand = 0;
	returnState = cardEffect(card, choice1, choice2, choice3, &test, handPos, &coin_bonus); // play card

	// should return 0
	printf("Valid choice, should return 0\n");
	testAssert(0, returnState);

	// check discarded cards for trashed card
	printf("Discard pile does not contain trashed copper\n");
	//check through discard pile for copper card
	int i;
	for (i = 0; i < test.discardCount[player]; i++)
	{
		// check if its a copper card
		if (test.discard[player][i] == copper)
		{
			inDiscard = 1;
		}
	}
	testAssert(0, inDiscard);

	// check hand for trashed card
	printf("Hand does not contain trashed copper\n");
	//check through hand for copper card
	for (i = 0; i < test.handCount[player]; i++)
	{
		// check if its a copper card
		if (test.hand[player][i] == copper)
		{
			inHand = 1;
		}
	}
	testAssert(0, inHand);

	printf("Unit Test Complete\n");

	return 0;
}
||||||| merged common ancestors
=======
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
	int card, choice1, choice2, choice3, handPos, coin_bonus, returnState, inDiscard, inHand;
	int player = 0;

	// initialize game
	initializeGame(numPlayers, k, seed, &base);
	base.hand[player][0] = mine; // load hand with a mine and copper
	base.hand[player][1] = copper;
	base.handCount[player] = 2;

	// adjust variables that won't change
	choice3 = 0;
	coin_bonus = 0;
	card = mine;

	// start testing
	printf("\n*************************\n* Bug 1 Unit Test Start *\n*************************\n");

	printf("\nTEST 1: Trash a copper to gain a silver\n");
	memcpy(&test, &base, sizeof(struct gameState)); // refresh test state
	choice1 = 1; // hand position of copper to trash
	choice2 = silver; // card to get in exchange
	handPos = 0; // hand position of mine
	inDiscard = 0; // discard pile doesn not contain copper
	inHand = 1; // hand contains copper
	returnState = cardEffect(card, choice1, choice2, choice3, &test, handPos, &coin_bonus); // play card

	// should return 0
	printf("Valid choice, should return 0\n");
	testAssert(0, returnState);

	// check discarded cards for trashed card
	printf("Discard pile does not contain trashed copper\n");
	//check through discard pile for copper card
	int i;
	for (i = 0; i < test.discardCount[player]; i++)
	{
		// check if its a copper card
		if (test.discard[player][i] == copper)
		{
			inDiscard = 1;
		}
	}
	testAssert(0, inDiscard);

	// check hand for trashed card
	printf("Hand does not contain trashed copper\n");
	//check through hand for copper card
	for (i = 0; i < test.handCount[player]; i++)
	{
		// check if its a copper card
		if (test.hand[player][i] == copper)
		{
			inHand = 1;
		}
	}
	testAssert(0, inHand);

	printf("Unit Test Complete\n");

	return 0;
}
>>>>>>> kretzscd-finalproject-bugs
