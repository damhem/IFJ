/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file parser.h
*
* @author Daniel Pátek (xpatek08)
*/

#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#include "error_code.h"
#include "scanner.h"
#include "string.h"
#include "expression.h"
#include "exp_stack.h"
#include "stack.h"
#include "generator.h"
#include "linear_list.h"
#include "symtable.h"

#include <stdbool.h>

//actual token
Token token;

//one token ahead
Token peekToken;

//scanner stack
tStack s;

//expression stack
ptrStack stack_expression;

//line variable for scanner
bool line_flag;

//bool var to check when we expect a docstring from scanner
bool nowExpression;

//var for actual function name
string functionName;

//index counter for params in function
int paramIndex;

//counter for generating labels
int labelCounter;

//global symtable
symtable glSymtable;

//local symtable
symtable lcSymtable;

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
ERROR_CODE commands();
ERROR_CODE checkDefinedFunctions(struct tBSTNode* Root);

#endif
