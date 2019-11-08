

// @author dp98 lol



#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

#include "string.h"
#include "error_code.h"

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

typedef enum {
  SCANNER_START,
  SCANNER_NUMBER,
  SCANNER_ID,
  SCANNER_DECIMAL,
  SCANNER_EXPONENT_1
} scanner_state;

typedef enum {
  TOKEN_UNDEF,
  TOKEN_EOF,
  TOKEN_ID,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_INDENT,
  TOKEN_DEDENT
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
