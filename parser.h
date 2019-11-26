

#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#include "error_code.h"
#include "scanner.h"
#include "string.h"
#include "expression.h"
#include "exp_stack.h"
#include "generator.h"
#include "linear_list.h"

#include <stdbool.h>

//actual token
Token token;

//one token ahead
Token peekToken;

//scanner stack
tStack s;
ptrStack stack_expression;
//line variable for scanner
bool line_flag;

//var for actual function name
string functionName;

//index counter for params in function
int paramIndex;

//instruction list
instruction_list instrList;

//function list
ERROR_CODE parse();
ERROR_CODE program();
ERROR_CODE programBody();
ERROR_CODE functionHead();
ERROR_CODE functionParam();
ERROR_CODE functionDef();
ERROR_CODE functionBody();
ERROR_CODE command();
ERROR_CODE functionParam();
ERROR_CODE nextFunctionParam();

ERROR_CODE continueID();
ERROR_CODE terms();
ERROR_CODE nextTerms();

ERROR_CODE commands();

#endif
