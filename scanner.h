

// @author dp98



#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

#include "string.h"

typedef enum {
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_INDENT,
  TOKEN_DEDENT
} token_type;

typedef struct {
  string t_data;
  token_type t_type;
} Token;

#endif
