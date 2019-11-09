/* -----------------------------------------------------------------------
 * Unit testing for the tribute_action refactored function 
 * Include the following lines in your makefile:
 *
 * unittest4: unittest4.c dominion.o rngs.o
 *      gcc -o unittest4 -g  unittest4.c interface.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include "interface.h"
#define TESTCARD "tribute_action"

// declare assert function
void assert(int expression, int *passed, int* failed);

int main() {
    // decleare variables needed to test function
    int seed = 1000;
    int numPlayer = 2;
    int p, i;
    int bonus = 0;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , tribute, ambassador, minion, baron, great_hall};
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
    int revealedTributeCards[2] = {-1,-1};

    
    for (p = 0; p < numPlayer - 1; p++)
    {
	printf ("BEGIN TESTING TRIBUTE_ACTION() FOR PLAYER %d\n\n", p + 1);

        printf("*** TEST 1 ***\n");
        printf("Next player's top 2 cards are a silver and adventure\n\n");

	// TEST 1 set up
        memcpy(&T, &G, sizeof(struct gameState));   // reset the game state
	T.numPlayers = 2;
        // set the state variables
	T.numActions = 1;
        // The next players handCount is set to 5
	T.handCount[p + 1] = 5;
	bonus = 0;
	T.coins = 0;
        // The next player's first three cards are action 
	for(int i = 0; i < 3; i++)
	{
	    T.hand[p+1][i] = estate;
	}
        // The next player's hand position 4 and 5 are silver/adventurer
	T.hand[p+1][3] = silver;
	T.hand[p+1][4] = adventurer;
	T.deckCount[p+1] = 10;
        //The player has 6 cards in their hand.  Index 5 is a tribute card.  
	T.hand[p][5] = 19;
	T.handCount[p] = 6;
	
        // call the function
	tribute_action(&T,0, 1, revealedTributeCards);
	printf("Expect: numActions = 3\n");
	printf("Actual: numActions = %d\n", T.numActions);
	assert(T.numActions == 3, &passed, &failed);
	
	// player's bonus coins should increase by revealing treasure card
	printf("Expect: bonus = 2\n");
	printf("Actual: bonus = %d\n", bonus);
	assert(bonus == 2, &passed, &failed);
	
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

        // the next player's deckCount should decrease by 2 since they
        // revealed 2 cards.         
        printf("Expect: deckCount[nextPlayer] = 8\n");
        printf("Actual: deckCount[nextPlayer = %d\n", T.deckCount[p + 1]);
	assert(T.deckCount[p+1] == 8, &passed, &failed);


	// Start TEST 2
	//The next play has 0 deckCount and discard
        memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 6;
	for(int i = 0; i < 5; i++)
	{
	    T.hand[p][i] = estate;
	}
	// set card number 6 or position 5 to tribute
	T.hand[p][5] = 19;
	//set the card's in the next player's deck to all copper 
	T.handCount[p + 1] = 0;
	for(int i = 0; i < 10; i++)
	{
	    T.deck[p+1][i] = 4;
	}
        //reset all the variables used to their baseline
	T.numActions = 1;
        bonus = 0;
	T.coins = 0;
	T.discardCount[p] = 0;
	T.discardCount[p + 1] = 0;
	T.playedCardCount = 0;
	T.deckCount[p+1] = 0;
	revealedTributeCards[0] = -1;
	revealedTributeCards[1] = -1;
	//player calls function  
	tribute_action(&T,0, 1, revealedTributeCards);
	printf("*** TEST 2 ***\n");
	printf("Next player's deckCount and discardCount are both 0\n\n");

	printf("Expect: numActions = 1\n");
        printf("Actual: numActions = %d\n", T.numActions);
	assert(T.numActions == 1, &passed, &failed);
    
        //expect that there should be no bonus
        printf("Expect: bonus = 0\n");
	printf("Actual: bonus = %d\n", bonus);
	assert(bonus == 0, &passed, &failed);
	
        printf("Expect: coins = 0\n");
	printf("Actual: coins = %d\n", T.coins);
	assert(T.coins == 0, &passed, &failed);

	//expect the next player's hand count to be 0
        printf("Expect: handCount[nextPlayer] = 0\n");
	printf("Actual: handCount[nextPlayer] = %d\n", T.handCount[p + 1]);
	assert(T.handCount[p + 1] == 0, &passed, &failed);

        printf("Expect: playedCardCount = 1\n");
	printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 1, &passed, &failed);

	//deckCount for the nextPlayer should stay at 0
        printf("Expect: deckCount[nextPlayer] = 0\n");
	printf("Actual: deckCount[nextPlayer] = %d\n", T.deckCount[p+1]);
	assert(T.deckCount[p+1] == 0, &passed, &failed);

	printf("Expect: discardCount[nextPlayer] = 0\n");
	printf("Actual: discardCount[nextPlayer] = %d\n", T.discardCount[p+1]);
	assert(T.discardCount[p+1] == 0, &passed, &failed);
	
	// TEST #3
	// next player has deck of 10 and all cards in hand are coppers
        memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 6;
        //set the player's hand in index 0 to 4 to estates
	for(int i = 0; i < 3; i++)
	{
	    T.hand[p][i] = 4;
	}
	// set card 5 to tribute
	T.hand[p][5] = 19;
	T.handCount[p+1] = 5;
        // put all coppers in the next player's hand
	for(int i = 0; i < 5; i++)
	{
	    T.hand[p+1][i] = 4;
	}
        //reset other variables to baseline
	T.discardCount[p] = 0;
        T.discardCount[p + 1] = 0;
	T.playedCardCount = 0;
	T.coins = 0;
	T.numActions = 1;
	bonus = 0;
	T.deckCount[p+1] = 10;
        // set the entire deck of the next player to coppers
	for(i = 0; i < 10; i++)
	{
	    T.deck[p+1][i] = copper;
	}
	revealedTributeCards[0] = -1;
	revealedTributeCards[1] = -1;
	// call the function
	tribute_action(&T,0, 1, revealedTributeCards);
	printf("*** TEST 3 ***\n");
	printf("Next player's deckCount is 10, top 2 cards are coppers\n\n");
        
	printf("Expect: numActions = 1\n");
        printf("Actual: numActions = %d\n", T.numActions);
	assert(T.numActions == 1, &passed, &failed);

        printf("Expect: bonus = 0\n");
	printf("Actual: bonus = %d\n", bonus);
	assert(bonus == 0, &passed, &failed);
	
        printf("Expect: coins = 0\n");
	printf("Actual: coins = %d\n", T.coins);
	assert(T.coins == 0, &passed, &failed);

	printf("Expect: deckCount[nextPlayer] = 8\n");
	printf("Actual: deckCount[nextPlayer] = %d\n", T.deckCount[p + 1]);
	assert(T.deckCount[p + 1] == 8, &passed, &failed);

        printf("Expect: discardCount[nextPlayer] = 0\n");
	printf("Actual: discardCount[estate] = %d\n", T.discardCount[p + 1]);
	assert(T.discardCount[p +1] == 0, &passed, &failed);

	printf("Expect: playedCardCount = 2\n");
	printf("Actual: playedCardCount = %d\n", T.playedCardCount);
	assert(T.playedCardCount == 2, &passed, &failed);
	
        // check the state of the other parts of the game by checking 
        // kingdom and victory cards.
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

        /*Test 4 - nextPlayer deckCount = 1, nextPlayer discardCount = 0
         */
	memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 6;
	for(int i = 0; i < 5; i++)
	{
	    T.hand[p][i] = 4;
	}
	// set card number 5 to a tribute
	T.hand[p][5] = 19;
	T.handCount[p+1] = 0;
	// set the next player's one card in the deck
	// to a copper
	T.deck[p+1][0] = 4;
	T.discardCount[p] = 0;
        T.discardCount[p + 1] = 0;
	T.playedCardCount = 0;
	T.deckCount[p+1] = 1;
	T.numActions = 1;
	T.coins = 0;
	bonus = 0;
	revealedTributeCards[0] = -1;
	revealedTributeCards[1] = -1;
	tribute_action(&T,0, 1, revealedTributeCards);
	printf("*** TEST 4 ***\n");
	printf("Next player deckCount = 1, nextPlayer discardCount = 0\n");
        printf("Next player's only card is a copper\n\n");

	printf("Expect: numActions = 3\n");
        printf("Actual: numActions = %d\n", T.numActions);
	assert(T.numActions == 3, &passed, &failed);

        printf("Expect: deckCount[nextPlayer] = 0\n");
	printf("Actual: deckCount[nextPlayer] = %d\n", T.deckCount[p+1]);
	assert(T.deckCount[p+1] == 0, &passed, &failed);
	
        printf("Expect: bonus = 0\n");
	printf("Actual: bonus = %d\n", bonus);
	assert(bonus == 0, &passed, &failed);

	printf("Expect: coins = 0\n");
	printf("Actual: coins = %d\n", T.coins);
	assert(T.coins == 0, &passed, &failed);

        printf("Expect: supplyCount[estate] = 8\n");
	printf("Actual: supplyCount[estate] = %d\n", T.supplyCount[1]);
	assert(T.supplyCount[1] == 8, &passed, &failed);

	printf("Expect: supplyCount[duchy] = 8\n");
	printf("Actual: supplyCount[duchy] = %d\n", T.supplyCount[duchy]);
	assert(T.supplyCount[duchy] == 8, &passed, &failed);

	printf("Expect: supplyCount[province] = 8\n");
	printf("Actual: supplyCount[province] = %d\n", T.supplyCount[province]);
	assert(T.supplyCount[province] == 8, &passed, &failed);



        // TEST 5 - nextPlayer discardCount = 1, nextPlayer deckCount = 0

	memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 6;
	for(int i = 0; i < 5; i++)
	{
	    T.hand[p][i] = 4;
	}
	// set card number 5 to a tribute
	T.hand[p][5] = 19;
	T.handCount[p+1] = 0;
	// set the next player's one card in the deck
	// to a copper
	T.deck[p+1][0] = gardens;
	T.discard[p+1][0] = gardens;
	
        //reset the variables to baseline
        T.discardCount[p] = 0;
        T.discardCount[p + 1] = 1;
	T.playedCardCount = 0;
	T.deckCount[p+1] = 0;
	T.numActions = 1;
	T.coins = 0;
	bonus = 0;
	revealedTributeCards[0] = -1;
	revealedTributeCards[1] = -1;
	tribute_action(&T,0, 1, revealedTributeCards);
	printf("*** TEST 5 ***\n");
	printf("Next player deckCount = 0, nextPlayer discardCount = 1\n");
        printf("Next player's only card in discard is a garden \n\n");

	// check the number of actions
	printf("Expect: numActions = 1\n");
	printf("Actual: numActions = %d\n", T.numActions);
	assert(T.numActions == 1, &passed, &failed);
	
	printf("Expect: discardCount[nextPlayer] = 0\n");
	printf("Actual: discardCount[nextPlayer] = %d\n", T.discardCount[p+1]);
	assert(T.discardCount[p+1] == 0, &passed, &failed);
	
	// checking bonus to make sure it was 0
	printf("Expect: bonus = 0\n");
	printf("Actual: bonus = %d\n", bonus);
	assert(bonus == 0, &passed, &failed);
	
	// checking to make sure coins are 0
	printf("Expect: coins = 0\n");
        printf("Actual: coins = %d\n", T.coins);
	assert(T.coins == 0, &passed, &failed);

	// the player starts with 6 cards in hand/ should draw 2 and have
        // one subtracted for playing card. handCount should be 7
        printf("Expect: handCount[player] = 7\n");
        printf("Actual: handCount[player] = %d\n", T.handCount[p]);
	assert(T.handCount[p] == 7, &passed, &failed);

	
        //TEST 6 - nextPlayer discardCount = 10, nextPlayer deckCount = 0
	// set up the variable needed
        memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 6;
	for(int i = 0; i < 5; i++)
	{
	    T.hand[p][i] = 4;
	}
	// set card number 3 or position 2 to a minion
	T.hand[p][5] = 19;
	T.handCount[p+1] = 0;
	
	T.discardCount[p] = 0;
        T.discardCount[p + 1] = 10;
	T.playedCardCount = 0;
	T.deckCount[p+1] = 0;
	T.numActions = 1;
	T.coins = 0;
	bonus = 0;
	//make all the cards in discard be copper
	for(i = 0; i<10; i++)
	{
	    T.discard[p+1][i] = copper;
	}
	revealedTributeCards[0] = -1;
	revealedTributeCards[1] = -1;
	tribute_action(&T,0, 1, revealedTributeCards);
	printf("*** TEST 6 ***\n");
        printf("Next player's discardCount = 10, deckCount = 0.\n");
	printf("All cards in discard are copper\n\n");
	
	// numActions should remain unchanged at 1
	printf("Expect: numActions = 1\n");
        printf("Actual: numActions = %d\n", T.numActions);
	assert(T.numActions == 1, &passed, &failed);

	printf("Expect: deckCount[nextPlayer] = 8\n");
	printf("Actual: deckCount[nextPlayer] = %d\n", T.deckCount[p + 1]);
	assert(T.deckCount[p+1] == 8, &passed, &failed);

	printf("Expect: discardCount[nextPlayer] = 0\n");
	printf("Actual: discardCount[nextPlayer] = %d\n", T.discardCount[p + 1]);
	assert(T.discardCount[p+1] == 0, &passed, &failed);
		
        printf("Expect: bonus = 0\n");
        printf("Actual: bonus = %d\n", bonus);
	assert(bonus == 0, &passed, &failed);

	printf("Expect: coins = 0\n");
	printf("Actual: coins = %d\n", T.coins);
	assert(T.coins == 0, &passed, &failed);

	printf("Expect: playedCardCount = 1\n");
	printf("Actual: playedCardCount= %d\n", T.playedCardCount);
	assert(T.playedCardCount == 1, &passed, &failed);

	printf("Expect: handCount[player] = 6\n");
	printf("Actual: handCount[player] %d\n", T.handCount[p]);
	assert(T.handCount[p] == 6, &passed, &failed);
	
	//TEST 7 - nextPlayer discardCount = 0, nextPlayer deckCount = 10
	// nextPlayer top 2 cards are a silver and estate however, the
	// index for next player is -100

 	memcpy(&T, &G, sizeof(struct gameState));
	T.numPlayers = 2;
	T.handCount[p] = 6;
	for(int i = 0; i < 5; i++)
	{
	    T.hand[p][i] = 4;
	}
	// set card number 3 or position 2 to a minion
	T.hand[p][5] = 19;
	T.handCount[p+1] = 5;
	for(int i = 0; i < 4; i++)
	{
	    T.hand[p+1][i] = 1;
	}
	T.hand[p+1][4] = silver;
	T.discardCount[p] = 0;
        T.discardCount[p + 1] = 0;
	T.playedCardCount = 0;
	T.deckCount[p+1] = 10;
	T.numActions = 1;
	T.coins = 0;
	bonus = 0;
	revealedTributeCards[0] = -1;
	revealedTributeCards[1] = -1;
	tribute_action(&T,0,-100, revealedTributeCards);
	printf("*** TEST 7 ***\n");
        printf("Invalid index input for next player\n\n");
	
	// numActions should remain unchanged at 1
	printf("Expect: numActions = 1\n");
        printf("Actual: numActions = %d\n", T.numActions);
	assert(T.numActions == 1, &passed, &failed);

	printf("Expect: deckCount[nextPlayer] = 10\n");
	printf("Actual: deckCount[nextPlayer] = %d\n", T.deckCount[p + 1]);
	assert(T.deckCount[p+1] == 10, &passed, &failed);
		
        printf("Expect: bonus = 0\n");
        printf("Actual: bonus = %d\n", bonus);
	assert(bonus == 0, &passed, &failed);

	printf("Expect: coins = 0\n");
	printf("Actual: coins = %d\n", T.coins);
	assert(T.coins == 0, &passed, &failed);

	printf("Expect: handCount[player] = 6\n");
	printf("Actual: handCount[player] %d\n", T.handCount[p]);
	assert(T.handCount[p] == 6, &passed, &failed);

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
