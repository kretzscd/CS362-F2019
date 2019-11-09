/* -----------------------------------------------------------------------
 * Unit testing for the minion_actionAttack refactored function 
 * Include the following lines in your makefile:
 *
 * unittest2: unittest2.c dominion.o rngs.o
 *      gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include "interface.h"
#define TESTCARD "minion_actionAttack"

//declare assert function
void assert(int expression, int *passed, int* failed);

int main() {
    //set up the original game state
    int seed = 1000;
    int numPlayer = 2;
    int p, i;
    int bonus = 0;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, minion, baron, great_hall};
    struct gameState G, T;
    int passed = 0;
    int failed = 0;
    char name[] = "council_room";
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.coins = 0;
    G.playedCardCount = 0;
    G.numActions = 1;
    G.numBuys = 1;

    //use a for loop that can be expanded to include more than one player for
    //testing purposes
    for (p = 0; p < numPlayer - 1; p++)
    {
	printf ("BEGIN TESTING MINION_ACTIONATTACK() FOR PLAYER %d\n\n", p + 1);

        printf("*** TEST 1 ***\n");
        printf("Player %d plays minion card and choses choice1.\n\n", p+1);

	// set up the variable needed
        memcpy(&T, &G, sizeof(struct gameState));   // reset the game state
	T.handCount[p + 1] = 5;
        // set the players hand to have coppers
	for(int i = 0; i < 4; i++)
	{
	    T.hand[p][i] = 4;
	}
        // set up for TEST 1 -- choice1 = 1, bonus +2 
	T.hand[p][5] = 17;
	T.handCount[p] = 6;
	bonus = 0;
	
        // Player wishes to choice 1
	minion_actionAttack(1,-1, &T, 5, 0);
	printf("Expect: bonus = 2\n");
	printf("Actual: bonus = %d\n", bonus);
	assert(bonus == 2, &passed, &failed);
	
	// coins should not be changed because they are overwritten
        // in the updateCoins function and then player does not
        // get bonus
	printf("Expect: coins = 0\n");
	printf("Actual: coins = %d\n", T.coins);
	assert(T.coins == 0, &passed, &failed);
	
	// checking handCount to make sure minion was removed
        // because it was played
	printf("Expect: handCount[p] = 5\n");
	printf("Actual: handCount[p] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 5, &passed, &failed);
	
	// checking to make sure playedCardCount incremented since minion
        // was discarded. 
	printf("Expect: playedCardCount = 1\n");
        printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 1, &passed, &failed);
	
        printf("Expect: numActions = 2\n");
        printf("Actual: numActions = %d\n", T.numActions);
	assert(T.numActions == 2, &passed, &failed);


	// Start TEST 2
	//The player chooses choice 2 and the next player has > 4 cards
        memcpy(&T, &G, sizeof(struct gameState));
	T.handCount[p] = 6;
	for(int i = 0; i < 5; i++)
	{
	    T.hand[p][i] = 4;
	}
	// set card number 6 or position 5 to a minion
	T.hand[p][5] = 17;
	// set the next player's hand to > 4 cards
	T.handCount[p + 1] = 5;
        // set the next players hand to all coppers
	for(int i = 0; i < 5; i++)
	{
	    T.hand[p+1][i] = 4;
	}
        //set bonus, discardCount, playedCount all to 0	
        bonus = 0;
        T.numActions = 1;
	T.discardCount[p] = 0;
	T.playedCardCount = 0;

	//player calls minion_actionAttack  using choice2, nextPlayer has > 4 cards   
	minion_actionAttack(0,1,&T, 5, 0);
	printf("*** TEST 2 ***\n");
	printf("Player %d input choice2, nextPlayer has >4 cards in hand\n", p+1);

        // expect no change in bonus
	printf("Expect: bonus coins = 0\n");
        printf("Actual: bonus coins = %d\n", bonus);
	assert(bonus == 0, &passed, &failed);

        //check that the number of actions increased
        printf("Expect: numActions = 2\n");
        printf("Actual: numActions = %d\n", T.numActions);
	assert(T.numActions == 2, &passed, &failed);
        // expect that the discardCount will be 0 because drawCard resets it
        // to 0.
        printf("Expect: discardCount[player] = 0\n");
	printf("Actual: discardCount[player] = %d\n", T.discardCount[p]);
	assert(T.discardCount[p] == 0, &passed, &failed);

        // the players hand count should decrease by 1
        printf("Expect: handCount[player] = 5\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 5, &passed, &failed);
	
        // the next player's discard count will not changed, only their
        // playedCardCount for the redraws.
        printf("Expect: discardCount[nextPlayer] = 0\n");
	printf("Actual: discardCount[nextPlayer] = %d\n", T.discardCount[p + 1]);
	assert(T.discardCount[p + 1] == 0, &passed, &failed);

        //the handCount of the next player should not change.
	printf("Expect: handCount[nextPlayer] = 5\n");
	printf("Actual: handCount[nextPlayer] = %d\n", T.handCount[p + 1]);
	assert(T.handCount[p + 1] == 5, &passed, &failed);

        // expect the played card count to be 9, 4 redraws for each player and
        // then 1 for the player using the minion card
	printf("Expect: playedCardCount = 9\n");
	printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 9, &passed, &failed);
	
	// TEST #3
	// player chooses choice 2 and only has 3 cards, one is minion,
        // next player has < 4 cards
        memcpy(&T, &G, sizeof(struct gameState));
	T.handCount[p] = 3;
        T.numActions = 1;
        //set the player's hand to all coppers
	for(int i = 0; i < 3; i++)
	{
	    T.hand[p][i] = 4;
	}
	// set card number 3 or position 2 to a minion
	T.hand[p][2] = 17;
	T.handCount[p+1] = 3;
        //set the next player's hand to all coppers
	for(int i = 0; i < 3; i++)
	{
	    T.hand[p+1][i] = 4;
	}
	T.discardCount[p] = 0;
        T.discardCount[p + 1] = 0;
	minion_actionAttack(0,1,&G, 2, 0);

	printf("*** TEST 3 ***\n");
	printf("Player %d uses choice2, has 3 cards including minion\n", p + 1);

	//check that the number of actions increased
        printf("Expect: numActions = 2\n");
        printf("Actual: numActions = %d\n", T.numActions);
	assert(T.numActions == 2, &passed, &failed);printf("Next player has 3 cards\n\n");
        //discardCount will be 0 because draw resets it to 0
	printf("Expect: discardCount[player] = 0\n");
        printf("Actual: discardCount[player] = %d\n", T.discardCount[p]);
	assert(T.discardCount[p] == 0, &passed, &failed);

        //handCount of player should now be 4
	printf("Expect: handCount[player] = 4\n");
        printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 4, &passed, &failed);
	
        //drawCard resets the discardCount to 0
	printf("Expect: discardCount[nextPlayer] = 0\n");
	printf("Actual: discardCount[nextPlayer = %d\n", T.discardCount[p+1]);
	assert(T.discardCount[p+1] == 0, &passed, &failed);

        // the next player should still have 3 cards
	printf("Expect: handCount[nextPlayer] = 3\n");
        printf("Actual: handCount[player] = %d\n", T.handCount[p + 1]);
	assert(T.handCount[p + 1] == 4, &passed, &failed);

        //the player had 2 cards to discard with trash flag set to 0, so the 
        //played card count should be 2 
	printf("Expect: playedCardCount = 2\n");
        printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 2, &passed, &failed);

	printf("There should be no change in supply of victory cards\n");
        //the supply for the victory cards should not change. 
        printf("Expect: supplyCount[estate] = 8\n");
	printf("Actual: supplyCount[estate] = %d\n", G.supplyCount[estate]);
	assert(G.supplyCount[estate] == 8, &passed, &failed);

        printf("Expect: supplyCount[duchy] = 8\n");
	printf("Actual: supplyCount[duchy] = %d\n", G.supplyCount[duchy]);
	assert(G.supplyCount[duchy] == 8, &passed, &failed);

        printf("Expect: supplyCount[province] = 8\n");
	printf("Actual: supplyCount[province] = %d\n", G.supplyCount[duchy]);
	assert(G.supplyCount[province] == 8, &passed, &failed);

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
	// TEST #4
	// player chooses choice 2 and only has 1 card, it is the minion, 
        // next player has 5
        memcpy(&T, &G, sizeof(struct gameState));
	T.handCount[p] = 1;
        T.numActions = 1;
	// set card number 3 or position 2 to a minion
	T.hand[p][0] = 17;
	T.handCount[p+1] = 5;
        // set the next player's hand to all copper's
	for(int i = 0; i < 5; i++)
	{
	    T.hand[p+1][i] = 4;
	}
	minion_actionAttack(-1,1,&G, 0, 0);
	printf("*** TEST 4 ***\n");
	T.discardCount[p] = 0;
        T.discardCount[p + 1] = 0;
	printf("Player %d uses choice2, has 1 card which is the minion\n", p + 1);
	printf("Next player has 3 cards\n\n");
        //drawCard will reset the discardCount to 0
	printf("Expect: discardCount[player] = 0\n");
        printf("Actual: discardCount[player] = %d\n", T.discardCount[p]);
	assert(T.discardCount[p] == 0, &passed, &failed);
        //the player had 1 card in hand and played it, he redraws 4, so he
        //should have 4 in hand. 
	printf("Expect: handCount[player] = 4\n");
        printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 4, &passed, &failed);
        //expect drawCard to reset ot 0	
	printf("Expect: discardCount[nextPlayer] = 0\n");
	printf("Actual: discardCount[nextPlayer = %d\n", T.discardCount[p+1]);
	assert(T.discardCount[p+1] == 0, &passed, &failed);
        //hand count for next player should remain at 5.
	printf("Expect: handCount[nextPlayer] = 5\n");
        printf("Actual: handCount[player] = %d\n", T.handCount[p + 1]);
	assert(T.handCount[p + 1] == 5, &passed, &failed);

	printf("Expect: playedCardCount = 5\n");
        printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 5, &passed, &failed);

        
	// TEST #5
	// player chooses neither choice 1 or choice 2
        // next player has 5
	printf("*** TEST 5 ***\n");
        printf("Player %d plays minion card but choses neither choice1 or choice2.\n\n", p+1);

	// set up the variable needed
        memcpy(&T, &G, sizeof(struct gameState));   // reset the game state
	T.handCount[p + 1] = 5;
        T.numActions = 1;
        T.playedCardCount = 0;
	for(int i = 0; i < 4; i++)
	{
	    T.hand[p][i] = 4;
	}
        // set up for TEST 5 -- neither choice1 or 2
	T.hand[p][5] = 17;
	T.handCount[p] = 6;
	bonus = 0;
	
        // Player wishes to choice 1
	minion_actionAttack(0,0, &T, 5, 0);
	printf("Expect: bonus = 0\n");
	printf("Actual: bonus = %d\n", bonus);
	assert(bonus == 0, &passed, &failed);
	
	// coins should not be changed because they are overwritten
        // in the updateCoins function and then player does not
        // get bonus
	printf("Expect: coins = 0\n");
	printf("Actual: coins = %d\n", T.coins);
	assert(T.coins == 0, &passed, &failed);
	
	// checking handCount to make sure minion was removed
        // because it was played
	printf("Expect: handCount[player] = 5\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 5, &passed, &failed);
	
	// checking to make sure playedCardCount incremented since minion
        // was discarded. 
	printf("Expect: playedCardCount = 1\n");
        printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 1, &passed, &failed);
	
        printf("Expect: numActions = 2\n");
        printf("Actual: numActions = %d\n", T.numActions);
	assert(T.numActions == 2, &passed, &failed);

	printf("Expect: handCount[nextPlayer] = 5\n");
	printf("Actual: handCount[nextPlayer] = %d\n", T.handCount[p + 1]);
	assert(T.handCount[p + 1] == 5, &passed, &failed);

        //TEST 6 - boundary case, neither player has any cards. players
        // selects choice 2

	printf("*** TEST 6 ***\n");
        printf("Player %d plays minion and choice2.\n", p+1);
	printf("Neither player nor next player have any cards\n\n");

	// set up the variable needed
        memcpy(&T, &G, sizeof(struct gameState));   // reset the game state
	T.handCount[p + 1] = 0;
	
	T.handCount[p] = 0;
	bonus = 0;
	T.coins = 0;
	T.numActions = 1;
	
        // Player wishes to choice 2
	minion_actionAttack(0,1, &T, 0, 0);
	printf("Expect: bonus = 0\n");
	printf("Actual: bonus = %d\n", bonus);
	assert(bonus == 0, &passed, &failed);
	
	// coins should not be changed because they are overwritten
        // in the updateCoins function and then player does not
        // get bonus
	printf("Expect: coins = 0\n");
	printf("Actual: coins = %d\n", T.coins);
	assert(T.coins == 0, &passed, &failed);
	
	// checking handCount to make sure minion was removed
        // because it was played
	printf("Expect: handCount[player] = 4\n");
	printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 4, &passed, &failed);
	
	// checking to make sure playedCardCount incremented since minion
        // was discarded. 
	printf("Expect: playedCardCount = 1\n");
        printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 1, &passed, &failed);
	
        printf("Expect: numActions = 2\n");
        printf("Actual: numActions = %d\n", T.numActions);
	assert(T.numActions == 2, &passed, &failed);

	printf("Expect: handCount[nextPlayer] = 0\n");
	printf("Actual: handCount[nextPlayer] = %d\n", T.handCount[p + 1]);
	assert(T.handCount[p + 1] == 0, &passed, &failed);

	

        printf("Tests completed for player %d\n", p + 1);
        printf("Summary:\n");
	printf("Passed tests = %d\n", passed);
        printf("Failed tests = %d\n", failed);
	
		
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
