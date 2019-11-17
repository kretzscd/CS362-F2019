/* -----------------------------------------------------------------------
 * Random testing for the baron_action refactored function
 * Include the following lines in your makefile:
 *
 *randomtestCard1: randomtestcard1.c dominion.o rngs.o
 *	gcc  -o randomtestCard1 -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------*/
 
#include "interface.h"
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define TESTCARD "baron_action"

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

//declare the assert function
void assert(int expression, int* passed, int* failed, int* interimP,\
        int* interimF, int *count);

int main() {
    //one time set up to use rand()
    srand(time(NULL));
    int numPlayer;
    int p;
    int bonus;
    int seed;
    int k[10] = { adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall };
    struct gameState G;
    //these are 4 variables used by assert to report tests that
    // passed/failed and for intermediate results at the
    // end of each test 
    int passed, failed, interimP, interimF;
    failed = 0, passed = 0, interimP = 0, interimF = 0;
    int choice1;
    int currentPlayer;
    int i,n;
    int x = 100;
    //declare and initialize counters for failed tests
    int a = 0, b =0, c = 0, d = 0, e = 0, f = 0, g = 0;

    printf("BEGIN RANDOM TESTING BARON_ACTION()\n\n");
    //this for-loop will run for a set number of times
    for (n = 0; n < x; n++)
    {
        
        //randomly generate the number of players in the game
        numPlayer = randomNumber(2,4);
        #if(NOISY_TEST == 1)
        printf("BEGIN RANDOM TESTING BARON_ACTION() ROUND %d of %d\n\n", n+1, x);
        #endif
        //a new game is randomly generated for each player
        for(p = 0; p < numPlayer; p++)
        {
            // set up the game state and variable so that they are random
            //randomize the seed used to set up the game
            seed = floor(Random()*5000);
            memset(&G, 23, sizeof(struct gameState));   // clear the game state
            initializeGame(numPlayer, k, seed, &G); // initialize a new game

            // set up for TEST 1 -- choice1 = 1, bonus +4 
            // randomize the variables used.
            G.coins = randomNumber(-10, 100);
            G.numBuys = randomNumber(0, 100);
            G.handCount[p] = randomNumber(0,250);
            for(i = 0; i < G.handCount[p]; i++)
	    {
	        G.hand[p][i] = randomNumber(0,26);
	    }
            G.playedCardCount = randomNumber(0,250);
            G.discardCount[p] = randomNumber(0, 250);
            G.supplyCount[estate] = randomNumber(-10,10);
            bonus = randomNumber(0,20);
            currentPlayer = randomNumber(0, 1);
            choice1 = randomNumber(0, 10);

            //if you want to see the values of the randomized variables prior
            // to the function call, changed NOISY_TEST to 1
            #if(NOISY_TEST == 1)
            printf("G.coins = %d\n", G.coins);
            printf("bonus = %d\n", bonus);
            printf("G.numBuys = %d\n", G.numBuys);
            printf("choice1 = %d\n", choice1);
            printf("G.handCount[player] = %d\n", G.handCount[p]);
            printf("currentPlayer = %d\n", currentPlayer);
            printf("G.playedCardCount = %d\n", G.playedCardCount);
            printf("G.discardCount[p] = %d\n", G.discardCount[p]);
            printf("G.supplyCount[estate] = %d\n", G.supplyCount[estate]);
            for(int i = 0; i < G.handCount[p]; i++)
	    {
	        printf("G.hand[p][i] = %d \n",G.hand[p][i]);
	    }
            printf("\n\n");
	    #endif
            
            #if(NOISY_TEST == 1)
            printf("*** TEST 1 FOR PLAYER %d ***\n", p+1);
            #endif

            // call baron_action with randomized input
            baron_action(choice1, &G, currentPlayer);

            #if(NOISY_TEST == 1)
            printf("Player %d plays baron card and discards estate.\n", p + 1);
            printf("Expect bonus = 4; handCount = 5; numbuys = 2; playedCardCount = 1\n\n");
            printf("Expect: bonus coins = 4\n");
            printf("Actual: bonus coins = %d\n", bonus);
            #endif
            assert(bonus == 4, &passed, &failed, &interimP, &interimF, &a);

            //check that the number of buys was increased appropriately
            #if(NOISY_TEST == 1)
            printf("Expect: numBuys = 2\n");
            printf("Actual: numBuys = %d\n", G.numBuys);
            #endif
            assert(G.numBuys == 2, &passed, &failed, &interimP, &interimF, &b);

            // checking handCount to make sure baron was discarded
            #if(NOISY_TEST == 1)
            printf("Expect: G.handCount[p] = 5\n");
            printf("Actual: G.handCount[p] = %d\n", G.handCount[p]);
            #endif
            assert(G.handCount[p] == 5, &passed, &failed, &interimP, &interimF, &c);

            // checking to make sure playedCardCount incremented since baron
                // was discarded. 
            #if(NOISY_TEST == 1)
            printf("Expect: G.playedCardCount = 1\n");
            printf("Actual: G.playedCardCount = %d\n", G.playedCardCount);
            #endif
            assert(G.playedCardCount == 1, &passed, &failed, &interimP, &interimF, &d);

            //check that the supply count of estates has decreased by 1
            #if(NOISY_TEST == 1)
            printf("Expect: supplyCount[estate] = 9\n");
            printf("Actual: supplyCount[estate] = %d\n", G.supplyCount[estate]);
            #endif
            assert(G.supplyCount[estate] == 9, &passed, &failed, &interimP, &interimF, &e );

            #if(NOISY_TEST == 1)
            printf("Expect: player discardCount = 1\n");
            printf("Actual: player discardCount = %d\n", G.discardCount[p]);
            #endif
            assert(G.discardCount[p] == 1, &passed, &failed, &interimP, &interimF, &f);

            #if(NOISY_TEST == 1)
            printf("Expect: player deckCount = 11\n");
            printf("Actual: player deckCount = %d\n", G.deckCount[p]);
            #endif
            assert(G.discardCount[p] == 1, &passed, &failed, &interimP, &interimF, &g);


            //print out passed/failed tests 
            #if(NOISY_TEST == 1)
            printf("Test Results: %d\n", p+1);
            #endif
            #if(NOISY_TEST == 1)
            printf("Passed tests = %d\n", interimP);
            printf("Failed tests = %d\n\n", interimF);
            #endif

            // reset the interim passed and failed
            interimP = 0;
            interimF = 0;

        }//end inner for loop
        #if(NOISY_TEST == 1)
        printf("END OF TESTS FOR THIS ROUND\n\n");
        #endif
    }//end outer for loop
    // display a summary of the number of tests passed/failed	
    printf("Tests completed for %d players\n", numPlayer);
    printf("Summary:\n");
    printf("Passed tests = %d\n", passed);
    printf("Failed tests = %d\n\n", failed);
    printf("Of the failed tests:\n\n");
    printf("coins failed %d times\n", a);
    printf("numBuys failed %d times\n", b);
    printf("player handcount failed %d times\n", c);
    printf("playedCardCount failed %d times\n", d);
    printf("supply[estates] failed %d times\n", e);
    printf("player discardCount failed %d times\n", f);
    printf("deckCount failed %d times\n\n", g);

    printf("All tests done for %s!\n", TESTCARD);
    printf("******************************************************\n\n");


    return 0;
}
//this function tracts the number of passed/failed as well as which variables
//passed or failed. 
void assert(int expression, int* passed, int* failed, int* interimP,\
        int* interimF, int *count)
{
    if (expression == 0)
    {
        *failed += 1;
        *interimF+=1;
        *count += 1;
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



