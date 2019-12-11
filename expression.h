/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file expression.h
*
* @author Daniel Pátek (xpatek08)
* @author Daniel Čechák (xcecha06)
*/

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdbool.h>

#include "scanner.h"
#include "exp_stack.h"
#include "error_code.h"
#include "string.h"
#include "symtable.h"
#include "parser.h"

//Size of the precedence table
#define PT_SIZE 20

typedef union element_data {
  string ID;
  int integer;
  double decimal;
} element_data;

//Structure representing (not)terminal on stack
typedef struct exp_element {
    element_data e_data;
    token_type type;
    bool handle;    //Handle for help with reduction of the expression stack
    bool terminal;
} Exp_element;

//first terminal on stack
ptStack* firstTerm;

//return value int
VarType retVal;

// Declaration of funcition for expression
int expression(VarType* returnValue); 
int expressionAnalysis();
char getSignFromTable();
Exp_element *newElement(int type,bool handle);
int get_stack_type(ptrStack *stack_expression);
Exp_element *tokentoExp_element(Token token,bool handle);
int convertTokenToIndex(Token token);
ERROR_CODE useRule(ptrStack *stack_expression);
ERROR_CODE reducePars(ptrStack *stack_expression);
ERROR_CODE makeIdInstr();
ERROR_CODE makeFunction();
ERROR_CODE makePrintFunction();
ERROR_CODE makeLenFunction();
ERROR_CODE makeChrFunction();
ERROR_CODE makeOrdFunction();
ERROR_CODE makeSubstrFunction();
ERROR_CODE checkOperands(token_type rightResultType, Exp_element* element1, Exp_element* element2);


#endif //EXPRESSION_H
