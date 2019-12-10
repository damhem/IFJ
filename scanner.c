
#include "scanner.h"
#include "parser.h"

// checks the next token
token_type peekNextToken() {

  if (peekToken.t_type != TOKEN_UNDEF) return peekToken.t_type;

  peekToken = getNextToken(&line_flag, &s);

  return peekToken.t_type;
}

Token getNextToken(bool *line_flag, tStack *s) {

  // checks if some token isnt already read
  if (peekToken.t_type != TOKEN_UNDEF) {
    Token peekThisToken = peekToken;
    peekToken.t_type = TOKEN_UNDEF;
    return peekThisToken;
  }

  // initialization of the first state and token before reading first char
  Token token;
  token.t_type = TOKEN_UNDEF;
  int state = SCANNER_START;

  char c; // actual char in stdin
  char prev_c; // helps with hexadecimal chars in strings
  int spacecount = 0; // helps counting spaces for indentation

  // sorts chars from the source file
  while(true) {

    // generates dedent tokens if more than 1 are needed
    if (dentcount>0) {
      dentcount--;
      token.t_type = TOKEN_DEDENT;
      return token;
    }

    c = (char) getc(stdin);

    switch (state) {

      case (SCANNER_START):

        if (c == EOF) {

          // generates extra EOL token before the end of file for correct syntax
          if (!endoffile) {
            ungetc(c, stdin);
            endoffile = true;
            token.t_type = TOKEN_EOL;
  					return token;
          }
          // generates dedent tokens until the indentation stack is empty
          else if (stackEmpty(s)==0) {
            stackPop(s);
            ungetc(c, stdin);
            token.t_type = TOKEN_DEDENT;
            return token;
          }
					token.t_type = TOKEN_EOF;
					return token;
				}

        // returns token with error code 99 if it is unable to initialize string for token.t_data.ID
        else if (stringInit(&token.t_data.ID) == ERROR_CODE_INTERNAL) {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_INTERNAL;
          return token;
        }


        else if (c == '\n'){

          // looks at the next char
          c = (char) getc(stdin);

          // returns error code 1 if tab is used for indentation
          if (c == '\t') {
            token.t_type = TOKEN_UNDEF;
            token.t_data.integer = ERROR_CODE_LEX;
            return token;
          }
          // ignores next line if it is line comment
          if (c == '#') {
            *line_flag=false;
            state = SCANNER_LINE_COMMENT;
            break;
          }

          // increments dentcount for dedent token generation if the next line doesn't start with space
          else if (!isspace(c)) {
            while (stackEmpty(s)==0) {
            stackPop(s);
            dentcount++;
            }
          }
          ungetc(c, stdin);
          *line_flag=true;
          token.t_type = TOKEN_EOL;
          return token;
        }

        // starts counting spaces when the line flag is true
        else if ((c == ' ' )&& (*line_flag) == true) {
          state = SCANNER_DENTCOUNT;
          spacecount++;
        }

        // zero has special state for BASE expansion and possible redundant zeros in number
        else if (c == '0') {
          *line_flag=false;
          state = SCANNER_ZERO;
          stringAddChar(&token.t_data.ID, c);
        }

        else if (isalpha(c) || c == '_') {
          *line_flag=false;
          state = SCANNER_ID;
          token.t_type = TOKEN_ID;
          stringAddChar(&token.t_data.ID, c);
        }

        else if (isdigit(c)) {
          *line_flag=false;
          state = SCANNER_NUMBER;
          stringAddChar(&token.t_data.ID, c);
        }

        else if (c == '"') {
          *line_flag=false;
          token.t_type = TOKEN_STRING;
          state = SCANNER_COMMENT_1;
        }

        else if ( c == '#'){
          *line_flag=false;
          state = SCANNER_LINE_COMMENT;
        }

        else if ( c == '+'){
          *line_flag=false;
          token.t_type = TOKEN_ADDITION;
          return token;
        }

        else if ( c == '-'){
          *line_flag=false;
          token.t_type = TOKEN_SUBTRACTION;
          return token;
        }

        else if ( c == '*'){
          *line_flag=false;
          token.t_type = TOKEN_MULTIPLICATION;
          return token;
        }

        else if ( c == '/'){
          *line_flag=false;
          state= SCANNER_DIVISION;
          token.t_type = TOKEN_DIVISION;
        }

        else if ( c == '='){
          *line_flag=false;
          state= SCANNER_EQUAL;
          token.t_type = TOKEN_EQUAL;
        }

        else if ( c == '<'){
          *line_flag=false;
          state= SCANNER_SMALLERTHEN;
          token.t_type = TOKEN_SMALLERTHEN;
        }

        else if ( c == '>'){
          *line_flag=false;
          state= SCANNER_BIGGERTHEN;
          token.t_type = TOKEN_BIGGERTHEN;
        }

        else if ( c == '!'){
          *line_flag=false;
          state = SCANNER_NEG;
        }

        else if ( c == '\''){
          *line_flag=false;
          state = SCANNER_STRING_1;
        }

        else if ( c == '('){
          *line_flag=false;
          token.t_type = TOKEN_LEFTPAR;
          return token;
        }

        else if ( c == ')'){
          *line_flag=false;
          token.t_type = TOKEN_RIGHTPAR;
          return token;
        }

        else if ( c == ':'){
          *line_flag=false;
          token.t_type = TOKEN_DOUBLEDOT;
          return token;
        }

        else if ( c == ','){
          *line_flag=false;
          token.t_type = TOKEN_COMMA;
          return token;
        }

        // ignores white spaces in between lexems
        else if (isspace(c)){
          ;
        }

        // returns error code 1 for unknown symbols
        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }
        break;

      case (SCANNER_ID):

        if (isalpha(c) || isdigit(c) || c == '_') {
          stringAddChar(&token.t_data.ID, c);
        }
        else {
          ungetc(c, stdin);
          //checking for keywords in identificators
          if (strcmp("def",token.t_data.ID.value) == 0) {
            token.t_type = TOKEN_DEF;
          }
          else if (strcmp("else",token.t_data.ID.value) == 0) {
            token.t_type = TOKEN_ELSE;
          }
          else if (strcmp("if",token.t_data.ID.value) == 0) {
            token.t_type = TOKEN_IF;
          }
          else if (strcmp("None",token.t_data.ID.value) == 0) {
            token.t_type = TOKEN_NONE;
          }
          else if (strcmp("pass",token.t_data.ID.value) == 0) {
            token.t_type = TOKEN_PASS;
          }
          else if (strcmp("return",token.t_data.ID.value) == 0) {
            token.t_type = TOKEN_RETURN;
          }
          else if (strcmp("while",token.t_data.ID.value) == 0) {
            token.t_type = TOKEN_WHILE;
          }

          return token;
        }

        break;

      case (SCANNER_DENTCOUNT):

        // ignores indentation in lines with line comment
        if ( c == '#'){
          *line_flag=false;
          state = SCANNER_LINE_COMMENT;
        }

        // count spaces for indentation
        if (c == ' ') {
          spacecount++;
        }

        // when all of the spaces are counted, generate correct token
        else {
          *line_flag=false;
          ungetc(c, stdin);

          char stack_top; // number of spaces on the top of the indentation stack
          stackTop(s,&stack_top);

          // if the number of spaces is greater than top of the stack, generate indent token
          if (spacecount>stack_top) {
            if (stackPush(s,spacecount) == ERROR_CODE_INTERNAL) {
              token.t_type = TOKEN_UNDEF;
              token.t_data.integer = ERROR_CODE_INTERNAL;
              return token;
            }
            token.t_type = TOKEN_INDENT;
            spacecount=0;
            return token;
          }

          // if the number of spaces is equal to top of the stack, don't generate any token
          else if (spacecount==stack_top) {
            spacecount=0;
            state = SCANNER_START;
          }

          // if the number of spaces is lower than top of the stack, generate dedent token
          else if (spacecount<stack_top) {
            // pops stack until it is empty or the top of it is same as the number of spaces
            while (spacecount<stack_top && stackEmpty(s)==0) {
              dentcount++;
              stackPop(s);
              stackTop(s,&stack_top);
              // returns error code 1 if the indentation is incorrect
              // (number of spaces is greater than top of the stack, after stackPop)
              if (spacecount>stack_top) {
                token.t_type = TOKEN_UNDEF;
                token.t_data.integer = ERROR_CODE_LEX;
                return token;
              }

              if (spacecount==stack_top || stackEmpty(s) != 0) {
                token.t_type = TOKEN_DEDENT;
                spacecount=0;
                dentcount--;
                return token;
              }
            }
          }
        }
        break;

      case (SCANNER_ZERO):

        // returns error code 1 if there there are redundant zeros in number
        if (c == '0' || isdigit(c)) {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        else if (c == '.') {
          state = SCANNER_DECIMAL_1;
          stringAddChar(&token.t_data.ID, c);
        }

        else if (c == 'e' || c == 'E') {
          state = SCANNER_EXPONENT_1;
          stringAddChar(&token.t_data.ID, c);
        }

        // sorting for BASE expansion
        else if (c == 'b' || c == 'B') {
          state = SCANNER_BIN_1;
          stringClear(&token.t_data.ID);
        }

        else if (c == 'o' || c == 'O') {
          state = SCANNER_OCT_1;
          stringClear(&token.t_data.ID);
        }

        else if (c == 'x' || c == 'X') {
          state = SCANNER_HEX_1;
          stringClear(&token.t_data.ID);
        }

        else {
          token.t_data.integer = 0;
          token.t_type = TOKEN_INT;
          ungetc(c, stdin);
          return token;
        }

        break;

      case (SCANNER_BIN_1):

        if (c == '_') {
          state = SCANNER_BIN_UNDERSCORE;
        }

        else if (c == '0' || c == '1') {
          state = SCANNER_BIN_2;
          stringAddChar(&token.t_data.ID, c);
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_OCT_1):

        if (c == '_') {
          state = SCANNER_OCT_UNDERSCORE;
        }

        else if (c>='0' && c<='7') {
          state = SCANNER_OCT_2;
          stringAddChar(&token.t_data.ID, c);
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_HEX_1):

        if (c == '_') {
          state = SCANNER_HEX_UNDERSCORE;
        }

        else if ((c>='a' && c<='f') || (c>='A' && c<='F') || isdigit(c)) {
          state = SCANNER_HEX_2;
          stringAddChar(&token.t_data.ID, c);
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_BIN_2):

        if (c == '_') {
          state = SCANNER_BIN_UNDERSCORE;
        }

        else if (c == '0' || c == '1') {
          stringAddChar(&token.t_data.ID, c);
        }

        else {
          unsigned int temp = strtol(token.t_data.ID.value, NULL, 2);
          token.t_data.integer = temp;
          token.t_type = TOKEN_INT;
          ungetc(c, stdin);
          return token;
        }

        break;

      case (SCANNER_OCT_2):

        if (c == '_') {
          state = SCANNER_OCT_UNDERSCORE;
        }

        else if (c>='0' && c<='7') {
          stringAddChar(&token.t_data.ID, c);
        }

        else {
          unsigned int temp = strtol(token.t_data.ID.value, NULL, 8);
          token.t_data.integer = temp;
          token.t_type = TOKEN_INT;
          ungetc(c, stdin);
          return token;
        }

        break;

      case (SCANNER_HEX_2):

        if (c == '_') {
          state = SCANNER_HEX_UNDERSCORE;
        }

        else if ((c>='a' && c<='f') || (c>='A' && c<='F') || isdigit(c)) {
          stringAddChar(&token.t_data.ID, c);
        }

        else {
          unsigned int temp = strtol(token.t_data.ID.value, NULL, 16);
          token.t_data.integer = temp;
          token.t_type = TOKEN_INT;
          ungetc(c, stdin);
          return token;
        }

        break;

      case (SCANNER_BIN_UNDERSCORE):
        if (c == '0' || c == '1') {
          state = SCANNER_BIN_2;
          stringAddChar(&token.t_data.ID, c);
        }
        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_OCT_UNDERSCORE):

        if (c>='0' && c<='7') {
          state = SCANNER_OCT_2;
          stringAddChar(&token.t_data.ID, c);
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_HEX_UNDERSCORE):

        if ((c>='a' && c<='f') || (c>='A' && c<='F') || isdigit(c)) {
          state = SCANNER_HEX_2;
          stringAddChar(&token.t_data.ID, c);
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_NUMBER):

        if (isdigit(c)) {
          stringAddChar(&token.t_data.ID, c);
        }

        else if (c == '.') {
          state = SCANNER_DECIMAL_1;
          stringAddChar(&token.t_data.ID, c);
        }

        else if (c == 'e' || c == 'E') {
          state = SCANNER_EXPONENT_1;
          stringAddChar(&token.t_data.ID, c);
        }

        else if (c == '_') {
          state = SCANNER_INT_UNDERSCORE;
        }

        else {
          int temp;
          sscanf(token.t_data.ID.value, "%d", &temp);
          token.t_data.integer = temp;
          token.t_type = TOKEN_INT;
          ungetc(c, stdin);
          return token;
        }

        break;

      case (SCANNER_INT_UNDERSCORE):

        if (isdigit(c)) {
          state = SCANNER_NUMBER;
          stringAddChar(&token.t_data.ID, c);
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_DECIMAL_1):

        if (isdigit(c)) {
          state = SCANNER_DECIMAL_2;
          stringAddChar(&token.t_data.ID, c);
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_DECIMAL_2):

        if (isdigit(c)) {
          stringAddChar(&token.t_data.ID, c);
        }

        else if (c == 'e' || c == 'E') {
          state = SCANNER_EXPONENT_1;
          stringAddChar(&token.t_data.ID, c);
        }

        else if (c == '_') {
          state = SCANNER_DECIMAL_UNDERSCORE;
        }

        else {
          double temp = atof(token.t_data.ID.value);
          token.t_data.decimal=temp;
          token.t_type = TOKEN_DOUBLE;
          ungetc(c, stdin);
          return token;
        }

        break;

      case (SCANNER_DECIMAL_UNDERSCORE):

        if (isdigit(c)) {
          state = SCANNER_DECIMAL_2;
          stringAddChar(&token.t_data.ID, c);
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_EXPONENT_1):

        if (isdigit(c)) {
          state = SCANNER_EXPONENT_3;
          stringAddChar(&token.t_data.ID, c);
        }

        else if (c == '-') {
          state = SCANNER_EXPONENT_2;
          stringAddChar(&token.t_data.ID, c);
        }

        else if ( c == '+') {
          state = SCANNER_EXPONENT_2;
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_EXPONENT_2):

        if (isdigit(c)) {
          state = SCANNER_EXPONENT_3;
          stringAddChar(&token.t_data.ID, c);
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_EXPONENT_3):

        if (isdigit(c)) {
          stringAddChar(&token.t_data.ID, c);
        }

        else if (c == '_') {
          state = SCANNER_EXPONENT_UNDERSCORE;
        }

        else if (c == '.') {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        else {
          double temp = atof(token.t_data.ID.value);
          token.t_data.decimal=temp;
          token.t_type = TOKEN_DOUBLE;
          ungetc(c, stdin);
          return token;
        }

        break;

      case (SCANNER_EXPONENT_UNDERSCORE):

        if (isdigit(c)) {
          state = SCANNER_EXPONENT_3;
          stringAddChar(&token.t_data.ID, c);
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_COMMENT_1):

        if (c == '"') {
          state = SCANNER_COMMENT_2;
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_COMMENT_2):

        if (c == '"') {
          state = SCANNER_COMMENT_3;
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_COMMENT_3):

        if (c == '"') {
          state = SCANNER_COMMENT_4;
        }

        stringInit(&token.t_data.ID);

        // documentation string not correctly ended
        if (c == EOF) {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        // certain characters need to be recorded as escape sequences
        else if (c == '#') {
          stringAddChar(&token.t_data.ID, '\\');
          stringAddChar(&token.t_data.ID, '0');
          stringAddChar(&token.t_data.ID, '3');
          stringAddChar(&token.t_data.ID, '5');
        }

        else if (c == ' ') {
          stringAddChar(&token.t_data.ID, '\\');
          stringAddChar(&token.t_data.ID, '0');
          stringAddChar(&token.t_data.ID, '3');
          stringAddChar(&token.t_data.ID, '2');
        }

        else if (c == '\n') {
          stringAddChar(&token.t_data.ID, '\\');
          stringAddChar(&token.t_data.ID, '0');
          stringAddChar(&token.t_data.ID, '1');
          stringAddChar(&token.t_data.ID, '0');
        }

        else if( c == '\\') {
          stringAddChar(&token.t_data.ID, '\\');
          stringAddChar(&token.t_data.ID, '0');
          stringAddChar(&token.t_data.ID, '9');
          stringAddChar(&token.t_data.ID, '2');
        }

        // carriage return needs to be ignored
        else if(c == '\r') {
          ;
        }

        else {
          stringAddChar(&token.t_data.ID, c);
        }

        break;

      case (SCANNER_COMMENT_4):

        if (c == '"') {
          state = SCANNER_COMMENT_5;
        }

        else {
          stringAddChar(&token.t_data.ID, '"');
          ungetc(c, stdin);
          state = SCANNER_COMMENT_3;
        }

        break;

      case (SCANNER_COMMENT_5):

        if (c == '"') {

          // if we expect multi-line string we return token
          if (nowExpression == true) {
              return token;
          }

          else {
            state = SCANNER_START;
          }
        }

        else {
          stringAddChar(&token.t_data.ID, '"');
          stringAddChar(&token.t_data.ID, '"');
          ungetc(c, stdin);
          state = SCANNER_COMMENT_3;
        }

        break;

      case (SCANNER_LINE_COMMENT):

        if (c == EOF || c == '\n') {
          state = SCANNER_START;
          ungetc(c, stdin);
        }

        else {
          state = SCANNER_LINE_COMMENT;
        }

        break;

      case (SCANNER_DIVISION):

        if (c == '/'){
          token.t_type = TOKEN_INTEGER_DIVISION;
          return token;
        }

        else {
          ungetc(c, stdin);
          return token;
        }

        break;

      case (SCANNER_EQUAL):

        if (c == '='){
          token.t_type = TOKEN_EQUAL_EQUAL;
          return token;
        }

        else {
          ungetc(c, stdin);
          return token;
        }

        break;

      case (SCANNER_SMALLERTHEN):

        if (c == '='){
          token.t_type = TOKEN_SMALLERTHEN_EQUAL;
          return token;
        }

        else {
          ungetc(c, stdin);
          return token;
        }

        break;

      case (SCANNER_BIGGERTHEN):

        if (c == '='){
          token.t_type = TOKEN_BIGGERTHEN_EQUAL;
          return token;
        }

        else {
          ungetc(c, stdin);
          return token;
        }

        break;

      case (SCANNER_NEG):

        if (c == '='){
          token.t_type = TOKEN_NEG_EQUAL;
          return token;
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_STRING_1):

        if( c == '\''){
          token.t_type = TOKEN_STRING;
          return token;
        }

        else if (c== '\\'){
          state = SCANNER_STRING_2;
        }

        else if (c == EOF || c == '\n') {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        // certain characters need to be recorded as escape sequences
        else if (c == '#') {
          stringAddChar(&token.t_data.ID, '\\');
          stringAddChar(&token.t_data.ID, '0');
          stringAddChar(&token.t_data.ID, '3');
          stringAddChar(&token.t_data.ID, '5');
        }

        else if (c == ' ') {
          stringAddChar(&token.t_data.ID, '\\');
          stringAddChar(&token.t_data.ID, '0');
          stringAddChar(&token.t_data.ID, '3');
          stringAddChar(&token.t_data.ID, '2');
        }

        else {
          stringAddChar(&token.t_data.ID, c);
        }

        break;

      case (SCANNER_STRING_2):

        if( c == '\\') {
          stringAddChar(&token.t_data.ID, c);
          stringAddChar(&token.t_data.ID, '0');
          stringAddChar(&token.t_data.ID, '9');
          stringAddChar(&token.t_data.ID, '2');
          state = SCANNER_STRING_1;
        }

        else if(c == '\'') {
          stringAddChar(&token.t_data.ID, c);
          state = SCANNER_STRING_1;
        }

        else if(c == '\"') {
          stringAddChar(&token.t_data.ID, c);
          state = SCANNER_STRING_1;
        }

        else if(c == 'n') {
          stringAddChar(&token.t_data.ID, '\\');
          stringAddChar(&token.t_data.ID, '0');
          stringAddChar(&token.t_data.ID, '1');
          stringAddChar(&token.t_data.ID, '0');
          state = SCANNER_STRING_1;
        }

        else if(c == 't') {
          stringAddChar(&token.t_data.ID, '\\');
          stringAddChar(&token.t_data.ID, '0');
          stringAddChar(&token.t_data.ID, '0');
          stringAddChar(&token.t_data.ID, '9');
          state = SCANNER_STRING_1;
        }

        else if(c == 'x') {
          state = SCANNER_STRING_3;
        }

        else if (c == EOF) {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        else {
          stringAddChar(&token.t_data.ID, '\\');
          stringAddChar(&token.t_data.ID, '0');
          stringAddChar(&token.t_data.ID, '9');
          stringAddChar(&token.t_data.ID, '2');
          stringAddChar(&token.t_data.ID, c);
          state = SCANNER_STRING_1;
        }

        break;

      case (SCANNER_STRING_3):

        if ((c>='a' && c<='f') || (c>='A' && c<='F') || isdigit(c)) {
          prev_c = c;
          state = SCANNER_STRING_4;
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      case (SCANNER_STRING_4):

        // coneverts hexadecimal escape sequence into respective char
        if ((c>='a' && c<='f') || (c>='A' && c<='F') || isdigit(c)) {
          string String;
          stringInit(&String);
          stringAddChar(&String, '0');
          stringAddChar(&String, 'x');
          stringAddChar(&String, prev_c);
          stringAddChar(&String, c);

          c = (char)strtol(String.value, NULL, 16);

          stringDispose(&String);
          stringAddChar(&token.t_data.ID, c);
          state = SCANNER_STRING_1;
        }

        else {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_LEX;
          return token;
        }

        break;

      default:
        token.t_type = TOKEN_UNDEF;
        token.t_data.integer = ERROR_CODE_LEX;
        return token;

    } //switch

  } //while

  return token;

} //getNextToken
