

// @author dp98 lol



#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

#include "string.h"
#include "error_code.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum {
  SCANNER_START,
  SCANNER_NUMBER,
  SCANNER_ID,
  SCANNER_DECIMAL,
  SCANNER_COMMENT,
  SCANNER_COMMENT_1,
  SCANNER_COMMENT_0,
  SCANNER_COMMENT_01,
  SCANNER_COMMENT_02,
  SCANNER_LINE_COMMENT,
  SCANNER_EXPONENT_1,
  SCANNER_DIVISION,
  SCANNER_EQUAL,
  SCANNER_SMALLERTHEN,
  SCANNER_BIGGERTHEN,
  SCANNER_NEG,
  SCANNER_STRING,
  SCANNER_STRING1,
  SCANNER_STRING2,
  SCANNER_STRING3
} scanner_state;

typedef enum {
  TOKEN_UNDEF,
  TOKEN_EOF,
  TOKEN_ID,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_INDENT,
  TOKEN_DEDENT,
  TOKEN_ADDITION,
  TOKEN_SUBTRACTION,
  TOKEN_MULTIPLICATION,
  TOKEN_DIVISION,
  TOKEN_INTEGER_DIVISION,
  TOKEN_EQUAL,
  TOKEN_NEG_EQUAL,
  TOKEN_EQUAL_EQUAL,
  TOKEN_BIGGERTHEN,
  TOKEN_SMALLERTHEN,
  TOKEN_SMALLERTHEN_EQUAL,
  TOKEN_BIGGERTHEN_EQUAL,
  TOKEN_STRING
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

Token getNextToken(bool *line_flag);

#endif
