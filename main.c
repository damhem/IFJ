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

#include "error_code.h"
#include "parser.h"
#include "linear_list.h"
#include "generator.h"
#include "scanner.h"
#include "string.h"
#include "stack.h"

int main() {

  ERROR_CODE error_code = EXIT_SUCCESS;

  DLInitList(&instrList);

  error_code = parse();
  if (error_code == ERROR_CODE_OK) printInstructionList(&instrList);

  DLDisposeList(&instrList);
  return error_code;
}
