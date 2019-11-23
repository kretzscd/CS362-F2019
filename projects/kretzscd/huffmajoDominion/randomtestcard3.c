/* -----------------------------------------------------------------------
 * Random testing for the tribute_action refactored function 
 * Include the following lines in your makefile:
 *
 *randomtestCard3: randomtestcard3.c dominion.o rngs.o
 *	gcc  -o randomtestCard3 -g  randomtestcard3.c dominion.o rngs.o $(CFLAGSa
 *	
 * Reference for finding the time it takes to run the program is:
 * https://www.geeksforgeeks.org/how-to-measure-time-taken-by-a-program-in-c/
 * ----------------------------------------------------------------------- */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include "interface.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>
#define TESTCARD "tributeEffect"

// if you want to see all the print outs
// then set NOISY_TEST to 1
#define NOISY_TEST 0

//declare and define a function used to generate random nums
int randomNumber(int min, int max)
{

    // formula = rand() %(max - min + 1) + min
    int randomNum = rand() % (max - min + 1) + min;

    return randomNum;
}


// declare assert function that also tracts counts on failed state variables
void assert(int expression, int *passed, int* failed,int* interimP,
        int* interimF, int *count);

int main() {

    //get the time for the program
    clock_t t;
    t = clock();
    //one time set up to use rand()
    srand(time(NULL));
    int numPlayer;
    int seed = 1000;
    int p, i, n;
    int bonus = 0;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , tribute, ambassador, minion, baron, great_hall};
    struct gameState G;
    int passed, failed, interimP, interimF;
    failed = 0, passed = 0, interimP = 0, interimF = 0;
    G.numActions = 1,G.numBuys = 1;
    //int revealedTributeCards[2] = {-1,-1};
    //int nextPlayer ;
    // x is the number of tests to be performed
    int x = 500;
    //declare/initialize counters for failed tests
    int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0;

    printf("BEGIN RANDOM TESTING FOR tributeEffect()\n\n");
    for (n = 0; n < x; n++)
    {
        //randomize the number of players
        numPlayer = 2;
        #if(NOISY_TEST == 1)
        printf("BEGIN RANDOM TESTING TRIBUTE_EFFECT() ROUND %d of %d\n\n", n + 1, x);
        #endif
        for(p = 0; p < numPlayer -1; p++)
        {
            #if(NOISY_TEST == 1)
            printf("*** TEST 1 FOR PLAYER %d ***\n", p+1);
            #endif
            #if(NOISY_TEST == 1)
            printf("Checking outcomes depending on next player's top two cards.\n\n");
            #endif
	    // TEST set up
            // set up the game state and variable so that they are random
            //randomize the seed used to set up the game

            //randomize setting up the structure of the game
            for (i = 0; i < sizeof(struct gameState); i++) {
                ((char*)&G)[i] = floor(Random() * 256);
            }
            //initialize game with random seed and random numPlayer
            initializeGame(numPlayer, k, seed, &G);

            // Randomize the next players handCount
	    G.handCount[p + 1] = randomNumber(0, 100);

            //the player's handCount is randomized
            G.handCount[p] = randomNumber(0,100);

            // set the state 
	    G.numActions = 1;
            bonus = 0;
	    G.coins = 0;

            //The player's cards are randomized
            for(i = 0; i < G.handCount[p]; i++)
            {
                G.hand[p][i] = randomNumber(0,26);
            }
           	
            // The next player's cards are randomized 
	    for(i = 0; i < G.handCount[p+1]; i++)
	    {
	        G.hand[p+1][i] = randomNumber(0,26);
	    }
            //randomize next player's deckCount
	    G.deckCount[p+1] = randomNumber(0,200); 
            G.discardCount[p+1] = randomNumber(0,200);
	    //G.hand[p][G.handCount[p] -1] = 19;
            
            //randomize the next player
            //nextPlayer = randomNumber(1,numPlayer -1);
            //re-initialize revealed tribute cards that will be assigned to
            //the top 2 cards of the player's random hand.
            //revealedTributeCards[0] = randomNumber(0,26);
	    //revealedTributeCards[1] = randomNumber(0,26);

            //if you want to see the randomized variables prior
            // to the function call, changed NOISY_TEST to 1
            #if(NOISY_TEST == 1)
            printf("G.handCount[p] = %d\n", G.handCount[p]);
            printf("G.playedCardCount = %d\n", G.playedCardCount);
            printf("The next player's index is: %d\n", nextPlayer);
            printf("G.handCount[nextPlayer] = %d\n", G.handCount[nextPlayer]);
            printf("G.deckCount[nextPlayer] = %d\n", G.deckCount[nextPlayer]);
            printf("G.hand[p][5] = %d\n", G.hand[p][5]);
            for(i = 0; i < G.handCount[p + 1]; i++)
	    {
	        printf("G.hand[player] at position %d = %d \n",i, G.hand[p + 1][i]);
	    }            
            printf("\n\n");
	    #endif
	
            // call the function
	    tributeEffect(&G, 0, 0);

            #if(NOISY_TEST == 1)
	    printf("Expect: numActions = 3\n");
	    printf("Actual: numActions = %d\n", G.numActions);
            #endif
	    assert(G.numActions == 3, &passed, &failed, &interimP, &interimF, &a);
	
	    // player's bonus coins should increase by revealing treasure card
            #if(NOISY_TEST == 1)
	    printf("Expect: bonus = 2\n");
	    printf("Actual: bonus = %d\n", bonus);
            #endif
	    assert(bonus == 2, &passed, &failed, &interimP, &interimF, &b);
	
	    // playedCardCount should increase because card was played
            #if(NOISY_TEST == 1)
	    printf("Expect: playedCardCount = 1\n");
            printf("Actual: playedCardCount = %d\n", G.playedCardCount);
            #endif
	    assert(G.playedCardCount == 1, &passed, &failed, &interimP, &interimF, &c);

	    // player's handCount should decrease since card was played
            #if(NOISY_TEST ==1)
	    printf("Expect: handCount[player] = 5\n");
	    printf("Actual: handCount[player] = %d\n", G.handCount[p]);
            #endif
	    assert(G.handCount[p] == 5, &passed, &failed, &interimP, &interimF, &d);

            // the next player's deckCount should decrease by 2 since they
            // revealed 2 cards. 
            #if(NOISY_TEST == 1)        
            printf("Expect: deckCount[nextPlayer] = 8\n");
            printf("Actual: deckCount[nextPlayer = %d\n", G.deckCount[p + 1]);
            #endif
	    assert(G.deckCount[p+1] == 8, &passed, &failed, &interimP, &interimF, &e);

	    //check next player's handCount
            #if(NOISY_TEST == 1)
            printf("Expect: handCount[nextPlayer] = 0\n");
	    printf("Actual: handCount[nextPlayer] = %d\n", G.handCount[p + 1]);
            #endif
	    assert(G.handCount[p + 1] == 0, &passed, &failed, &interimP, &interimF, &f);

            #if(NOISY_TEST == 1)
	    printf("Expect: discardCount[nextPlayer] = 0\n");
	    printf("Actual: discardCount[nextPlayer] = %d\n", G.discardCount[p+1]);
            #endif
	    assert(G.discardCount[p+1] == 0, &passed, &failed, &interimP, &interimF, &g);


	    //print out passed/failed tests for TEST 1
            #if(NOISY_TEST == 1)
            printf("Tests Results:\n");
            #endif
            #if(NOISY_TEST == 1)
            printf("Passed tests = %d\n", interimP);
            printf("Failed tests = %d\n\n", interimF);
            #endif

            // reset the interim passed and failed for test 2
            interimP = 0;
            interimF = 0;

	    // Start TEST 2
	    //limit the size of the deck and discardCount so that they can be 
            // tested. 
	    G.handCount[p] = randomNumber(6,20);
	    for(int i = 0; i < 5; i++)
	    {
	        G.hand[p][i] = randomNumber(0,26);
	    }
	    // set card number 6 or position 5 to a random card
	    G.hand[p][5] = randomNumber(0,26);
	    //set the card's in the next player's deck to random cards
	    G.handCount[p + 1] = 0;
	    for(int i = 0; i < 10; i++)
	    {
	        G.deck[p+1][i] = randomNumber(0,26);
	    }
            //reset all the variables used to their baseline
	    G.numActions = 1;
            bonus = 0;
	    G.coins = randomNumber(0,10);
	    G.discardCount[p] = randomNumber(0,10);
	    G.discardCount[p + 1] = randomNumber(0,10);
	    G.playedCardCount = randomNumber(0,10);
	    G.deckCount[p+1] = randomNumber(0,10);
            //nextPlayer = randomNumber(1, numPlayer -1);
	    //revealedTributeCards[0] = -1;
	    //revealedTributeCards[1] = -1;

           //if you want to see the randomized variables prior
            // to the function call, changed NOISY_TEST to 1
            #if(NOISY_TEST == 1)
            printf("G.discardCount[nextPlayer] = %d\n", G.discardCount[nextPlayer]);
            printf("G.discardCount[player] = %d\n", G.discardCount[p]);
            printf("The next player's index is: %d\n", nextPlayer);
            printf("G.handCount[nextPlayer] = %d\n", G.handCount[nextPlayer]);
            printf("G.handCount[player] = %d\n", G.handCount[p]);
            printf("G.hand[p][5] = %d\n", G.hand[p][5]);
            for(int i = 0; i < 4; i++)
	    {
	        printf("G.hand[player] at position %d = %d \n",i, G.hand[p][i]);
	    }          
            printf("\n\n");
	    #endif
	    //player calls function 
	    tributeEffect(&G,0, 0);

            #if(NOISY_TEST == 1)
            printf("*** TEST 2 FOR PLAYER %d ***\n", p+1);
            printf("Expect numAction = 1; bonus = 0; handCount[nextPlayer] = 0;,\
                    playedCardCount = 1 \n");
            printf("Expect deckCount[nextPlayer] = 0; discardCount[nextPlayer = 0\n\n");
            #endif

            #if(NOISY_TEST == 1)            
	    printf("Expect: numActions = 1\n");
            printf("Actual: numActions = %d\n", G.numActions);
            #endif
	    assert(G.numActions == 1, &passed, &failed, &interimP, &interimF, &a);
    
            //expect that there should be no bonus
            #if(NOISY_TEST == 1)
            printf("Expect: bonus = 0\n");
	    printf("Actual: bonus = %d\n", bonus);
            #endif
	    assert(bonus == 0, &passed, &failed, &interimP, &interimF, &b);

            #if(NOISY_TEST == 1)
            printf("Expect: playedCardCount = 1\n");
	    printf("Actual: playedCardCount = %d\n", G.playedCardCount);
            #endif
	    assert(G.playedCardCount == 1, &passed, &failed, &interimP, &interimF, &c);

	    //expect the next player's hand count to be 0
            #if(NOISY_TEST == 1)
            printf("Expect: handCount[nextPlayer] = 0\n");
	    printf("Actual: handCount[nextPlayer] = %d\n", G.handCount[p + 1]);
            #endif
	    assert(G.handCount[p + 1] == 0, &passed, &failed, &interimP, &interimF, &d);

	    //deckCount for the nextPlayer should stay at 0
            #if(NOISY_TEST == 1)
            printf("Expect: deckCount[nextPlayer] = 0\n");
	    printf("Actual: deckCount[nextPlayer] = %d\n", G.deckCount[p+1]);
            #endif
	    assert(G.deckCount[p+1] == 0, &passed, &failed, &interimP, &interimF, &e);

	    //check next player's handCount
            #if(NOISY_TEST == 1)
            printf("Expect: handCount[nextPlayer] = 0\n");
	    printf("Actual: handCount[nextPlayer] = %d\n", G.handCount[p + 1]);
            #endif
	    assert(G.handCount[p + 1] == 0, &passed, &failed, &interimP, &interimF, &f);

            #if(NOISY_TEST == 1)
	    printf("Expect: discardCount[nextPlayer] = 0\n");
	    printf("Actual: discardCount[nextPlayer] = %d\n", G.discardCount[p+1]);
            #endif
	    assert(G.discardCount[p+1] == 0, &passed, &failed, &interimP, &interimF,&g);

	    //print out passed/failed tests for TEST 2
            #if(NOISY_TEST == 1)
            printf("Tests Results: \n");
            printf("Passed tests = %d\n", interimP);
            printf("Failed tests = %d\n\n", interimF);
            #endif

            // reset the interim passed and failed for test 2
            interimP = 0;
            interimF = 0;

	
        }//end inner for loop
       		
    }//end for loop
    printf("Tests completed for %d players\n", numPlayer);
    printf("Summary:\n");
    printf("Passed tests = %d\n", passed);
    printf("Failed tests = %d\n\n", failed);

    printf("Of the failed tests:\n\n");
    printf("numActions failed %d times\n", a);
    printf("bonus failed %d times\n", b);
    printf("playedCardCount failed %d times\n", c);
    printf("player handCount failed %d times\n", d);
    printf("nextPlayer deckCount %d times\n", e);
    printf("nextPlayer handCount failed %d times\n", f);
    printf("nextPlayer discardCount failed %d times\n\n", g);


    printf("All tests done for %s!\n", TESTCARD);
    t = clock() - t;
    double time_taken = ((double) t)/CLOCKS_PER_SEC;
    printf("It took %f seconds for the testing to execute.\n", time_taken);
    printf("********************************************************\n\n");
    

    return 0;
}
//assert function definition and also counts # of times key state variables
//fail.
void assert(int expression, int* passed, int* failed, int* interimP,\
        int* interimF, int *count)
{
    if (expression == 0)
    {
        *count += 1;
        *failed += 1;
        *interimF+=1;
        #if(NOISY_TEST == 1)
        printf("Test failed.\n\n");
        #endif
    }
    else
    {
        *passed += 1;
        *interimP+=1;
        #if(NOISY_TEST == 1)
        printf("Test passed.\n\n");
        #endif
    }
}
