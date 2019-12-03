/*
* File: unittest5.c
* Description: Unit test for the bug 5 (scoreFor) bug of the CS362 final project
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

	// Set card array
	int k[10] = { estate, baron, feast, curse, duchy, remodel, smithy,
	village, province, great_hall };

	// Declare game state
	struct gameState stateOfGame;


	// Set general variables
	int seed = 456;
	memset(&stateOfGame, 23, sizeof(stateOfGame)); // Set the game state
	initializeGame(2, k, seed, &stateOfGame); // Initialize a new game

	// Player one will be current player
	int currentPlayer = 0;

	// Variable to store output score
	int scoreForOutput = 0;
	int calculatedScore = 0;

	// Set state of game for player one
	stateOfGame.handCount[0] = 3;
	stateOfGame.deckCount[0] = 4;
	stateOfGame.discardCount[0] = 3;

	// Cards in hand
	// Gives 2 points totals
	stateOfGame.hand[currentPlayer][0] = estate; // +1 point
	stateOfGame.hand[currentPlayer][1] = baron; // 0 points
	stateOfGame.hand[currentPlayer][2] = great_hall; // +1 points

	// Cards in deck
	// Gives 12 points total 
	stateOfGame.deck[currentPlayer][0] = estate; // +1 points
	stateOfGame.deck[currentPlayer][1] = province; // +6 points
	stateOfGame.deck[currentPlayer][2] = curse; // -1 points
	stateOfGame.deck[currentPlayer][3] = province; // +6 points


	// Cards in discard
	// Gives 4 points total 
	stateOfGame.discard[currentPlayer][0] = baron; // 0 points
	stateOfGame.discard[currentPlayer][1] = great_hall; // +1 points
	stateOfGame.discard[currentPlayer][2] = duchy; // +3 points


	// Set hand calculated score
	calculatedScore = 18;


	printf("\n*************************\n* Bug 5 Unit Test Start *\n*************************\n");

	// Call scoreFor
	scoreForOutput = scoreFor(0, &stateOfGame);

	// Asserts

	/* Fourth card in hand, province will not be counted due to bug
	 * so final total will be off by six points since handCount is 4
	 * and discard count is 3
	*/
	printf("\nTEST 1: The player's hand calculated score should match their scoreFor score\n");
	testAssert(calculatedScore, scoreForOutput);

}