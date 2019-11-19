

#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#include "error_code.h"
#include "scanner.h"

#include <stdbool.h>

//actual token
Token token;

//one token ahead
Token peekToken;

//scanner stack
tStack s;

//line variable for scanner
bool line_flag;

//var for actual function name
string functionName;

//index counter for params in function
int paramIndex;

//function list
int parse();
int program();
int programBody();
int functionHead();
int functionParam();
int functionDef();
int functionBody();
int prikaz();
int command();
int functionParam();
int nextFunctionParam();
int continueID();

#endif
