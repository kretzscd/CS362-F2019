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
	base.hand[currentPlayer][0] = baron;  // set currentPlayer's position 0 to baron
	base.hand[currentPlayer][1] = estate; // set currentPlayer's position 1 to estate
        base.coins = 0;

	// start testing
        printf("\n");
	printf("*************************\n* Bug 8 Unit Test Start *\n*************************\n");

	printf("\nTEST 1: Case Baron\n"); 

	memcpy(&test, &base, sizeof(struct gameState)); // refresh test state
	// set up other variables that will be needed to call case baron when 
        // card effect is called.
	handPos = 0;
        choice1 = 1;    // choice1 is set to 1 because player wishes to discard estate
        choice2 = -1;   // set to -1 because choice2 is not used in case baron
        choice3 = -1;   // set to -1 because choice3 is not used in case baron
	coin_bonus = 0; // ensure coin_bonus is initialized to 0
	card = baron;   // set the card to play as baron


	cardEffect(card, choice1, choice2, choice3, &test, handPos, &coin_bonus); // play card

	// check test->coins
	printf("Verify that test-> coins in baron is 0\n");
       
	testAssert(0, test.coins);

	// should return 0
	printf("Verify that coin_bonus in baron increased +4\n");
	testAssert(4, coin_bonus);

	// initialize game to test embargo
	initializeGame(numPlayers, k, seed, &base);
	base.hand[currentPlayer][0] = embargo;  // set currentPlayer's position 0 to baron
        base.coins = 0;

	printf("\nTEST 2: Case Embargo\n"); 

	memcpy(&test, &base, sizeof(struct gameState)); // refresh test state
	// set up other variables that will be needed to call case baron when 
        // card effect is called.
	handPos = 0;
        choice1 = 4;    // choice1 is set to the supply # of the card the player wishes to embargo
        choice2 = -1;   // set to -1 because choice2 is not used in case embargo
        choice3 = -1;   // set to -1 because choice3 is not used in case embargo
	coin_bonus = 0; // ensure coin_bonus is initialized to 0
	card = embargo;   // set the card to play as embargo


	cardEffect(card, choice1, choice2, choice3, &test, handPos, &coin_bonus); // play card

	// check test->coins
	printf("Verify that test-> coins in embargo is 0\n");
       
	testAssert(0, test.coins);

	// should return 0
	printf("Verify that coin_bonus in embargo increased +2\n");
	testAssert(2, coin_bonus);

	// initialize game to test tribute
	initializeGame(numPlayers, k, seed, &base);
	base.hand[currentPlayer][0] = tribute;  // set currentPlayer's position 0 to tribute
        //set the nextPlayer's deckCount and the top 3 cards
        base.deckCount[nextPlayer] = 10;
        base.deck[nextPlayer][9] = silver;
        base.deck[nextPlayer][8] = copper;
        base.deck[nextPlayer][7] = copper;
        base.coins = 0;

	printf("\nTEST 3: Case Tribute\n"); 

	memcpy(&test, &base, sizeof(struct gameState)); // refresh test state
	// set up other variables that will be needed to call case tribute when 
        // card effect is called.
	handPos = 0;
        choice1 = -1;   // set to -1 because choice1 is not used in case tribute
        choice2 = -1;   // set to -1 because choice2 is not used in case tribute
        choice3 = -1;   // set to -1 because choice3 is not used in case tribute
	coin_bonus = 0; // ensure coin_bonus is initialized to 0
	card = tribute; // set the card to play as tribute

	cardEffect(card, choice1, choice2, choice3, &test, handPos, &coin_bonus); // play card

	// check test->coins
	printf("Verify that test-> coins in tribute is 0\n");
       
	testAssert(0, test.coins);

	// should return 0
	printf("Verify that coin_bonus in tribute increased total of +4\n");
	testAssert(4, coin_bonus);

	// initialize game to test minion
	initializeGame(numPlayers, k, seed, &base);
	base.hand[currentPlayer][0] = minion;  // set currentPlayer's position 0 to minion
        base.coins = 0;

	printf("\nTEST 4: Case Minion\n"); 

	memcpy(&test, &base, sizeof(struct gameState)); // refresh test state
	// set up other variables that will be needed to call case minion when 
        // card effect is called.
	handPos = 0;
        choice1 = 1;   // player is choosing to be awarded 2 bonus coins
        choice2 = -1;   // set to -1 because player does not want this choice
        choice3 = -1;   // set to -1 because choice3 is not used in case minion
	coin_bonus = 0; // ensure coin_bonus is initialized to 0
	card = minion; // set the card to play as minion

	cardEffect(card, choice1, choice2, choice3, &test, handPos, &coin_bonus); // play card

	// check test->coins
	printf("Verify that test-> coins in minion is 0\n");
       
	testAssert(0, test.coins);

	// should return 0
	printf("Verify that coin_bonus in minion increased +2\n");
	testAssert(2, coin_bonus);

	printf("Unit Test Complete\n\n");

	return 0;
}
