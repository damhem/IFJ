/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file main.c
*
* @author Daniel Pátek (xpatek08)
* @author Daniel Čechák (xcecha06)
* @author Zdeněk Kroča (xkroca02)
* @author Unensanaa Tumenjargal (xtumen01)
*/

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main() {

  //open file w/ program and push it to the stdin
  FILE *fp;
  if ((fp = freopen("program.txt", "r", stdin)) == NULL) {
      return EXIT_FAILURE;
  }

  // make sure the program is there
  char nextChar;
  while ((nextChar = getc(stdin)) != EOF)  {
    printf("%c", nextChar);
  }

  

  return EXIT_SUCCESS;
}
