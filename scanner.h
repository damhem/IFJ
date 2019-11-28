

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
  SCANNER_START,
  SCANNER_NUMBER,
  SCANNER_ID,
  SCANNER_DECIMAL_1,
  SCANNER_DECIMAL_2,
  SCANNER_COMMENT,
  SCANNER_COMMENT_1,
  SCANNER_COMMENT_0,
  SCANNER_COMMENT_01,
  SCANNER_COMMENT_02,
  SCANNER_LINE_COMMENT,
  SCANNER_EXPONENT_1,
  SCANNER_EXPONENT_2,
  SCANNER_EXPONENT_3,
  SCANNER_ZERO,
  SCANNER_DIVISION,
  SCANNER_EQUAL,
  SCANNER_SMALLERTHEN,
  SCANNER_BIGGERTHEN,
  SCANNER_NEG,
  SCANNER_STRING,
  SCANNER_STRING1,
  SCANNER_STRING2,
  SCANNER_STRING3,
  SCANNER_DENTCOUNT,
  SCANNER_EOL,
} scanner_state;

typedef enum {
  TOKEN_MULTIPLICATION,     //0
  TOKEN_DIVISION,           //1
  TOKEN_INTEGER_DIVISION,   //2
  TOKEN_ADDITION,           //3
  TOKEN_SUBTRACTION,        //4
  TOKEN_EQUAL,              //5
  TOKEN_NEG_EQUAL,          //6
  TOKEN_SMALLERTHEN,        //7
  TOKEN_SMALLERTHEN_EQUAL,  //8
  TOKEN_BIGGERTHEN,         //9
  TOKEN_BIGGERTHEN_EQUAL,   //10
  TOKEN_EQUAL_EQUAL,        //11
  TOKEN_LEFTPAR,            //12
  TOKEN_RIGHTPAR,           //13
  TOKEN_STRING,             //14
  TOKEN_INT,                //15
  TOKEN_DOUBLE,             //16
  TOKEN_ID,                 //17
  TOKEN_EOF,                //18
  TOKEN_COMMA,              //19
  TOKEN_UNDEF,              //20

  TOKEN_PLUS,               //21
  TOKEN_MINUS,              //22
  TOKEN_INDENT,             //23
  TOKEN_DEDENT,             //24
  TOKEN_EOL,                //25
  TOKEN_DEF,                //26
  TOKEN_ELSE,               //27
  TOKEN_IF,                 //28
  TOKEN_NONE,               //29
  TOKEN_PASS,               //30
  TOKEN_RETURN,             //31
  TOKEN_WHILE,              //32
  TOKEN_DOUBLEDOT,          //33
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
