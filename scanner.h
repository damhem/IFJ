

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
  //Rozšíření BASE
  SCANNER_BIN,
  SCANNER_OCT,
  SCANNER_HEX,
  SCANNER_BIN_UNDERSCORE,
  SCANNER_OCT_UNDERSCORE,
  SCANNER_HEX_UNDERSCORE,
  //
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
  TOKEN_MULTIPLICATION,
  TOKEN_DIVISION,
  TOKEN_INTEGER_DIVISION,
  TOKEN_ADDITION,
  TOKEN_SUBTRACTION,
  TOKEN_EQUAL,              //12
  TOKEN_NEG_EQUAL,
  TOKEN_SMALLERTHEN,        //16
  TOKEN_SMALLERTHEN_EQUAL,  //17
  TOKEN_BIGGERTHEN,         //15
  TOKEN_BIGGERTHEN_EQUAL,   //18
  TOKEN_EQUAL_EQUAL,        //14
  TOKEN_LEFTPAR,            //28
  TOKEN_RIGHTPAR,           //29      //2
  TOKEN_STRING,
  TOKEN_INT,             //19
  TOKEN_DOUBLE,             //32
  TOKEN_ID,
  TOKEN_EOF,                //1                //31
  TOKEN_COMMA,               //33
  TOKEN_UNDEF,              //0

  TOKEN_PLUS,               //3
  TOKEN_MINUS,              //4
  TOKEN_INDENT,             //5
  TOKEN_DEDENT,             //6
  TOKEN_EOL,                //20 // //
  TOKEN_DEF,                //21
  TOKEN_ELSE,               //22
  TOKEN_IF,                 //23
  TOKEN_NONE,               //24
  TOKEN_PASS,               //25
  TOKEN_RETURN,             //26
  TOKEN_WHILE,              //27
  TOKEN_DOUBLEDOT,          //30
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
