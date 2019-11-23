/* -----------------------------------------------------------------------
 * Unit testing for the mine_action refactored function 
 * Include the following lines in your makefile:
 *
 * unittest5: unittest5.c dominion.o rngs.o
 *      gcc -o unittest5 -g  unittest5.c interface.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include "interface.h"
#define TESTCARD "mineEffect"

// declare assert function
void assert(int expression, int *passed, int* failed);

int main() {
    int seed = 1000;
    int numPlayer = 2;
    int p, i;
    int bonus = 0;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, ambassador, minion, baron, great_hall};
    struct gameState G, T;
    int passed = 0;
    int failed = 0;
    char name[] = "council_room";
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.coins = 0;
    G.playedCardCount = 0;
    G.numActions = 1;
    G.numBuys = 1;
    G.playedCardCount = 0;

    
    for (p = 0; p < numPlayer - 1; p++)
    {
	printf ("BEGIN TESTING mineEffect() FOR PLAYER %d\n\n", p + 1);

        printf("*** TEST 1 ***\n");
        printf("Player %d trashes copper and gets silver\n\n", p+1);

	// TEST 1 set up
        memcpy(&T, &G, sizeof(struct gameState));   // reset the game state
	T.numPlayers = 2;
	T.numActions = 1;
	T.handCount[p] = 5;
	bonus = 0;
	T.coins = 0;
	T.playedCardCount = 0;
        //set up the players hand
	for(int i = 0; i < 4; i++)
	{
	    T.hand[p][i] = copper;
	}         
	T.hand[p][4] = mine;
	
        // call the function
	mineEffect(&T, 0, 5, 4, p);
	
	// supply count of silver should decrease by 1
	// other treasure card supplies should not change
	printf("Expect: supplyCount[silver] = 39\n");
	printf("Actual: supplyCount[siver] = %d\n", T.supplyCount[silver]);
	assert(T.supplyCount[silver] == 39, &passed, &failed);

	printf("Expect: supplyCount[copper] = 46\n");
	printf("Actual: supplyCount[copper] = %d\n", T.supplyCount[copper]);
	assert(T.supplyCount[copper] == 46, &passed, &failed);

	printf("Expect: supplyCount[gold] = 30\n");
	printf("Actual: supplyCount[gold] = %d\n", T.supplyCount[gold]);
	assert(T.supplyCount[gold] == 30, &passed, &failed);

	// bonus coins should not increase
	printf("Expect: bonus = 0\n");
	printf("Actual: bonus = %d\n", T.coins);
	assert(T.coins == 0, &passed, &failed);
	
	// state coins should not increase because these are overwritten
	printf("Expect: coins = 0\n");
	printf("Actual: coins = %d\n", T.coins);
	assert(T.coins == 0, &passed, &failed);
	
	// playedCardCount should increase because card was played
	printf("Expect: playedCardCount = 1\n");
        printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 1, &passed, &failed);

	// player's handCount should decrease since card was played
	printf("Expect: handCount[player] = 5\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 5, &passed, &failed);
	

	// Start TEST 2
	//The next play has 0 deckCount and discard
        memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 5;
	for(int i = 0; i < 4; i++)
	{
	    T.hand[p][i] = copper;
	}
	// set card number 5 or position 4 to a mine
	T.hand[p][4] = mine;
	T.numActions = 1;
        bonus = 0;
	T.coins = 0;
	T.playedCardCount = 0;

	//player requests to trash a copper and gain gold   
	mineEffect(&T, 0, 6, 4, p);
	printf("*** TEST 2 ***\n");
	printf("Player %d trashes copper and requests gold\n\n", p+ 1);

	printf("Expect: supplyCount[siver] = 40\n");
        printf("Actual: supplyCount[silver] = %d\n", T.supplyCount[silver]);
	assert(T.supplyCount[silver] == 40, &passed, &failed);

	printf("Expect: supplyCount[copper] = 46\n");
        printf("Actual: supplyCount[copper] = %d\n", T.supplyCount[copper]);
	assert(T.supplyCount[copper] == 46, &passed, &failed);

	printf("Expect: supplyCount[gold] = 30\n");
        printf("Actual: supplyCount[gold] = %d\n", T.supplyCount[gold]);
	assert(T.supplyCount[gold] == 30, &passed, &failed);

        printf("Expect: bonus = 0\n");
	printf("Actual: bonus = %d\n", bonus);
	assert(bonus == 0, &passed, &failed);
	
        printf("Expect: coins = 0\n");
	printf("Actual: coins = %d\n", T.coins);
	assert(T.coins == 0, &passed, &failed);

	printf("Expect: handCount[player] = 5\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 5, &passed, &failed);

        printf("Expect: playedCardCount = 0\n");
	printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 0, &passed, &failed);

	
	// TEST #3
	// player wants to trash a copper and gain a
	// silver but they do not have a copper in their hand. 
        memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 5;
        //set the players hand
	for(int i = 0; i < 4; i++)
	{
	    T.hand[p][i] = adventurer;
	}
	// set card 5 to mine
	T.hand[p][4] = mine;
	T.playedCardCount = 0;
	T.coins = 0;
	T.numActions = 1;
	bonus = 0;

	// call the function
	mineEffect(&T, 3, 5, 4, p);
	printf("*** TEST 3 ***\n");
	printf("Player %d wants to trash a copper/gain a silver.\n", p+1);
	printf("There is no copper in the player's hand\n\n");

	printf("Expect: supplyCount[silver] = 40\n");
        printf("Actual: supplyCount[silver] = %d\n", T.supplyCount[silver]);
	assert(T.supplyCount[silver] == 40, &passed, &failed);

	printf("Expect: supplyCount[copper] = 46\n");
        printf("Actual: supplyCount[copper] = %d\n", T.supplyCount[copper]);
	assert(T.supplyCount[copper] == 46, &passed, &failed);

	printf("Expect: supplyCount[gold] = 30\n");
        printf("Actual: supplyCount[gold] = %d\n", T.supplyCount[gold]);
	assert(T.supplyCount[gold] == 30, &passed, &failed);

        printf("Expect: bonus = 0\n");
	printf("Actual: bonus = %d\n", bonus);
	assert(bonus == 0, &passed, &failed);
	
        printf("Expect: coins = 0\n");
	printf("Actual: coins = %d\n", T.coins);
	assert(T.coins == 0, &passed, &failed);

	printf("Expect: handCount[nextPlayer] = 5\n");
	printf("Actual: handCount[nextPlayer] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 5, &passed, &failed);

	printf("Expect: playedCardCount = 0\n");
	printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 0, &passed, &failed);
	
        //check the supply of the other victory and kingdom cards
	for(i = adventurer; i <= treasure_map; i++)
	{
	    for (int j = 0; j<= 10; j++)
	    {
		if(k[j] == i)
		{	
		    cardNumToName(i, name);
		    if(i == 10 || i == 16)
		    {
			printf("Expect: supplyCount of %s = 8\n", name);
			printf("Actual: supplyCount of %s = %d\n", name, G.supplyCount[i]);
			assert(G.supplyCount[i] == 8, &passed, &failed);
		    }
		    else
		    {
		    	printf("Expect: supplyCount of %s = 10\n", name);
			printf("Actual: supplyCount of %s = %d\n", name, G.supplyCount[i]);
			assert(G.supplyCount[i] == 10, &passed, &failed);
		    }
		}
	    }
	}

        /*Test 4 - player to discard silver, gain a gold, no gold in supply
         */
	memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 5;
	for(int i = 0; i < 4; i++)
	{
	    T.hand[p][i] = 5;
	}
	// set card number 5 to a mine
	T.hand[p][4] = mine;
	T.playedCardCount = 0;
	T.numActions = 1;
	T.coins = 0;
	bonus = 0;
	T.supplyCount[gold] = 0;
	mineEffect(&T, 2, 6, 4, p);
	printf("*** TEST 4 ***\n");
	printf("Player %d trashes silver, requests gold\n", p);
        printf("Gold supply is 0\n\n");

	printf("Expect: numActions = 1\n");
        printf("Actual: numActions = %d\n", T.numActions);
	assert(T.numActions == 1, &passed, &failed);

	printf("Expect: supplyCount[silver] = 40\n");
        printf("Actual: supplyCount[silver] = %d\n", T.supplyCount[silver]);
	assert(T.supplyCount[silver] == 40, &passed, &failed);

	printf("Expect: supplyCount[copper] = 46\n");
        printf("Actual: supplyCount[copper] = %d\n", T.supplyCount[copper]);
	assert(T.supplyCount[copper] == 46, &passed, &failed);

	printf("Expect: supplyCount[gold] = 0\n");
        printf("Actual: supplyCount[gold] = %d\n", T.supplyCount[gold]);
	assert(T.supplyCount[gold] == 0, &passed, &failed);


	printf("Expect: handCount[player] = 5\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 5, &passed, &failed);

	printf("Expect: playedCardCount = 0\n");
	printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 0, &passed, &failed);


        // TEST 5 - choice 1 to discard gold, request silver
        // supply of treasure cards is full.

	memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 5;
	for(int i = 0; i < 4; i++)
	{
	    T.hand[p][i] = 6;
	}
	// set card number 5 to a mine
	T.hand[p][4] = mine;
	T.playedCardCount = 0;
	T.deckCount[p+1] = 0;
	T.numActions = 1;
	T.coins = 0;
	bonus = 0;
        //call the function
        mineEffect(&T, 0, 5, 4, p);
	printf("*** TEST 5 ***\n");
	printf("Player %d discards a gold, requests a silver\n", p+1);
        printf("Supply of treasures is full \n\n");

	printf("Expect: supplyCount[silver] = 39\n");
        printf("Actual: supplyCount[silver] = %d\n", T.supplyCount[silver]);
	assert(T.supplyCount[silver] == 39, &passed, &failed);

	printf("Expect: supplyCount[copper] = 46\n");
        printf("Actual: supplyCount[copper] = %d\n", T.supplyCount[copper]);
	assert(T.supplyCount[copper] == 46, &passed, &failed);

	printf("Expect: supplyCount[gold] = 30\n");
        printf("Actual: supplyCount[gold] = %d\n", T.supplyCount[gold]);
	assert(T.supplyCount[gold] == 30, &passed, &failed);

        printf("Expect: supplyCount[estate] = 8\n");
	printf("Actual: supplyCount[estate] = %d\n", T.supplyCount[1]);
	assert(T.supplyCount[1] == 8, &passed, &failed);

	printf("Expect: supplyCount[duchy] = 8\n");
	printf("Actual: supplyCount[duchy] = %d\n", T.supplyCount[duchy]);
	assert(T.supplyCount[duchy] == 8, &passed, &failed);

	printf("Expect: supplyCount[province] = 8\n");
	printf("Actual: supplyCount[province] = %d\n", T.supplyCount[province]);
	assert(T.supplyCount[province] == 8, &passed, &failed);

	printf("Expect: handCount[player] = 4\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 4, &passed, &failed);
	
	// checking playedCardCount  to make sure it was 0
	printf("Expect: playedCardCount = 1\n");
	printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 1, &passed, &failed);

	
        //TEST 6 - Set choice 2 to invalid input of -30
	// set up the variable needed
        memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 5;
        //set up the player's hand with coppers
	for(int i = 0; i < 4; i++)
	{
	    T.hand[p][i] = 4;
	}
	// set card number 3 or position 2 to a mine
	T.hand[p][4] = mine;
	T.playedCardCount = 0;
	T.deckCount[p+1] = 0;
	T.numActions = 1;
	T.coins = 0;
	bonus = 0;
        //call the function
	mineEffect(&T, 0, -30, 4, p);
	printf("*** TEST 6 ***\n");
        printf("Player %d has invalid choice2 of -30\n\n", p + 1);
	
	printf("Expect: supplyCount[silver] = 40\n");
        printf("Actual: supplyCount[silver] = %d\n", T.supplyCount[silver]);
	assert(T.supplyCount[silver] == 40, &passed, &failed);

	printf("Expect: supplyCount[copper] = 46\n");
        printf("Actual: supplyCount[copper] = %d\n", T.supplyCount[copper]);
	assert(T.supplyCount[copper] == 46, &passed, &failed);

	printf("Expect: supplyCount[gold] = 30\n");
        printf("Actual: supplyCount[gold] = %d\n", T.supplyCount[gold]);
	assert(T.supplyCount[gold] == 30, &passed, &failed);

	printf("Expect: handCount[player] = 5\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 5, &passed, &failed);
	
	// checking playedCardCount is 0
	printf("Expect: playedCardCount = 0\n");
	printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 0, &passed, &failed);


	//TEST 7 - trash silver, gain gold
	// This test demonstrates both bugs that were inserted

 	memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 5;
	for(int i = 0; i < 4; i++)
	{
	    T.hand[p][i] = 7;
	}
	// set card number 3 or position 2 to a minion
	T.hand[p][4] = mine;
	//set position 0 to a silver
	T.hand[p][0] = silver;
	
	T.playedCardCount = 0;

	mineEffect(&T, 0, 6, 4, p);
	printf("*** TEST 7 ***\n");
        printf("Player %d trash a silver, gain a gold\n", p+1);
	printf("Demonstrates 2 bugs placed into function\n\n");

	printf("Expect: supplyCount[silver] = 40\n");
        printf("Actual: supplyCount[silver] = %d\n", T.supplyCount[silver]);
	assert(T.supplyCount[silver] == 40, &passed, &failed);

	printf("Expect: supplyCount[copper] = 46\n");
        printf("Actual: supplyCount[copper] = %d\n", T.supplyCount[copper]);
	assert(T.supplyCount[copper] == 46, &passed, &failed);

	printf("Expect: supplyCount[gold] = 29\n");
        printf("Actual: supplyCount[gold] = %d\n", T.supplyCount[gold]);
	assert(T.supplyCount[gold] == 30, &passed, &failed);


	printf("Expect: handCount[nextPlayer] = 4\n");
	printf("Actual: handCount[nextPlayer] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 4, &passed, &failed);

	printf("Expect: playedCardCount = 1\n");
	printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 1, &passed, &failed);

	for(int x = 0; x< T.handCount[p]; x++)
	{
	    if(x == 0)
	    {
		printf("Expect: hand position[%d] = 6\n", x);
		printf("Actual: hand position[%d] = %d\n", x, T.hand[p][x]);
		assert(T.hand[p][x] == 6, &passed, &failed);
	    }
	    else
	    {
		printf("Expect: hand position[%d] = 7\n", x);
		printf("Actual: hand position[%d] = %d\n", x, T.hand[p][x]);
		assert(T.hand[p][x] == 7, &passed, &failed);
	    }
	}
	printf("\n");

	//TEST 8 - choice2 = 60 which is an invalid number
	
	memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 5;
	for(int i = 0; i < 4; i++)
	{
	    T.hand[p][i] = 7;
	}
	// set card number 4 or position 3 to a mine
	T.hand[p][4] = mine;
	//set position 0 to a silver
	T.hand[p][0] = silver;
	
	T.playedCardCount = 0;
	// call the function
	mineEffect(&T, 0, 60, 4, p);
	printf("*** TEST 8 ***\n");
        printf("Player %d uses invalid choice 2 of 30\n\n", p+1);

	printf("Expect: supplyCount[silver] = 40\n");
        printf("Actual: supplyCount[silver] = %d\n", T.supplyCount[silver]);
	assert(T.supplyCount[silver] == 40, &passed, &failed);

	printf("Expect: supplyCount[copper] = 46\n");
        printf("Actual: supplyCount[copper] = %d\n", T.supplyCount[copper]);
	assert(T.supplyCount[copper] == 46, &passed, &failed);

	printf("Expect: supplyCount[gold] = 30\n");
        printf("Actual: supplyCount[gold] = %d\n", T.supplyCount[gold]);
	assert(T.supplyCount[gold] == 30, &passed, &failed);

	printf("Expect: handCount[player] = 5\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 4, &passed, &failed);
	
	printf("Expect: playedCardCount = 0\n");
	printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 0, &passed, &failed);

        printf("Tests completed for player %d\n\n", p + 1);
        printf("Summary:\n");
	printf("Passed tests = %d\n", passed);
        printf("Failed tests = %d\n\n", failed);
	
		
    }

    printf("All tests done for %s!\n", TESTCARD);
    printf("********************************************************\n\n");
    

    return 0;
}
//assert function definition
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
