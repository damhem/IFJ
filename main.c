/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file main.c
*
* @author Daniel Pátek (xpatek08)
* @author Daniel Čechák (xcecha06)
* @author Zdeněk Kroča (xkroca02)
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
ahoj vole vv hghhgg
>>>>>>> c43815bbb9146daaaa7b7569f93d953137d565a4
>>>>>>> 34647fb712a38e888e1fa92014c859c394e75ff1
