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
	struct gameState base;
	int numPlayers = 2;
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	int seed = 1000;
	int returnState;

	// initialize game
	initializeGame(numPlayers, k, seed, &base);

	// start testing
	printf("\n*************************\n* Bug 4 Unit Test Start *\n*************************\n");

	printf("\nTEST 1: Supply piles for sea hag, treasure map and mines are all empty\n");
	// set supply piles
	base.supplyCount[sea_hag] = 0;
	base.supplyCount[treasure_map] = 0;
	base.supplyCount[mine] = 0;

	returnState = isGameOver(&base); // play card

	// game should be over
	printf("Game ends with 3 supply piles empty\n");
	testAssert(1, returnState);

	printf("Unit Test Complete\n");
	return 0;
}
