CFLAGS = -Wall -fpic -coverage -lm

testme: testme.c
	gcc -o testme -g testme.c $(CFLAGS)

all: testme
	./testme

clean:
	rm -f testme testme.exe  *.gcov *.gcda *.gcno *.o 


