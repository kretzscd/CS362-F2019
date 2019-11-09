#Assignment 3
#To run type in make all
#To clean type in make clean


CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

#testDrawCard: testdrawcard.c dominion.o rngs.o
	#gcc  -o testDrawCard -g  testdrawcard.c dominion.o rngs.o $(CFLAGS)

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

testCoinCard: testUpdateCoins.c dominion.o rngs.o
	gcc -o testCoinCard -g testUpdateCoins.c dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

#runtests: testDrawCard 
#	./testDrawCard &> unittestresult.out
#	gcov dominion.c >> unittestresult.out
#	cat dominion.c.gcov >> unittestresult.out

unittestresults: unittest1 unittest2 unittest3 unittest4 unittest5
	./unittest1 > unittestresult.out
	./unittest2 >> unittestresult.out
	./unittest3 >> unittestresult.out
	./unittest4 >> unittestresult.out
	./unittest5 >> unittestresult.out
	gcov dominion.c -b -f >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player unittestresults


clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out unittest1 unittest1.exe testCoinCard testCoinCard.exe unittest2 unittest2.exe unittest3 unittest3.exe unittest4 unittest4.exe unittest5 unittest5.exe
