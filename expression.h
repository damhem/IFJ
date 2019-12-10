#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdbool.h>

#include "scanner.h"
#include "exp_stack.h"
#include "error_code.h"
#include "string.h"
#include "symtable.h"
#include "parser.h"

//velikost precedencni tabulky
#define PT_SIZE 20

typedef union element_data {
  string ID;
  int integer;
  double decimal;
} element_data;

//Struktura reprezentující (ne)terminál na zásobníku
typedef struct exp_element {
    element_data e_data;
    token_type type;
    bool handle;    //Handle pro pomoc při redukci stacku
    bool terminal;
} Exp_element;

//first terminal on stack
ptStack* firstTerm;

//return value int
VarType retVal;

int expression(VarType* returnValue); /*,int expectedValue*/
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
