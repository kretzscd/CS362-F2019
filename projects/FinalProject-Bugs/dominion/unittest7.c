/*
* File: unittest7.c
* Description: Unit test for the bug 7 (tribute) bug of the CS362 final project
* Author: Lee Rice
* Last Modified: 12/02/2019
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "interface.h"

int main() {

	// Initialize variables
	// Set card array
	int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy,
	village, baron, great_hall };

	// Declare game state
	struct gameState stateOfGame;

	// Set general variables
	// memset and initializeGame function setup adapted from
	// the assignment prompt
	int seed = 456;
	memset(&stateOfGame, 23, sizeof(stateOfGame)); // Set the game state
	initializeGame(2, k, seed, &stateOfGame); // Initialize a new game

	// Set variables

	stateOfGame.handCount[0] = 4;
	stateOfGame.deckCount[1] = 3;

	stateOfGame.numActions = 0;
	stateOfGame.coins = 0;

	// Set our player positions
	int currentPlayer = 0;
	int nextPlayer = 1;

	stateOfGame.numActions = 0;
	stateOfGame.coins = 0;
	
	int card = tribute; // Tribute card will be played
	int choice1 = -1; // Not used in the tribute case statement of card effect
	int choice2 = -1; // Not used in the tribute case statement of card effect
	int choice3 = -1;  // Not used in the tribute case statement of card effect
	int handPos = -1;  // Not used in the tribute case statement of card effect
	int bonus = 0;  // Not used in the tribute case statement of card effect


	// Set player hands
	stateOfGame.hand[currentPlayer][0] = tribute;
	stateOfGame.hand[currentPlayer][1] = ambassador;
	stateOfGame.hand[currentPlayer][2] = feast;
	stateOfGame.hand[currentPlayer][3] = gardens;

	stateOfGame.deck[nextPlayer][0] = feast;
	stateOfGame.deck[nextPlayer][1] = silver;



	printf("\n*************************\n* Bug 7 Unit Test Start *\n*************************\n");

	// Call cardEffect and pass in Tribute card to target the card's effect
	cardEffect(card, choice1, choice2, choice3, &stateOfGame, handPos, &bonus);

	// Assertions
	// Player should gain 2 coins since the next player has two treasure cards to reveal
	printf("\nTEST 1: The player should gain zero coins\n");
	printf("NOTE: Due to a seperate bug where the player in tribute, the player gains two coins\n");
	printf("This gain is not accounted for in the below test to a seperate bug where the player in tribute, the player gains two coins\n");
	printf("This test will pass once the bug is fixed in the final bug-free verion of dominion.c\n");
	testAssert(0, stateOfGame.coins);

	// Actions should not increase
	// However,due to a bug where the tribute function runs three times,
	// the player gets two additional actions
	printf("\nTEST 2: The player should gain only 2 additional actions due to the revealed cards being action cards\n");
	testAssert(2, stateOfGame.numActions);

	printf("\nTEST 3: The player should not gain additional cards\n");
	printf("NOTE: Due to a seperate bug where the player does not discard the played tribute card,\n");
	printf("the player will have the same amount of cards they started with.\n");
	printf("Unless additional cards are picked up through the tribute card effect.\n");
	testAssert(4, stateOfGame.handCount[0]);

	
}