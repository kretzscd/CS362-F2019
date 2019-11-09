/* -----------------------------------------------------------------------
 * Unit testing for the ambassador_actionAttack refactored function 
 * Include the following lines in your makefile:
 *
 * unittest3: unittest3.c dominion.o rngs.o
 *      gcc -o unittest2 -g  unittest3.c interface.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include "interface.h"
#define TESTCARD "ambassador_actionAttack"

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

    
    for (p = 0; p < numPlayer - 1; p++)
    {
	printf ("BEGIN TESTING AMBASSADOR_ACTIONATTACK() FOR PLAYER %d\n\n", p + 1);

        printf("*** TEST 1 ***\n");
        printf("Player %d plays ambassador card and choses to return 1 estates.\n\n", p+1);

	// set up the variable needed
        memcpy(&T, &G, sizeof(struct gameState));   // reset the game state
	T.numPlayers = 2;
	T.handCount[p + 1] = 5;
        T.playedCardCount = 0;
	for(int i = 0; i < 4; i++)
	{
	    T.hand[p][i] = estate;
	}
        // set up for TEST 1 -- choice1 = 0, choice 2 = 1 
	T.hand[p][5] = 20;
	T.handCount[p] = 6;
	bonus = 0;

        // call the function
	ambassador_actionAttack(0,1, &T, 5, 0);
	printf("Expect: playedCardCount[p] = 1\n");
	printf("Actual: playedCardCount[player] = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 1, &passed, &failed);
	
	// supplyCount in estates will have a net increase of 0       
	printf("Expect: supplyCount[estate] = 8\n");
	printf("Actual: supplyCount[estate] = %d\n", T.supplyCount[1]);
	assert(T.supplyCount[estate] == 8, &passed, &failed);
	
	// checking handCount to make sure ambassador and 1 estate cards
        // were removed
	printf("Expect: handCount[player] = 4\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 4, &passed, &failed);
	
        printf("Expect: discardCount[nextPlayer] = 1\n");
        printf("Actual: discardCount[nextPlayer = %d\n", T.discardCount[p + 1]);
	assert(T.discardCount[p+1] == 1, &passed, &failed);

	// expect no change in the next player's hand count
	printf("Expect: handCount[nextPlayer] = 5\n");
        printf("Actual: handCount[nextPlayer] = %d\n", T.handCount[p+1]);
	assert(T.handCount[p+1] == 5, &passed, &failed);

	// Start TEST 2
	//The player wishes to discard 0 estates
        memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 6;
	for(int i = 0; i < 5; i++)
	{
	    T.hand[p][i] = estate;
	}
	// set card number 6 or position 5 to a minion
	T.hand[p][5] = 20;
	// set the next player's hand to > 4 cards
	T.handCount[p + 1] = 5;
	for(int i = 0; i < 5; i++)
	{
	    T.hand[p+1][i] = estate;
	}
	
        bonus = 0;
	T.discardCount[p] = 0;
	T.discardCount[p + 1] = 0;
	T.playedCardCount = 0;
	//player calls function 
	ambassador_actionAttack(1,2,&T, 5, 0);
	printf("*** TEST 2 ***\n");
	printf("Player %d chooses not to return cards to supply\n", p+1);
        printf("Next Player has 5 cards in hand\n\n");

        //expect that the playedCardCount will be 1
	printf("Expect: playedCardCount = 1\n");
        printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 1, &passed, &failed);

        //The player's hand count should be decreased from 6 to 5 since the
        //ambassador card was played. 
        printf("Expect: handCount[player] = 5\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 5, &passed, &failed);

        // the discardCount for the next player should not change since 
        // the player chose 0 cards to return.         
        printf("Expect: discardCount[nextPlayer] = 0\n");
	printf("Actual: discardCount[nextPlayer] = %d\n", T.discardCount[p + 1]);
	assert(T.discardCount[p + 1] == 0, &passed, &failed);

        // expect the next player's hand count to remain at 5
	printf("Expect: handCount[nextPlayer] = 5\n");
	printf("Actual: handCount[nextPlayer] = %d\n", T.handCount[p + 1]);
	assert(T.handCount[p + 1] == 5, &passed, &failed);

        // expect the supply of estates to be 8
        printf("Expect: supplyCount[estate] = 8\n");
	printf("Actual: supplyCount[estate] = %d\n", T.supplyCount[1]);
	assert(T.supplyCount[1] == 8, &passed, &failed);

        //supply of other victory cards should not change
	printf("Expect: supplyCount[duchy] = 8\n");
	printf("Actual: supplyCount[duchy] = %d\n", T.supplyCount[duchy]);
	assert(T.supplyCount[2] == 8, &passed, &failed);

        // supply of province should not change
	printf("Expect: supplyCount[province] = 8\n");
	printf("Actual: supplyCount[province] = %d\n", T.supplyCount[province]);
	assert(T.supplyCount[3] == 8, &passed, &failed);
	
	// TEST #3
	// player chooses to trash 3 estate cards
        // next player has 5 cards
        memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 6;
	for(int i = 0; i < 3; i++)
	{
	    T.hand[p][i] = 4;
	}
	// set card 5 to ambassador
	T.hand[p][5] = 20;
	T.handCount[p+1] = 5;
        // set the next player's hand to all coppers
	for(int i = 0; i < 5; i++)
	{
	    T.hand[p+1][i] = 4;
	}
        // reset variable to 0
	T.discardCount[p] = 0;
        T.discardCount[p + 1] = 0;
	T.playedCardCount = 0;
        // call the function with the invalid request to return 3 cards to 
        // supply
	ambassador_actionAttack(0,3,&G, 5, 0);
	printf("*** TEST 3 ***\n");
	printf("Player %d chooses to trash 3 estate cards\n", p+1);
        printf("Next Player has 5 cards in hand\n\n");

        //expect no change as the function should exit with a -1
	printf("Expect: playedCardCount = 0\n");
        printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 0, &passed, &failed);

        printf("Expect: handCount[player] = 6\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 6, &passed, &failed);
	// expect no change as the function should exit with a -1
        printf("Expect: discardCount[nextPlayer] = 0\n");
	printf("Actual: discardCount[nextPlayer] = %d\n", T.discardCount[p + 1]);
	assert(T.discardCount[p + 1] == 0, &passed, &failed);

        // expect no change as the function should exit with a -1
	printf("Expect: handCount[nextPlayer] = 5\n");
	printf("Actual: handCount[nextPlayer] = %d\n", T.handCount[p + 1]);
	assert(T.handCount[p + 1] == 5, &passed, &failed);

        printf("Expect: supplyCount[estate] = 8\n");
	printf("Actual: supplyCount[estate] = %d\n", T.supplyCount[1]);
	assert(T.supplyCount[1] == 8, &passed, &failed);

        // supply count of victory cards should be unchanged. 
	printf("Expect: supplyCount[duchy] = 8\n");
	printf("Actual: supplyCount[duchy] = %d\n", T.supplyCount[duchy]);
	assert(T.supplyCount[2] == 8, &passed, &failed);
        // supply count of victory cards should be unchanged
	printf("Expect: supplyCount[province] = 8\n");
	printf("Actual: supplyCount[province] = %d\n", T.supplyCount[province]);
	assert(T.supplyCount[3] == 8, &passed, &failed);
	
        //for loop used to check the supply of kingdom cards and ensure that
        //they are not changed
	for(i = adventurer; i <= treasure_map; i++)
	{
	    for (int j = 0; j<= 10; j++)
	    {
		if(k[j] == i)
		{	
		    cardNumToName(i, name);
		    if(i == 10 || i == 16)
		    {
                        //checking supply count of garden
			printf("Expect: supplyCount of %s = 8\n", name);
			printf("Actual: supplyCount of %s = %d\n", name, G.supplyCount[i]);
			assert(G.supplyCount[i] == 8, &passed, &failed);
		    }
		    else
		    {
                        //other kingdom cards should be 10
			printf("Expect: supplyCount of %s = 10\n", name);
			printf("Actual: supplyCount of %s = %d\n", name, G.supplyCount[i]);
			assert(G.supplyCount[i] == 10, &passed, &failed);
		    }
		}
	    }
	}

        /*Test 4 - player will make choice 1 be the hand position of the
         * ambassador card.  This will cause an immediate return so
         * we should see no changes in the remainder of the state*/
	memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 6;
	for(int i = 0; i < 3; i++)
	{
	    T.hand[p][i] = 4;
	}
	// set card number 3 or position 2 to a ambassador
	T.hand[p][5] = 20;
	T.handCount[p+1] = 5;
	for(int i = 0; i < 5; i++)
	{
	    T.hand[p+1][i] = 4;
	}
        //reset the variables used in the function
	T.discardCount[p] = 0;
        T.discardCount[p + 1] = 0;
	T.playedCardCount = 0;
        //call the function
	ambassador_actionAttack(5,1,&G, 5, 0);
	printf("*** TEST 4 ***\n");
	printf("Player %d makes choice1 the hand position of ambassador card\n", p+1);
        printf("Next Player has 5 cards in hand\n\n");

        // expect no change due to the invalid request
	printf("Expect: playedCardCount = 0\n");
        printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 0, &passed, &failed);

        //expect no change
        printf("Expect: handCount[player] = 6\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 6, &passed, &failed);
	
        //expect no change
        printf("Expect: discardCount[nextPlayer] = 0\n");
	printf("Actual: discardCount[nextPlayer] = %d\n", T.discardCount[p + 1]);
	assert(T.discardCount[p + 1] == 0, &passed, &failed);

	printf("Expect: handCount[nextPlayer] = 5\n");
	printf("Actual: handCount[nextPlayer] = %d\n", T.handCount[p + 1]);
	assert(T.handCount[p + 1] == 5, &passed, &failed);

        //expect no change since invalid input should cause the function to
        //return almost immediately
        printf("Expect: supplyCount[estate] = 8\n");
	printf("Actual: supplyCount[estate] = %d\n", T.supplyCount[1]);
	assert(T.supplyCount[1] == 8, &passed, &failed);

	printf("Expect: supplyCount[20] = 8\n");
	printf("Actual: supplyCount[20] = %d\n", T.supplyCount[duchy]);
	assert(T.supplyCount[20] == 8, &passed, &failed);


        // TEST 5 - player choses to trash 2 coppers but only has 1 in
        // their hand. Should cause an exit
	printf("*** TEST 5 ***\n");
        printf("Player %d choses to trash 2 coppers but only has 1 in their hand\n\n", p+1);

	// set up the variable needed
        memcpy(&T, &G, sizeof(struct gameState));   // reset the game state
	T.handCount[p] = 6;
	for(int i = 0; i < 4; i++)
	{
	    T.hand[p][i] = 1;
	}
        // set up for TEST 5 -- set one card in player's hand to a copper
        // set hand index 5 to an ambassador 
	T.hand[p][4] = 4;
	T.hand[p][5] = 20;
	T.handCount[p+1] = 5;
        T.discardCount[p+1] = 0;
        T.discardCount[p] = 0;
	bonus = 0;
	
        //call the function 
	ambassador_actionAttack(4,2, &T, 5, 0);
	printf("*** TEST 5 ***\n");
	
        // checking bonus
        printf("Expect: bonus = 0\n");
        printf("Actual: bonus = %d\n", bonus);
        assert(bonus == 0, &passed, &failed);

        // checking handCount to make sure none were removed since the input
        // was invalid. 
	printf("Expect: handCount[player] = 6\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 6, &passed, &failed);
	
	// checking to make sure playedCardCount did not change due to invalid
        // entry. 
	printf("Expect: playedCardCount = 0\n");
        printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 0, &passed, &failed);
	
        // discard count should not change due to invalid entry
        printf("Expect: discardCount[nextPlayer] = 0\n");
        printf("Actual: discardCount[nextPlayer] = %d\n", T.discardCount[p+1]);
	assert(T.discardCount[p+1] == 0, &passed, &failed);

        // the handCount of the nextPlayer also should not change
	printf("Expect: handCount[nextPlayer] = 5\n");
	printf("Actual: handCount[nextPlayer] = %d\n", T.handCount[p + 1]);
	assert(T.handCount[p + 1] == 5, &passed, &failed);

        //supply should not change of estates. 
	printf("Expect: supplyCount[estate] = 46\n");
	printf("Actual: supplyCount[estate] = %d\n", T.supplyCount[estate]);
	assert(T.supplyCount[estate] == 46, &passed, &failed);

        //supply should not change of coppers. 
	printf("Expect: supplyCount[copper] = 46\n");
	printf("Actual: supplyCount[copper] = %d\n", T.supplyCount[copper]);
	assert(T.supplyCount[copper] == 46, &passed, &failed);

	
        //TEST 6 - player choses to trash 2 estates, supply count of estate is 0
	// set up the variable needed
        memcpy(&T, &G, sizeof(struct gameState));   // reset the game state
	T.handCount[p] = 6;
	for(int i = 0; i < 5; i++)
	{
		T.hand[p][i] = 1;
	}
        //set up needed variables 
	T.hand[p][5] = 20;
	T.handCount[p+1] = 5;
	T.discardCount[p] = 0;
	T.discardCount[p + 1] = 0;
	T.playedCardCount = 0;
	// set supply count of estates to 0
	T.supplyCount[estate] = 0;
	
        // call function
	ambassador_actionAttack(1,2, &T, 5, 0);
	printf("*** TEST 6 ***\n");
        printf("Player %d choses to trash 2 estates, supplyCount[estate] = 0.\n", p+1);
	printf("Next player has 5 cards in hand\n\n");
	
	// expect playedCardCount = 1 because ambassador was played
	printf("Expect: playedCardCount = 1\n");
        printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 1, &passed, &failed);

	//The player's handCount should decrease to 3
        printf("Expect: handCount[player] = 3\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 3, &passed, &failed);
	//The next player's discard count should increase by 1	
        printf("Expect: discardCount[nextPlayer] = 1\n");
        printf("Actual: discardCount[nextPlayer] = %d\n", T.discardCount[p+1]);
	assert(T.discardCount[p+1] == 1, &passed, &failed);

	printf("Expect: supplyCount[estate] = 1\n");
	printf("Actual: supplyCount[estate] = %d\n", T.supplyCount[estate]);
	assert(T.supplyCount[1] == 1, &passed, &failed);

	
        // all tests done - give user a summary of passed/failed
        printf("Tests completed for player %d\n", p + 1);
        printf("Summary:\n");
	printf("Passed tests = %d\n", passed);
        printf("Failed tests = %d\n\n", failed);
	
		
    }

    printf("All tests done for %s!\n", TESTCARD);
    printf("********************************************************\n\n");
    

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
