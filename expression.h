#ifndef EXPRESSION_H
#define EXPRESSION_H


#include "scanner.h"
#include "exp_stack.h"
#include "stack.h"
#include "error_code.h"
#include "string.h"
#include "symtable.h"
#include "parser.h"


#define PT_SIZE 16
typedef enum {
      EXP_MULTIPLY,
      EXP_DIVIDE,
      EXP_INTEGER_DIVIDE,
      EXP_PLUS,
      EXP_MINUS,

      EXP_EQUAL,
      EXP_NOT_EQUAL,
      EXP_LESS,
      EXP_LESS_EQUAL,
      EXP_MORE,
      EXP_MORE_EQUAL,
      EXP_COMPARISON,

      EXP_LEFT_PAR,
      EXP_RIGHT_PAR,

      EXP_OPERAND,
      EXP_DOLLAR,
      EXP_OTHER

  }exp_type;

//Struktura reprezentující (ne)terminál na zásobníku
typedef struct exp_element{
    exp_type type;
    bool handle;    //Handle pro pomoc při redukci stacku
}Exp_element;

int expression(); /*,int expectedValue*/
int expressionAnalysis(ptrStack *stack_expression);
char getSignFromTable();
int initexpressionStack(ptrStack*);
Exp_element *newElement(int type,bool handle);
int get_stack_type(ptrStack *stack_expression);
Exp_element *tokentoExp_element(Token token,bool handle);
int convertTokenToIndex(Token token);
/*int useRule(ptrStack *stack_expression);*/

#endif //EXPRESSION_H
