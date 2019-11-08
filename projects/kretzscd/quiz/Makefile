# testme.c exists error[220] by design. It is very important when compiling
# and running this program that at the command line you use make all -i
# The -i flag tells make to ignore the exit with other than 0
# VERY IMPORTANT: use make all -i or make all --ignore-errors


CFLAGS = -Wall -fpic -coverage -lm

testme: testme.c
	gcc -o testme -g testme.c $(CFLAGS)

all: testme
	./testme
	gcov testme.c -b -f > testmeResults.out
	cat testme.c.gcov >> testmeResults.out

clean:
	rm -f testme testme.exe  *.gcov *.gcda *.gcno *.o testmeResults.out


