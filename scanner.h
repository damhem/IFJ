/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file scanner.h
*
* @author Daniel Pátek (xpatek08)
* @author Daniel Čechák (xcecha06)
* @author Zdeněk Kroča (xkroca02)
*/


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

// individual states in scanner
typedef enum {
  SCANNER_START,
  SCANNER_NUMBER,
  SCANNER_ID,
  SCANNER_DECIMAL_1,
  SCANNER_DECIMAL_2,
  SCANNER_COMMENT_1,
  SCANNER_COMMENT_2,
  SCANNER_COMMENT_3,
  SCANNER_COMMENT_4,
  SCANNER_COMMENT_5,
  SCANNER_LINE_COMMENT,
  SCANNER_EXPONENT_1,
  SCANNER_EXPONENT_2,
  SCANNER_EXPONENT_3,
  SCANNER_ZERO,

  // Expansion BASE
  SCANNER_BIN_1,
  SCANNER_OCT_1,
  SCANNER_HEX_1,
  SCANNER_BIN_2,
  SCANNER_OCT_2,
  SCANNER_HEX_2,
  SCANNER_BIN_UNDERSCORE,
  SCANNER_OCT_UNDERSCORE,
  SCANNER_HEX_UNDERSCORE,
  SCANNER_INT_UNDERSCORE,
  SCANNER_DECIMAL_UNDERSCORE,
  SCANNER_EXPONENT_UNDERSCORE,

  SCANNER_DIVISION,
  SCANNER_EQUAL,
  SCANNER_SMALLERTHAN,
  SCANNER_BIGGERTHAN,
  SCANNER_NEG,
  SCANNER_STRING_1,
  SCANNER_STRING_2,
  SCANNER_STRING_3,
  SCANNER_STRING_4,
  SCANNER_DENTCOUNT,
  SCANNER_EOL,
} scanner_state;

// individual token types
typedef enum {
  TOKEN_MULTIPLICATION,     //0
  TOKEN_DIVISION,           //1
  TOKEN_INTEGER_DIVISION,   //2
  TOKEN_ADDITION,           //3
  TOKEN_SUBTRACTION,        //4
  TOKEN_EQUAL,              //5
  TOKEN_NEG_EQUAL,          //6
  TOKEN_SMALLERTHAN,        //7
  TOKEN_SMALLERTHAN_EQUAL,  //8
  TOKEN_BIGGERTHAN,         //9
  TOKEN_BIGGERTHAN_EQUAL,   //10
  TOKEN_EQUAL_EQUAL,        //11
  TOKEN_LEFTPAR,            //12
  TOKEN_RIGHTPAR,           //13
  TOKEN_STRING,             //14
  TOKEN_INT,                //15
  TOKEN_DOUBLE,             //16
  TOKEN_ID,                 //17
  TOKEN_NONE,               //18
  TOKEN_UNDEF,              //19
  
  TOKEN_INDENT,             //21
  TOKEN_DEDENT,             //22
  TOKEN_EOL,                //23
  TOKEN_DEF,                //24
  TOKEN_ELSE,               //25
  TOKEN_IF,                 //26
  TOKEN_COMMA,               //27
  TOKEN_PASS,               //28
  TOKEN_RETURN,             //29
  TOKEN_WHILE,              //30
  TOKEN_DOUBLEDOT,          //31
  TOKEN_EOF                 //32
} token_type;

// union for storing useful data of a token
typedef union token_data {
  string ID;
  int integer;
  double decimal;
} token_data;

// structure for storing basic informations about each token
typedef struct Token {
  token_data t_data;
  token_type t_type;
} Token;

int dentcount; // helps with multiple DEDENT tokens generation
bool endoffile; // helps with generation of extra EOL token before EOF token for correct syntax

// scanner functions
Token getNextToken(bool *line_flag, tStack *s);
token_type peekNextToken();

#endif
