#ifndef IFJ_STACK_H
#define IFJ_STACK_H

#include <stdio.h>
#include "error_code.h"


#define MAX_STACK 100

int STACK_SIZE;

typedef struct
{
	char arr[MAX_STACK];          /* pole pro uložení hodnot */
	int top;                      /* index prvku na vrcholu zásobníku */
} tStack;


/* Hlavičky funkcí pro práci se zásobníkem. */

void stackError(int error_code);
ERROR_CODE stackInit(tStack* s);
int stackEmpty(const tStack* s);
int stackFull(const tStack* s);
ERROR_CODE stackTop(const tStack* s, char* c);
void stackPop(tStack* s);
ERROR_CODE stackPush(tStack* s, char c);
void stackClear(tStack* s);

#endif
