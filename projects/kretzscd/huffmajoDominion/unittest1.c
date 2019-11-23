/* -----------------------------------------------------------------------
 * Unit testing for the baron_action refactored function 
 * Include the following lines in your makefile:
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o unittest1 -g  unittest1.c interface.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */
#include "interface.h"
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"

#define TESTCARD "baronEffect"

//declare the assert function
void assert(int expression, int *passed, int* failed);


int main() {
    int seed = 1000;
    int numPlayer = 2;
    int i, p;
    int bonus = 0;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int passed = 0;
    int failed = 0;
    char name[] = "council_room";
    //this for-loop can run as many players as you wish to test. For this card
    //there is only going to be one player tested. 
    for (p = 0; p < numPlayer - 1; p++)
    {
	printf ("BEGIN TESTING baronEffect() FOR PLAYER %d\n\n", p + 1);

	// set up the game state
        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        initializeGame(numPlayer, k, seed, &G); // initialize a new game

        // set up for TEST 1 -- choice1 = 1, bonus +4 
        G.coins = 0;
	G.hand[p][5] = 15;
	G.hand[p][6] = 1;
	G.handCount[p] = 7;
	G.playedCardCount = 0;
	bonus = 0;

        // Player wishes to discard estate, and they have estate in hand
        // checks to see if bonus of 4 correctly assigned
 	printf("*** TEST 1 ***\n");

	// call baronEffect
	baronEffect(&G, 1, 0);
        printf("Player %d plays baron card and discards estate.\n\n", p+1);
	printf("Expect: bonus coins = 4\n");
	printf("Actual: bonus coins = %d\n", bonus);
	assert(bonus == 4, &passed, &failed);
	
	// coins should not be changed because they are overwritten
        // in the updateCoins function and then player does not
        // get bonus
	printf("Expect: G.coins = 0\n");
	printf("Actual: G.coins = %d\n", G.coins);
	assert(G.coins == 0, &passed, &failed);
	
	// checking handCount to make sure baron was discarded
	printf("Expect: G.handCount[p] = 5\n");
	printf("Actual: G.handCount[p] = %d\n", G.handCount[p]);
	assert(G.handCount[p] == 5, &passed, &failed);
	
	// checking to make sure playedCardCount incremented since baron
        // was discarded. 
	printf("Expect: G.playedCardCount = 1\n");
	printf("Actual: G.playedCardCount = %d\n", G.playedCardCount);
	assert(G.playedCardCount == 1, &passed, &failed);

	// Set up state for TEST 2
	//it tests the inner conditional if the player wanted to discard an
        //estate but does not have one. 
	G.playedCardCount = 0;
	G.handCount[p] = 6;
	for(i = 0; i < 5; i++)
	{
    	    G.hand[p][i] = 4;
	}
	// set card number 6 or position 5 to a baron
	G.hand[p][5] = 15;
	G.numBuys = 1;
	G.coins = 0;
	G.supplyCount[estate] = 10;
	G.discardCount[p] = 0;
	//player calls baron_action saying they wish to discard an estate, but
        //they really do not have one. 
	baronEffect(&G,1, 0);
	printf("*** TEST 2 ***\n");
	printf("Player %d input to discard estate, none in hand.\n\n", p + 1);
	printf("Expect: discardCount = 2\n");
	printf("Actual: discardCount = %d\n", G.discardCount[p]);
	assert(G.discardCount[p] == 2, &passed, &failed);
        
        //check the the hand count is as expected        
        printf("Expect: handCount[p] = 5\n");
	printf("Actual: handCount[p] = %d\n", G.handCount[p]);
	assert(G.handCount[p] == 5, &passed, &failed);

	//check that the supply count of estates has decreased by 1
        printf("Expect: supplyCount[estate] = 9\n");
	printf("Actual: supplyCount[estate] = %d\n", G.supplyCount[estate]);
	assert(G.supplyCount[estate] == 9, &passed, &failed);
	
	// TEST #3 -- set up state
	// checking for the bug about not increasing buys correctly. 
	G.playedCardCount = 0;
	G.handCount[p] = 6;
        //for loop puts 5 coppers in the players hand
	for(int i = 0; i < 5; i++)
	{
	    G.hand[p][i] = 4;
	}
	// set the next player's hand to 5 cards. Each card will be a 
        // copper.
	G.handCount[p + 1] = 5;
	for(int i = 0; i < 5; i++)
	{
	    G.hand[p+1][i] = 4;
	}
	
	// set card number 6 or position 5 to a baron
	G.hand[p][5] = 15;
	G.numBuys = 1;
	G.coins = 0;
        // only put 1 estate in the supply count of estates
	G.supplyCount[estate] = 1;
	G.discardCount[p] = 0;
	G.discardCount[p+1] = 0;
        //call the function
	baronEffect(&G,1, 0);

	printf("*** TEST 3 ***\n");
	printf("Player %d checking that the number of buys increased.\n\n", p + 1);
	printf("Expect: numBuys = 2\n");
	printf("Actual: numBuys = %d\n", G.numBuys);
	assert(G.numBuys == 2, &passed, &failed);

        // this assert checks that there is no change in the next players state
        printf("Next player expects no state changes.\n");
        printf("Expect: handCount[nextPlayer] = 5\n");
	printf("Actual: handcount[nextPlayer] = %d\n", G.handCount[p + 1]);
	assert(G.handCount[p+1] == 5, &passed, &failed);

        //checking again that no change in next player's state. 
        printf("Expect: discardCount[nextPlayer] = 0\n");
	printf("Actual: discardCount[nextPlayer] = %d\n", G.discardCount[p + 1]);
	assert(G.discardCount[p+1] == 0, &passed, &failed);
	
	
	/* TEST #4 -- set up state for test	
	// test conditional that player says they have an estate, but they do not and set
	// supply to 2 to trigger the path if supplyCount is 0
	*/
	G.deckCount[p] = 10;
	G.playedCardCount = 0;
	
	G.handCount[p] = 6;
	G.discardCount[p] = 0;
	for(int i = 0; i < 5; i++)
	{
    	    // make cards 0 through 4 be copper
	    G.hand[p][i] = 4;
	}
	// set card number 6 or position 5 to a baron
	G.hand[p][5] = 15;
	G.numBuys = 1;
	G.playedCardCount = 0;
	G.handCount[p + 1] = 5;
	G.discardCount[p + 1] = 0;
        //set next player's hand so we will be able to check the state of their
        //hand after the function call
	for(int i = 0; i < 5; i++)
	{
	    // make cards 0 through 4 be copper
	    G.hand[p+1][i] = 4;
	}
	// set supplyCount of estates to 2
	G.supplyCount[1] = 2;

	// call the function
	baronEffect(&G,1,0);
	printf("*** TEST 4 ***\n");
	printf("Player %d input to discard estate, none in hand, supply = 2\n",p+1);
	printf("Also checking next player status and all cards in supply\n\n");
	printf("Expect: player discardCount = 1\n");
	printf("Actual: player discardCount = %d\n", G.discardCount[p]);
	assert(G.discardCount[p] == 1, &passed, &failed);

        // check to see if the playedCardCount is 1 (the baron was played)
	printf("Expect: playedCardCount = 1\n");
	printf("Actual: playedCardCount = %d\n", G.playedCardCount);
	assert(G.playedCardCount == 1, &passed, &failed);

        // the player's deckCount should increase by 1 because they gain an
        // estate.
	printf("Expect: player deckCount = 11\n");
	printf("Actual: player deckcount = %d\n", G.deckCount[p]);
	assert(G.deckCount[p] == 11, &passed, &failed);

        // the nextPlayer should have no change in their state
	printf("Expect: nextPlayer discardCount = 0\n");
	printf("Actual: nextPlayer discardCount = %d\n", G.discardCount[p+1]);
	assert(G.discardCount[p+1] == 0, &passed, &failed);

        // there should be no change in the next player's handCount
	printf("Expect: nextPlayer handCount = 5\n");
	printf("Actual: nextPlayer handCount = %d\n", G.handCount[p+1]);
	assert(G.handCount[p+1] == 5, &passed, &failed);
    
        // the next player's hand count should still be 10
        printf("Expect: nextPlayer deckCount = 10\n");
	printf("Actual: nextPlayer deckCount = %d\n", G.deckCount[p+1]);
	assert(G.deckCount[p+1] == 10, &passed, &failed);
	
        // the estate count should decrease from 2 to 1 because the player
        // did not have an estate in their hand
	printf("Expect: supplyCount[estate] = 1\n");
	printf("Actual: supplyCount[estate] = %d\n", G.supplyCount[estate]);
	assert(G.supplyCount[estate] == 1, &passed, &failed);

        //the supply of the other victory cards should not change
        printf("Expect: supplyCount[duchy] = 8\n");
	printf("Actual: supplyCount[duchy] = %d\n", G.supplyCount[duchy]);
	assert(G.supplyCount[duchy] == 8, &passed, &failed);

        //the supply of the other victory cards should not change
        printf("Expect: supplyCount[province] = 8\n");
	printf("Actual: supplyCount[province] = %d\n", G.supplyCount[duchy]);
	assert(G.supplyCount[province] == 8, &passed, &failed);
	
	
	//TEST #5
	//set up hand so that player says they want to discard an estate
	// but they do not have one. Also set supply to 0.  		
	G.playedCardCount = 0;
	G.handCount[p] = 6;
	for(int i = 0; i < 5; i++)
	{
	    // make cards 0 through 4 be copper
	    G.hand[p][i] = 4;
	}
	// set card number 6 or position 5 to a baron
	G.hand[p][5] = 15;
	G.numBuys = 1;
	G.discardCount[p] = 0;
	// set the supply count of estates to 0
	G.supplyCount[1] = 0;
	baronEffect(&G,1,0);
	printf("*** TEST 5 ***\n");
	printf("Player %d input to discard estate, none in hand, supply = 0.\n\n", p + 1);

        //player's discard count should not change because there are no 
        //estates in supply
	printf("Expect: discardCount = 0\n");
	printf("Actual: discardCount = %d\n", G.discardCount[p]);
	assert(G.discardCount[p] == 0, &passed, &failed);
	
        //supply count of estates should remain at 0
	printf("Expect: suppyCount[estate] = 0\n");
	printf("Actual: supplyCount[estate] = %d\n", G.supplyCount[1]);
	assert(G.supplyCount[1] == 0, &passed, &failed);

        // TEST #6
	// Test an edge case for supply of 1 when player has no estate card.
	printf("*** TEST 6 ***\n");
	printf("Player %d with no estate card, supply of estate is 1\n\n", p+1);
	G.playedCardCount = 0;
	G.handCount[p] = 6;
	for(int i = 0; i < 5; i++)
	{
	    // make cards 0 through 4 be copper
	    G.hand[p][i] = 4;
	}
	// set card number 6 or position 5 to a baron
	G.hand[p][5] = 15;
	G.discardCount[p] = 0;
	G.numBuys = 1;
	// set supplyCount of estates to 1
	G.supplyCount[1] = 1;
	baronEffect(&G,0,0);
	//check to see if supplyCount for estate was decremented to 0
        printf("Expect: supplyCount[estate] = 0\n");
	printf("Actual: supplyCount[estate] = %d\n", G.supplyCount[estate]);
	assert(G.supplyCount[estate] == 0, &passed, &failed);
	
	// expect to see the player's discardCount increase by 1
        printf("Expect: G.discardCount[player] = 1\n");
	printf("Actual: G.discardCount[player] = %d\n", G.discardCount[p]);
	assert(G.discardCount[p] == 1, &passed, &failed);

        //player's deckCount should increase since they gain an estate
	printf("Expect: G.deckCount[player] = 11\n");
	printf("Actual: G.deckCount[player] = %d\n", G.deckCount[p]);
	assert(G.deckCount[p] == 11, &passed, &failed);

        //the next player's state should not change
	printf("Expect: G.discardCount[nextPlayer] = 0\n");
	printf("Actual: G.discardCount[nextPayer] = %d\n", G.discardCount[p + 1]);
	assert(G.discardCount[p+ 1] == 0, &passed, &failed);
	
        //the next player's deck count should not change
	printf("Expect: G.deckCount[nextPlayer] = 10\n");
	printf("Actual: G.deckCount[nextPlayer] = %d\n", G.deckCount[p+1]);
	assert(G.deckCount[p+1] == 10, &passed, &failed);
	

        /* TEST #7	
	 * need to check second bug which is in outer  else that supply < 0
	 * make supplyCount of estates at -1 from the start
         * at the same time also check state of other victory cards
        */
	G.playedCardCount = 0;
	G.deckCount[p] = 10;
	G.handCount[p] = 6;
	for(int i = 0; i < 5; i++)
	{
	    // make cards 0 through 4 be copper
	    G.hand[p][i] = 4;
	}
	// set card number 6 or position 5 to a baron
	G.hand[p][5] = 15;
	G.numBuys = 1;
	// set supplyCount of estates to -1
	G.supplyCount[1] = -1;
	baronEffect(&G,0,0);
	//check to see if the supply count was made to a negative number
	printf("*** TEST 7 ***\n");
	printf("Player %d with no estate, supply count set to -1\n\n", p + 1);

        //if a supply count is -1, it should not keep decrementing
        printf("Expect: supplyCount[estate] = -1 (no change)\n");
	printf("Actual: supplyCount[estate] = %d\n", G.supplyCount[1]);
	assert(G.supplyCount[estate] == -1, &passed, &failed);

        // with no estates in supply, the deckCount should not change
	printf("Expect: deckCount[player] = 10\n");
	printf("Actual: deckCount[player] = %d\n", G.deckCount[p]);
	assert(G.deckCount[p] == 10, &passed, &failed);

        // the hand count of the player should decrease by 1 becasue the 
        // baron is played and there is no estate card to gain.
	printf("Expect: handCount[player] = 5\n");
	printf("Actual: handCount[player] = %d\n", G.handCount[p]);
	assert(G.handCount[p] == 5, &passed, &failed);

	//there should be no change in the next player's deckCount
        printf("Expect: deckCount[next player] = 10\n");
	printf("Actual: deckCount[next player] = %d\n", G.deckCount[p + 1]);
	assert(G.deckCount[p + 1] == 10, &passed, &failed);

        //there should be no change in other victory cards
        printf("There should be no change in other victory cards\n");
        printf("Expect: supplyCount[duchy] = 8\n");
	printf("Actual: supplyCount[duchy] = %d\n", G.supplyCount[duchy]);
	assert(G.supplyCount[duchy] == 8, &passed, &failed);

        printf("Expect: supplyCount[province] = 8\n");
	printf("Actual: supplyCount[province] = %d\n", G.supplyCount[province]);
	assert(G.supplyCount[province] == 8, &passed, &failed);
        
        /* TEST #8	
	 * this is a negative input test case. when the function is called,
         * used a negative index for the current player instead of a positive
         * index. Because of this invalid entry, there should be no change in 
         * the state of the game.
	 * also checking for state of other players hand
	 * and state of victory and kingdom cards
        */
	G.playedCardCount = 0;
	G.handCount[p] = 6;
	for(i = 0; i < 5; i++)
	{
	    // make cards 0 through 4 be copper
	    G.hand[p][i] = 4;
	}
	// set card number 6 or position 5 to a baron
	G.hand[p][5] = 15;
	G.numBuys = 1;
	// set supplyCount of estates to -1
	G.supplyCount[1] = 8;
	baronEffect(&G,0,0);
	//check to see if the supply count was made to a negative number
	printf("*** TEST 8 ***\n");
	printf("Includes negative testing and status of game.\n");
	printf("Player %d with no estate, estate supply = 8, currentPlayer = -5\n\n", p + 1);
	printf("Expect: handCount[p] = 6\n");
	printf("Actual: handCount[p] = %d\n", G.handCount[p]);
        assert(G.handCount[p] == 6, &passed, &failed);

        // expect no change in playedCardCount
	printf("Expect: playedCardCount = 0\n");
	printf("Actual: playedCardCount = %d\n", G.playedCardCount);
        assert(G.playedCardCount == 0, &passed, &failed);

        // the nextPlayer's hand count should remain unchanged. 
	printf("Expect: handCount[nextPlayer] = 5\n");
	printf("Actual: handCount[nextPlayer] = %d\n", G.handCount[p+1]);
	assert(G.handCount[p+1] == 5, &passed, &failed);

	printf("There should be no change in supply of victory cards\n");

        // there should be no change in the supply of estates
        printf("Expect: supplyCount[estate] = 8\n");
	printf("Actual: supplyCount[estate] = %d\n", G.supplyCount[estate]);
	assert(G.supplyCount[estate] == 8, &passed, &failed);

        printf("Expect: supplyCount[duchy] = 8\n");
	printf("Actual: supplyCount[duchy] = %d\n", G.supplyCount[duchy]);
	assert(G.supplyCount[duchy] == 8, &passed, &failed);

        printf("Expect: supplyCount[province] = 8\n");
	printf("Actual: supplyCount[province] = %d\n", G.supplyCount[duchy]);
	assert(G.supplyCount[province] == 8, &passed, &failed);

        // this for-loop iterates through all the kingdom cards to make sure
        // that the expected number is unchanged with the invalid entry.
	for(i = adventurer; i <= treasure_map; i++)
	{
	    for (int j = 0; j<= 10; j++)
    	    {
		if(k[j] == i)
		{	
		    cardNumToName(i, name);
		    if(i == 10 || i == 16)
		    {
                        //checking the supply count of victory cards
			printf("Expect: supplyCount of %s = 8\n", name);
			printf("Actual: supplyCount of %s = %d\n", name, G.supplyCount[i]);
			assert(G.supplyCount[i] == 8, &passed, &failed);
		    }
		    else
		    {
                        //checking the supply of kingdom cards
			printf("Expect: supplyCount of %s = 10\n", name);
			printf("Actual: supplyCount of %s = %d\n", name, G.supplyCount[i]);
			assert(G.supplyCount[i] == 10, &passed, &failed);
		    }
		}
	    }
	}
        // display a summary of the number of tests passed/failed	
	printf("Tests completed for player %d\n", p + 1);
        printf("Summary:\n");
	printf("Passed tests = %d\n", passed);
        printf("Failed tests = %d\n", failed);
    }

    printf("All tests done for %s!\n", TESTCARD);
    printf("******************************************************\n\n");
    

    return 0;
}

void assert(int expression, int *passed, int* failed)
{
    if(expression == 0)
    {
	*failed += 1;
        printf("Test failed.\n\n");
    }
    else
    {
	*passed += 1;
        printf("Test passed.\n\n");
    }      
}



