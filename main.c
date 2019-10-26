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

#include "parser.h"

int main() {

  FILE *fp;

  if ((fp = fopen ("program.txt","r")) == NULL) {
      return EXIT_FAILURE;
  }



  printf("ahojky debilci\n");

  return EXIT_SUCCESS;
