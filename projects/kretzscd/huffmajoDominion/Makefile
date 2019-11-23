# To compile and run type in at command prompt: make all
# To clean: type in at the command prompt: make clean

CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

randomtestCard1: randomtestcard1.c dominion.o rngs.o
	gcc  -o randomtestCard1 -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)

randomtestCard2: randomtestcard2.c dominion.o rngs.o
	gcc  -o randomtestCard2 -g  randomtestcard2.c dominion.o rngs.o $(CFLAGS)

randomtestCard3: randomtestcard3.c dominion.o rngs.o
	gcc  -o randomtestCard3 -g  randomtestcard3.c dominion.o rngs.o $(CFLAGS)

unittest1: unittest1.c dominion.o rngs.o
	gcc -o unittest1 -g  unittest1.c interface.c dominion.o rngs.o $(CFLAGS)

unittest2: unittest2.c dominion.o rngs.o
	gcc -o unittest2 -g  unittest2.c interface.c dominion.o rngs.o $(CFLAGS)

unittest3: unittest3.c dominion.o rngs.o
	gcc -o unittest3 -g  unittest3.c interface.c dominion.o rngs.o $(CFLAGS)

unittest4: unittest4.c dominion.o rngs.o
	gcc -o unittest4 -g  unittest4.c interface.c dominion.o rngs.o $(CFLAGS)

unittest5: unittest5.c dominion.o rngs.o
	gcc -o unittest5 -g  unittest5.c interface.c dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

unittestresults: unittest1 unittest2 unittest3 unittest4 unittest5
	./unittest1 > unittestresult.out
	./unittest2 >> unittestresult.out
	./unittest3 >> unittestresult.out
	./unittest4 >> unittestresult.out
	./unittest5 >> unittestresult.out
	gcov dominion.c -b -f >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

randomtestresults: randomtestCard1 randomtestCard2 randomtestCard3 
	./randomtestCard1 > randomtestresults.out
	./randomtestCard2 >> randomtestresults.out
	./randomtestCard3 >> randomtestresults.out
	gcov dominion.c -b -f >> randomtestresults.out
	cat dominion.c.gcov >> randomtestresults.out



player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player unittestresults randomtestresults 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe unittest1 unittest1.exe unittest2 unittest3 unittest4 unittest5 randomtestCard1 randomtestCard2 randomtestCard3
