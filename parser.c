




#include "parser.h"
#include "stack.h"

int parse() {

  bool line_flag = true;
  tStack s;
  stackInit(&s);

  Token token = getNextToken(&line_flag);
  int token_counter = 0;

  while (token.t_type != TOKEN_EOF) {


    if (token.t_type == TOKEN_UNDEF) {
      return token.t_data.integer;
    }

    printf("token num %d type: %d\n", token_counter, token.t_type);
    printf("token num %d data: %s\n", token_counter, token.t_data.ID.value);
    printf("\n");

    token_counter++;

    token = getNextToken(&line_flag,&s);
  }


  return ERROR_CODE_OK;
}
