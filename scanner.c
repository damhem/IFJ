
#include "scanner.h"

Token getNextToken(bool *line_flag/*stack *indent_stack*/) {
  bool run = true;
  //bool line_flag = true;
  Token token;
  token.t_type = TOKEN_UNDEF;

  int state = SCANNER_START;
  char c;

  while(run) {

    c = (char) getc(stdin);

    switch (state) {
      case (SCANNER_START):
        if (c == EOF) {
					token.t_type = TOKEN_EOF;
					return token;
				}
        else if (stringInit(&token.t_data.ID) == ERROR_CODE_INTERNAL) {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_INTERNAL;
          return token;
        }
        if (isalpha(c) || c == '_') {
          state = SCANNER_ID;
          token.t_type = TOKEN_ID;
          stringAddChar(&token.t_data.ID, c);
        }
        else if (isdigit(c)) {
          state = SCANNER_NUMBER;
          stringAddChar(&token.t_data.ID, c);
        }
        break;
      case (SCANNER_ID):
        if (isalpha(c) || isdigit(c) || c == '_') {
          stringAddChar(&token.t_data.ID, c);
        }
        else {
          ungetc(c, stdin);
          return token;
        }
        break;
      case (SCANNER_NUMBER):
        if (isdigit(c)) {
          stringAddChar(&token.t_data.ID, c);
        }
        else if (c == '.') {
          state = SCANNER_DECIMAL;
          stringAddChar(&token.t_data.ID, c);
        }
        else if (c == 'e' || c == 'E') {
          state = SCANNER_EXPONENT_1;
          stringAddChar(&token.t_data.ID, c);
        }
        break;
      default:
        token.t_type = TOKEN_UNDEF;
        return token;
    } //switch
  } //while
  return token;
} //getNextToken
