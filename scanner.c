
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
        else if (c == '"'){
          state = SCANNER_COMMENT;
        }
        else if ( c == '#'){
          state = SCANNER_LINE_COMMENT;
        }
        else if ( c == '+'){
          token.t_type = TOKEN_ADDITION;
          return token;
        }
        else if ( c == '-'){
          token.t_type = TOKEN_SUBTRACTION;
          return token;
        }
        else if ( c == '*'){
          token.t_type = TOKEN_MULTIPLICATION;
          return token;
        }
        else if ( c == '/'){
          state= SCANNER_DIVISION;
          token.t_type = TOKEN_DIVISION;
        }
        else if ( c == '='){
          state= SCANNER_EQUAL;
          token.t_type = TOKEN_EQUAL;
        }
        else if ( c == '<'){
          state= SCANNER_SMALLERTHEN;
          token.t_type = TOKEN_SMALLERTHEN;
        }
        else if ( c == '>'){
          state= SCANNER_BIGGERTHEN;
          token.t_type = TOKEN_BIGGERTHEN;
        }
        else if ( c == '!'){
          state= SCANNER_NEG;
        }
        break;
      case (SCANNER_ID):
        if (isalpha(c) || isdigit(c) || c == '_') {
          stringAddChar(&token.t_data.ID, c);
        }else {
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
      case (SCANNER_COMMENT):
          if (c == '"'){                      //2 "
            state = SCANNER_COMMENT_1;
        }else{
          ungetc(c, stdin);
        }
        break;
      case (SCANNER_COMMENT_1):
        if (c == '"'){                       //3"
          state = SCANNER_COMMENT_0;
        }else{
          ungetc(c, stdin);
          ungetc(c, stdin);
        }
        break;
      case (SCANNER_COMMENT_0):
        if (c == '"'){                        //1"
          state = SCANNER_COMMENT_01;
        }
        break;
      case (SCANNER_COMMENT_01):
        if (c == '"'){                        //2"
          state = SCANNER_COMMENT_02;
        }else{
          state = SCANNER_COMMENT_0;
        }
        break;
      case (SCANNER_COMMENT_02):
        if (c == '"'){                       //3"
          state = SCANNER_START;
        }else{
          state = SCANNER_COMMENT_0;
        }
        break;
      case (SCANNER_LINE_COMMENT):
        if (c != EOF){
          state = SCANNER_LINE_COMMENT;
        }else{
          token.t_type = TOKEN_EOF;
					return token;
        }
        break;
      case (SCANNER_DIVISION):
      if (c == '/'){
        token.t_type = TOKEN_INTEGER_DIVISION;
        return token;
      }else{
        ungetc(c, stdin);
        return token;
      }
      case (SCANNER_EQUAL):
        if (c == '='){
          token.t_type = TOKEN_EQUAL_EQUAL;
          return token;
        }else{
          ungetc(c, stdin);
          return token;
        }
      case(SCANNER_SMALLERTHEN):
        if (c == '='){
          token.t_type = TOKEN_SMALLERTHEN_EQUAL;
          return token;
        }else{
          ungetc(c, stdin);
          return token;
        }
      case(SCANNER_BIGGERTHEN):
        if (c == '='){
          token.t_type = TOKEN_BIGGERTHEN_EQUAL;
          return token;
        }else{
          ungetc(c, stdin);
          return token;
        }
      case(SCANNER_NEG):
        if (c == '='){
          token.t_type = TOKEN_NEG_EQUAL;
          return token;
        }else{
          ungetc(c, stdin);
        }
        break;
      default:
        token.t_type = TOKEN_UNDEF;
        return token;
    } //switch
  } //while
  return token;
} //getNextToken
