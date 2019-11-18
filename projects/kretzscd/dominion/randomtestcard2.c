/* -----------------------------------------------------------------------
 * Random testing for the minion_actionAttack refactored function 
 * Include the following lines in your makefile:
 *
 * randomtestCard2: randomtestcard2.c dominion.o rngs.o
 *	gcc  -o randomtestCard2 -g  randomtestcard2.c dominion.o rngs.o $(CFLAGS)
 *
 * Reference for finding the time it takes to run the program is:
 * https://www.geeksforgeeks.org/how-to-measure-time-taken-by-a-program-in-c/
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include "interface.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>
#define TESTCARD "minion_actionAttack"
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

//declare assert function
void assert(int expression, int *passed, int* failed,int* interimP,\
        int* interimF, int *count);

int main() {

    //get the time for the program
    clock_t t;
    t = clock();
    //one time set up to use rand()
    srand(time(NULL));
    // randomize the number of players in the game
    int numPlayer;// = randomNumber(2,4);
    int seed;
    int p, i, n;
    int bonus = 0;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, minion, baron, great_hall};
    struct gameState G;
    int passed, failed, interimP, interimF;
    failed = 0, passed = 0, interimP = 0, interimF = 0;
    G.coins = 0;
    G.playedCardCount = 0;
    G.numActions = 1;
    G.numBuys = 1;
    int choice1, choice2; 
    //declare/initialize counts that track the number of times certain state
    //outcomes fail
    int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0;
    //x represents the number of times the test will run
    int x = 100;
    printf("BEGIN RANDOM TESTING FOR MINION_ACTIONATTACK()\n\n");
    //use a for loop that can be expanded to the number of tests needed
    for (n = 0; n < x; n++)
    {
        numPlayer = randomNumber(2,4);
        #if(NOISY_TEST == 1)
        printf("BEGIN RANDOM TESTING MINION_ACTIONATTACK() ROUND %d of %d\n\n", n + 1, x);
        #endif
        for(p = 0; p < numPlayer -1; p++)
        {
            #if(NOISY_TEST == 1)
            printf("*** TEST 1 FOR PLAYER %d ***\n", p+1);
            #endif
            // set up the game state and variable so that they are random
            //randomize the seed used to set up the game
            seed = floor(Random()*5000);

            //randomize setting up the structure of the game
            for (i = 0; i < sizeof(struct gameState); i++) {
                ((char*)&G)[i] = floor(Random() * 256);
            }
            //initialize game with random seed and numPlayer
            initializeGame(numPlayer, k, seed, &G);
            //randomize the other variables in the test
	    G.handCount[p + 1] = randomNumber(6,50);
            G.handCount[p] = randomNumber(6,50);
            // set the players hand to random cards
            for(i = 0; i < G.handCount[p]; i++)
            {
                G.hand[p][i] = randomNumber(0,26);
            }
            // set up for TEST 1 -- choice1 = 1, bonus +2 
            G.hand[p][5] = 17;
            G.discardCount[p] = randomNumber(0,50);
            G.discardCount[p+1] = randomNumber(0,50);
            //choice1 and choice2 are booleans which is
            // why they are randomized to 0 or 1.
            choice1 = randomNumber(0,1);
            choice2 = randomNumber(0,1);
            //bonus and numActions are not randomized because
            // they are not variable for this test
	    bonus = 0;
            G.numActions = 1;

            //if you want to see the randomized variables prior
            // to the function call, changed NOISY_TEST to 1
            #if(NOISY_TEST == 1)
            printf("G.handCount[p+1] = %d\n", G.handCount[p+1]);
            printf("G.hand[p][5] = %d\n", G.hand[p][5]);
            printf("choice1 = %d\n", choice1);
            printf("choice2 = %d\n", choice2);
            for(int i = 0; i < 4; i++)
	    {
	        printf("G.hand[p][i] = %d \n",G.hand[p][i]);
	    }
            printf("\n\n");
	    #endif

            // call minion_actionAction with random input. Note that 5 is
            // an index of a randomized card. So it is a randomized value.
	    minion_actionAttack(choice1,choice2, &G, 5, 0);

            #if(NOISY_TEST ==1)
	    printf("Expect: bonus = 2\n");
            #endif
            #if(NOISY_TEST ==1)
	    printf("Actual: bonus = %d\n", bonus);
            #endif
	    assert(bonus == 2, &passed, &failed, &interimP, &interimF, &a);
	
	    // coins should not be changed because they are overwritten
            // in the updateCoins function and then player does not
            // get bonus
            #if(NOISY_TEST ==1)
	    printf("Expect: coins = 0\n");
            #endif
            #if(NOISY_TEST ==1)
	    printf("Actual: coins = %d\n", G.coins);
            #endif
	    assert(G.coins == 0, &passed, &failed, &interimP, &interimF, &b);
	
	    // checking handCount to make sure minion was removed
            // because it was played
            #if(NOISY_TEST ==1)
	    printf("Expect: handCount[p] = 5\n");
            #endif
            #if(NOISY_TEST ==1)
	    printf("Actual: handCount[p] = %d\n", G.handCount[p]);
            #endif
	    assert(G.handCount[p] == 5, &passed, &failed, &interimP, &interimF, &c);
	
	    // checking to make sure playedCardCount incremented since minion
            // was discarded. 
            #if(NOISY_TEST ==1)
	    printf("Expect: playedCardCount = 1\n");
            #endif
            #if(NOISY_TEST ==1)
            printf("Actual: playedCardCount = %d\n", G.playedCardCount);
            #endif
	    assert(G.playedCardCount == 1, &passed, &failed, &interimP, &interimF, &d);

            #if(NOISY_TEST ==1)	
            printf("Expect: numActions = 2\n");
            #endif
            #if(NOISY_TEST ==1)
            printf("Actual: numActions = %d\n", G.numActions);
            #endif
	    assert(G.numActions == 2, &passed, &failed, &interimP, &interimF, &e);

            // expect that the discardCount will be 0 because drawCard resets it
            // to 0.
            #if(NOISY_TEST == 1)
            printf("Expect: discardCount[player] = 0\n");
	    printf("Actual: discardCount[player] = %d\n", G.discardCount[p]);
            #endif
	    assert(G.discardCount[p] == 0, &passed, &failed, &interimP, &interimF, &f);

           // the next player's discard count will not changed, only their
            // playedCardCount for the redraws.
            #if(NOISY_TEST == 1)
            printf("Expect: discardCount[nextPlayer] = 0\n");
	    printf("Actual: discardCount[nextPlayer] = %d\n", G.discardCount[p + 1]);
            #endif
	    assert(G.discardCount[p + 1] == 0, &passed, &failed, &interimP, &interimF, &g);

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



        }//end of inner for loop	
		
    }//end of for loop
    printf("Tests completed for %d players\n", p + 1);
    printf("Summary:\n");
    printf("Passed tests = %d\n", passed);
    printf("Failed tests = %d\n\n", failed);

    printf("Of the failed tests:\n\n");
    printf("bonus failed %d times\n", a);
    printf("coins failed %d times\n", b);
    printf("player handCount failed %d times\n", c);
    printf("playedCardCount failed %d times\n", d);
    printf("numActions failed %d times\n", e);
    printf("player discardCount failed %d times\n", f);
    printf("nextPlayer discardCount failed %d times\n\n", g);

    printf("All tests done for %s!\n", TESTCARD);
    t = clock() - t;
    double time_taken = ((double) t)/CLOCKS_PER_SEC;
    printf("It took %f seconds for the testing to execute.\n", time_taken);
    printf("********************************************************\n\n");
    

    return 0;
}
//this assert function also tracks the count for the 8 state variables that
//were asserted so that their failure rate can be reported. 
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
