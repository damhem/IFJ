/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file main.c
*
* @author Daniel Pátek (xpatek08)
* @author Daniel Čechák (xcecha06)
* @author
* @author
*
*/

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main() {

  FILE *fp;

  if ((fp = fopen ("program.txt","r")) == NULL) {
      return EXIT_FAILURE;
  }



  printf("ahoj\n");

  return EXIT_SUCCESS;
}
<<<<<<< HEAD
=======
ahoj vole vv
>>>>>>> 97c559ba76bccba82cdcca6785f6845c3037f9cf
