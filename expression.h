#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdbool.h>

#include "scanner.h"
#include "exp_stack.h"
#include "error_code.h"
#include "string.h"
#include "symtable.h"
#include "parser.h"


#define PT_SIZE 18
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
      EXP_FUNCTION,
      EXP_COMMA,
      EXP_DOLLAR,
      EXP_OTHER

  }exp_type;

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

int expression(); /*,int expectedValue*/
int expressionAnalysis();
char getSignFromTable();
Exp_element *newElement(int type,bool handle);
int get_stack_type(ptrStack *stack_expression);
Exp_element *tokentoExp_element(Token token,bool handle);
int convertTokenToIndex(Token token);
int useRule(ptrStack *stack_expression);


#endif //EXPRESSION_H
