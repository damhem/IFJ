

// @author dp98 lol



#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

#include "string.h"
#include "error_code.h"
#include "stack.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum {
  TOKEN_UNDEF,              //0
  TOKEN_EOF,                //1
  TOKEN_ID,                 //2
  TOKEN_PLUS,               //3
  TOKEN_MINUS,              //4
  TOKEN_INDENT,             //5
  TOKEN_DEDENT,             //6
  TOKEN_ADDITION,           //7
  TOKEN_SUBTRACTION,        //8
  TOKEN_MULTIPLICATION,     //9
  TOKEN_DIVISION,           //10 // //
  TOKEN_INTEGER_DIVISION,   //11
  TOKEN_EQUAL,              //12
  TOKEN_NEG_EQUAL,          //13
  TOKEN_EQUAL_EQUAL,        //14
  TOKEN_BIGGERTHEN,         //15
  TOKEN_SMALLERTHEN,        //16
  TOKEN_SMALLERTHEN_EQUAL,  //17
  TOKEN_BIGGERTHEN_EQUAL,   //18
  TOKEN_STRING,             //19
  TOKEN_EOL,                //20 // //
  TOKEN_DEF,                //21
  TOKEN_ELSE,               //22
  TOKEN_IF,                 //23
  TOKEN_NONE,               //24
  TOKEN_PASS,               //25
  TOKEN_RETURN,             //26
  TOKEN_WHILE,              //27
  TOKEN_LEFTPAR,            //28
  TOKEN_RIGHTPAR,           //29
  TOKEN_DOUBLEDOT,          //30 // //
  TOKEN_INT,                //31
  TOKEN_DOUBLE,             //32
  TOKEN_COMMA               //33
} token_type;

typedef union token_data {
  string ID;
  int integer;
  double decimal;
} token_data;

typedef struct Token {
  token_data t_data;
  token_type t_type;
} Token;

int dentcount;

Token getNextToken(bool *line_flag, tStack *s);
token_type peekNextToken();

#endif
