/*
 * CS362 Random Testing
 * File: testme.c
 * Description: This is a random tester that generates random chars until it
 * finds a '[', ,'(', '{', ' ', 'a', 'x', '}', ')', ']' which are between 32
 * and 125.  Depending on which of these is generated a state from 1 - 9 is
 * entered. At the same time, it randomly puts characters between 'e' to 't'
 * which are the ascii chars between 101 - 116 into a char array. The program
 * loops until a state of 9 and the char string contain reset with a null
 * terminator. The program exits with error 200. When reset 
 * state is 9.  It then exits with error and exit 200.
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    // use ASCII characters 32 - 125 
    // formula = rand() %(max - min + 1) + min
    char randomCharacter = rand()%(125 - 32 + 1) + 32;
    
    return randomCharacter;
}

char *inputString()
{
    //declare a static char array of 6 -- 5 with 1 left over for '\0'
    static char input[6];

    // initialize the c-string
    memset(input, '\0', sizeof(input));

    // generate random chars between 101 to 116 (includes the lower case
    // letters in reset
    int i;
    for(i = 0; i < 5; i++)
    {
        input[i] = rand()%(116 - 101 + 1) + 101;
    }

    return input;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
