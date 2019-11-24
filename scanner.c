
#include "scanner.h"
#include "parser.h"

token_type peekNextToken() {
  peekToken = getNextToken(&line_flag, &s);

  if (peekToken.t_type == TOKEN_UNDEF) {
    return -1;
  }

  return peekToken.t_type;
}

Token getNextToken(bool *line_flag, tStack *s) {

  //checks if some token isnt already read
  if (peekToken.t_type != TOKEN_UNDEF) {
    Token peekThisToken = peekToken;
    peekToken.t_type = TOKEN_UNDEF;
    return peekThisToken;
  }

  bool run = true;
  Token token;
  token.t_type = TOKEN_UNDEF;

  int state = SCANNER_START;
  char c;
  char prev_c;
  int spacecount = 0;
  char s_top;

  while(run) {

  if (dentcount>0) {
    dentcount--;
    token.t_type = TOKEN_DEDENT;
    return token;
  }

    c = (char) getc(stdin);

    switch (state) {
      case (SCANNER_START):
        if (c == EOF) {
          *line_flag=false;
					token.t_type = TOKEN_EOF;
					return token;
				}
        else if (stringInit(&token.t_data.ID) == ERROR_CODE_INTERNAL) {
          token.t_type = TOKEN_UNDEF;
          token.t_data.integer = ERROR_CODE_INTERNAL;
          return token;
        }
        else if (c == '\n'){
          c = (char) getc(stdin);
          if (c != ' ') {
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
        if (isalpha(c) || c == '_') {
          *line_flag=false;
          state = SCANNER_ID;
          token.t_type = TOKEN_ID;
          stringAddChar(&token.t_data.ID, c);
        }
        else if (c == ' ' && (*line_flag) == true) {
          state = SCANNER_DENTCOUNT;
          spacecount++;
        }
        else if (isdigit(c)) {
          *line_flag=false;
          state = SCANNER_NUMBER;
          stringAddChar(&token.t_data.ID, c);
        }
        else if (c == '"'){
          *line_flag=false;
          state = SCANNER_COMMENT;
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
          state = SCANNER_STRING;
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
        break;
      case (SCANNER_ID):
        if (isalpha(c) || isdigit(c) || c == '_') {
          stringAddChar(&token.t_data.ID, c);
        }else {
          ungetc(c, stdin);
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
          if (c == ' ') {
            spacecount++;
          }
          else {
            *line_flag=false;
            ungetc(c, stdin);
            stackTop(s,&s_top);
            if (spacecount>s_top || stackEmpty(s)) {
              token.t_type = TOKEN_INDENT;

              stackPush(s,spacecount);
              spacecount=0;
              return token;
            }
            else if (spacecount==s_top) {
              spacecount=0;
              state = SCANNER_START;
            }
            else if (spacecount<s_top) {
              while (spacecount<s_top && stackEmpty(s)==0) {
                dentcount++;
                stackPop(s);
                stackTop(s,&s_top);
                if (spacecount>s_top) {
                  token.t_type = TOKEN_UNDEF;
                  token.t_data.integer = ERROR_CODE_LEX;
                  return token;
                }
               
                if (spacecount==s_top || stackEmpty(s) != 0) {
                   //printf("spacecount: %d\n", spacecount);
                   //printf("stack_top: %d\n", s_top);
                  token.t_type = TOKEN_DEDENT;
                  spacecount=0;
                  dentcount--;
                  return token;
                }
              }
            }
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
        else {
          //přidání rozdělení na INT a DOUBLE, přidání čísla do data.int data.decimal atd...
          int temp;
          sscanf(token.t_data.ID.value, "%d", &temp);
          token.t_data.integer=temp;
          token.t_type = TOKEN_INT;
          ungetc(c, stdin);
          return token;
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
        if (c == '"'){
          token.t_type = TOKEN_STRING;              //3"
          state = SCANNER_COMMENT_0;
        }else{
          ungetc(c, stdin);
          ungetc(c, stdin);
        }
        break;
      case (SCANNER_COMMENT_0):
        if (c == '"'){                        //1"
          state = SCANNER_COMMENT_01;
        }else{
          stringAddChar(&token.t_data.ID, c);
        }
        break;
      case (SCANNER_COMMENT_01):
        if (c == '"'){                        //2"
          state = SCANNER_COMMENT_02;
        }else{
          stringAddChar(&token.t_data.ID, c);
          state = SCANNER_COMMENT_0;
        }
        break;
      case (SCANNER_COMMENT_02):
        if (c == '"'){                       //3"
          return token;
        }else{
          stringAddChar(&token.t_data.ID, c);
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
        break;
      case (SCANNER_EQUAL):
        if (c == '='){
          token.t_type = TOKEN_EQUAL_EQUAL;
          return token;
        }else{
          ungetc(c, stdin);
          return token;
        }
        break;
      case(SCANNER_SMALLERTHEN):
        if (c == '='){
          token.t_type = TOKEN_SMALLERTHEN_EQUAL;
          return token;
        }else{
          ungetc(c, stdin);
          return token;
        }
        break;
      case(SCANNER_BIGGERTHEN):
        if (c == '='){
          token.t_type = TOKEN_BIGGERTHEN_EQUAL;
          return token;
        }else{
          ungetc(c, stdin);
          return token;
        }
        break;
      case(SCANNER_NEG):
        if (c == '='){
          token.t_type = TOKEN_NEG_EQUAL;
          return token;
        }else{
          ungetc(c, stdin);
        }
        break;
      case(SCANNER_STRING):
       if( c == '\''){
         token.t_type = TOKEN_STRING;
         return token;
       }else if(c != '\\'){
         stringAddChar(&token.t_data.ID, c);
       }else if(c== '\\'){
         state = SCANNER_STRING1;
       }
       break;
     case(SCANNER_STRING1):
      if( c == '\\'){
        stringAddChar(&token.t_data.ID, c);
        state = SCANNER_STRING;
      }else if(c == '\''){
        stringAddChar(&token.t_data.ID, c);
        state = SCANNER_STRING;
      }else if(c == '\"'){
        stringAddChar(&token.t_data.ID, c);
        state = SCANNER_STRING;
      }else if(c == 'n'){
        c = '\n';
        stringAddChar(&token.t_data.ID, c);
        state = SCANNER_STRING;
      }else if(c == 't'){
        c = '\t';
        stringAddChar(&token.t_data.ID, c);
        state = SCANNER_STRING;
      }else if(c == 'x'){
        state = SCANNER_STRING2;
      }
      break;
    case(SCANNER_STRING2):
      if(isdigit(c) || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F' ) {
        prev_c = c;
        state = SCANNER_STRING3;
      }
      break;
    case(SCANNER_STRING3):
      if(isdigit(c) || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F' ) {
        char c_h;
        string String;
        stringInit(&String);
        c_h = '0';
        stringAddChar(&String, c_h);
        c_h = 'x';
        stringAddChar(&String, c_h);
        stringAddChar(&String, prev_c);
        stringAddChar(&String, c);

        c = (char)strtol(String.value, NULL, 16);

        stringDispose(&String);
        stringAddChar(&token.t_data.ID, c);
        state = SCANNER_STRING;
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
