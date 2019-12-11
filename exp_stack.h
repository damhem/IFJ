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


// Strucure of the element on stack 
typedef struct ptstack_structure{
    struct ptstack_structure *left;
    struct exp_element *value;
} ptStack;

// Strucure of the stack
typedef struct {
    ptStack *top_of_stack;
} ptrStack;

// Declaration of funcition for stack
ERROR_CODE exp_stackInit(ptrStack *);
ERROR_CODE exp_stackPush(ptrStack *, void *);
void exp_stackPop(ptrStack *);
bool exp_stackEmpty(ptrStack *);
bool exp_stackClear(ptrStack *);

#include "expression.h"

#endif
