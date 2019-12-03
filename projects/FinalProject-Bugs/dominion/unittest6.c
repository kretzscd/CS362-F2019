/*
* File: unittest6.c
* Description: Unit test for the bug 6 (feast) bug of the CS362 final project
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


	// Set other variables
	
	// Province costs 8, so player should not be able to afford this card
	int card = feast; // Feast card will be played
	int choice1 = 11; // Card player wants to buy
	int choice2 = -1; // Not used in the feast case statement of card effect
	int choice3 = -1; // Not used in the feast case statement of card effect
	int handPos = -1; // Position of feast
	int bonus = 0; // Not used in the feast case statement of card effect
	
	// Set player's coins to 3
	stateOfGame.coins = 0;
	
	// Setup player's discard pile
	stateOfGame.discardCount[0] = province;
	stateOfGame.handCount[0] = 3;


	// Cards in discard for player one
	stateOfGame.discard[currentPlayer][0] = great_hall;
	stateOfGame.discard[currentPlayer][1] = feast;
	stateOfGame.discard[currentPlayer][2] = curse;

	// Initialize supply for card player wants to buy
	stateOfGame.supplyCount[mine] = 1;

	// Cards in hand for player one
	stateOfGame.hand[currentPlayer][0] = copper;
	stateOfGame.hand[currentPlayer][1] = feast;
	stateOfGame.hand[currentPlayer][2] = baron;


	printf("\n*************************\n* Bug 6 Unit Test Start *\n*************************\n");


	// Call cardEffect and pass in Feast card to target the effect of the Feast card
	cardEffect(card, choice1, choice2, choice3, &stateOfGame, handPos, &bonus);

	// Assertions
	// Player should have been able to pick up the province card that was in supply
	printf("\nTEST 1: The player should have been able to pick up the desired card from supply.\n");
	printf("NOTE: Due to bug 6 not being a valid bug, this test will pass as expected.\n");
	testAssert(0, stateOfGame.supplyCount[mine]);

	// Player should have 3 cards in hand due to picking up a card and discarding a card
	// A seperate bug exists that resets the player's hand, however discarding and
	// picking up a card maintains the player's hand count.  So even without the hand count bug,
	// the player would have the same amount of  cards as before playing the feast card.
	printf("\nTEST 2: The player should have three cards in have after discarding and gaining a card.\n");
	printf("NOTE: Due to seperate bug that resets the player's hand in the feast card effect, this test will always pass.\n");
	testAssert(3, stateOfGame.handCount[0]);
}