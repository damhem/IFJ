/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file exp_stack.h
*
* @author Daniel Čechák (xcecha06)
*/

#ifndef EXP_STACK_H
#define EXP_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "error_code.h"
#include "scanner.h"


// Struktura prvku na zásobníku
typedef struct ptstack_structure{
    struct ptstack_structure *left;
    struct exp_element *value;
} ptStack;

// Struktura zásobníku
typedef struct {
    ptStack *top_of_stack;
} ptrStack;

// Deklarace funkcí pro zásobník
ERROR_CODE exp_stackInit(ptrStack *);
ERROR_CODE exp_stackPush(ptrStack *, void *);
void exp_stackPop(ptrStack *);
//Token exp_stackTop(ptrStack *);
bool exp_stackEmpty(ptrStack *);
bool exp_stackClear(ptrStack *);

#include "expression.h"

#endif //EXP_STACK_H
