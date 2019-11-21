
#include "parser.h"
#include "stack.h"

int parse() {
  printf("START PARSING\n");

  line_flag = true;
  stackInit(&s);
  
  int result;
  
  paramIndex = 0;

  stringInit(&functionName);

  peekToken.t_type = TOKEN_UNDEF;  

  //get first token
  token = getNextToken(&line_flag, &s);
  if (token.t_type == TOKEN_UNDEF) {
    return token.t_data.integer;
  } else {
    result = program();
  }
  

  stringDispose(&functionName);
  return result;
}

int skipEol() {
  int result;
  
  switch (token.t_type) {
  case TOKEN_EOL:
    printf("SKIPPING EOL\n");
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    result = skipEol();
    break;
  default:
    return ERROR_CODE_OK;
  }
  return result;
}

int program() {
  // Program -> Telo_Programu EOF
  int result;

  printf("IN PROGRAM\n");

  switch (token.t_type) {
  case TOKEN_EOL:
    result = skipEol();
    if (result != ERROR_CODE_OK) return result;
    break;

  case TOKEN_ID:
  case TOKEN_DEF:
  case TOKEN_WHILE:
  case TOKEN_IF:
  case TOKEN_PASS:
  case TOKEN_LEFTPAR:
  case TOKEN_INT:
  case TOKEN_DOUBLE:
  case TOKEN_STRING:
  case TOKEN_NONE:
    //get into program body
    result = programBody();
    if (result != ERROR_CODE_OK) return result;

    //there can be some eols
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    result = skipEol();
    if (result != ERROR_CODE_OK) return result;

    //there should be eof
    if (token.t_type == TOKEN_EOF) {
      return ERROR_CODE_OK;
    }
    else {
      return ERROR_CODE_SEM;
    }
  case TOKEN_EOF:
    return ERROR_CODE_OK;
  default:
    return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

int programBody() {
  int result;

  printf("IN PROGRAM BODY\n");

  switch (token.t_type) {
  
  //Telo_programu -> Definice_funkce Telo_programu
  case TOKEN_DEF:
    result = functionDef();
    if (result != ERROR_CODE_OK) return result;

    result = programBody();
    if (result != ERROR_CODE_OK) return result;

   return ERROR_CODE_OK;

  //Telo_programu -> Prikaz Telo_programu
  case TOKEN_ID:
  case TOKEN_PASS:
  case TOKEN_IF:
  case TOKEN_WHILE:
  case TOKEN_INT:
  case TOKEN_DOUBLE:
  case TOKEN_STRING:
  case TOKEN_LEFTPAR:
  case TOKEN_NONE:
    result = command();
    if (result != ERROR_CODE_OK) return result;

    result = programBody();
    if (result != ERROR_CODE_OK) return result;

   return ERROR_CODE_OK;

  //Telo_programu -> eps 
  case TOKEN_EOF:
    return ERROR_CODE_OK;

  default:
    return ERROR_CODE_LEX;
  }
}

int functionDef() {

  printf("IN FUNCTION DEF\n");
  int result;

  switch (token.t_type) {
  //Definice_funkce -> Hlavicka_funkce eol indent Telo_funkce dedent
  case TOKEN_DEF:
     result = functionHead();
     //todo zapis instrukce?
     if (result != ERROR_CODE_OK) return result;

     //todo symtable shit
     

    //there has to be EOL
     if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

     if (token.t_type != TOKEN_EOL) {
       return ERROR_CODE_LEX;
     }

     if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    printf("token typ: %d", token.t_type);
    //hey there can be a lot of eols izi
    result = skipEol();
    if (result != ERROR_CODE_OK) return result;
    
    //there has to be indent
    //
    
    if (token.t_type != TOKEN_INDENT) {
      return ERROR_CODE_LEX;
    }
    
    //going deeper to function body
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

    result = functionBody();
    if (result != ERROR_CODE_OK) return result;

    //there has to be dedent
    //if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_DEDENT) {
      return ERROR_CODE_SYN;
    }

    return ERROR_CODE_OK;
  
  default:
    return ERROR_CODE_LEX;
  }
}

int functionHead() {
  int result;

  printf("IN FUNCTION HEAD\n");

  switch (token.t_type) {
  //Hlavicka_funkce -> def id ( Parametry ) :
  case TOKEN_DEF:
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_ID) {
      return ERROR_CODE_LEX;
    }

    //todo check id of function (maybe its already declared by our program or sth)
    stringAddChars(&functionName, token.t_data.ID.value);

    //there has to be (
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_LEFTPAR) {
      return ERROR_CODE_LEX;
    }
    
    //function parametres
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

    result = functionParam();
    if (result != ERROR_CODE_OK) return result;

    //there has to be )
    //if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_RIGHTPAR) {
      return ERROR_CODE_LEX;
    }

    //there has to be :
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_DOUBLEDOT) {
      return ERROR_CODE_LEX;
    }

    return ERROR_CODE_OK;

  default:
    return ERROR_CODE_LEX;
  }
}

int functionParam() {
  int result;
  printf("IN FUNCTION PARAMS\n");
  switch (token.t_type) {
    case TOKEN_ID:
    //Parametry -> id Dalsi_parametr
      //todo vytvořit string na paramname a práce se symtable
      
      paramIndex++;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      result = nextFunctionParam();
      if (result != ERROR_CODE_OK) return result;

      return ERROR_CODE_OK;
    case TOKEN_RIGHTPAR:
    //Parametry -> eps
      paramIndex = 0;
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_LEX;
  }
  return ERROR_CODE_LEX;
}

int nextFunctionParam() {
  int result;
  printf("IN NEXT FUNCTION PARAM\n");
  switch (token.t_type) {
  case TOKEN_COMMA:
  //Dalsi_parametr -> , Parametry
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    result = functionParam();
    if (result != ERROR_CODE_OK) return result;

    return ERROR_CODE_OK;
  
  case TOKEN_RIGHTPAR:
  //Dalsi_parametr -> eps
    return ERROR_CODE_OK;
  default:
    return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

int functionBody() {
  int result;
  printf("IN FUNCTION BODY, token type: %d\n", token.t_type);
  switch (token.t_type) {
  case TOKEN_ID:
  case TOKEN_PASS:
  case TOKEN_WHILE:
  case TOKEN_IF:
  case TOKEN_RETURN:
  case TOKEN_INT:
  case TOKEN_DOUBLE:
  case TOKEN_NONE:
  case TOKEN_STRING:
  case TOKEN_LEFTPAR:
  //Telo_funkce -> Prikaz Telo_funkce
  //todo lokalni tabulka symbolu?
    result = command();
    if (result != ERROR_CODE_OK) return result;

    result = functionBody();
    if (result != ERROR_CODE_OK) return result;
    return ERROR_CODE_OK;
  
  case TOKEN_DEDENT:
    printf("got dedent\n");
    return ERROR_CODE_OK;
  default:
    return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

int command() {

  printf("IN COMMAND\n");

  int result;
  switch (token.t_type) {
  case TOKEN_IF:
    //Prikaz -> if Vyraz : eol indent Sekvence_prikazu dedent else : eol indent Sekvence_prikazu dedent
    //todo vyraz expression
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    printf("token type: %d\n", token.t_type);

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_DOUBLEDOT) {
      return ERROR_CODE_SYN;
    }
    printf("token type: %d\n", token.t_type);
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_EOL) {
      return ERROR_CODE_SYN;
    }
    printf("token type: %d\n", token.t_type);
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_INDENT) {
      return ERROR_CODE_SYN;
    }
    printf("token type: %d\n", token.t_type);
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    
    result = commands();
    if (result != ERROR_CODE_OK) {
      return result;
    }

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_DEDENT) {
      return ERROR_CODE_SYN;
    }
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_ELSE) {
      return ERROR_CODE_SYN;
    }
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_DOUBLEDOT) {
      return ERROR_CODE_SYN;
    }
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_INDENT) {
      return ERROR_CODE_SYN;
    }
    //

    result = commands();
    if (result != ERROR_CODE_OK) {
      return result;
    }

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_DEDENT) {
      return ERROR_CODE_SYN;
    }

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

    return ERROR_CODE_OK;

  case TOKEN_WHILE:
    //Prikaz -> while Vyraz : eol indent Sekvence_prikazu dedent
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    //todo vyraz here

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_DOUBLEDOT) return ERROR_CODE_SYN;

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_INDENT) return ERROR_CODE_SYN;

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    
    result = commands();
    if (result != ERROR_CODE_OK) {
      return result;
    }

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_DEDENT) {
      return ERROR_CODE_SYN;
    }

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

    return ERROR_CODE_OK;

  case TOKEN_PASS:
    //Prikaz -> pass eol
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    return ERROR_CODE_OK;

  case TOKEN_RETURN:
    //Prikaz -> return Vyraz eol
    //todo vyraz again lol
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_EOL) {
      return ERROR_CODE_SYN;
    }

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

    return ERROR_CODE_OK;
  case TOKEN_ID: ;
    //have to check if its expression
    token_type nextTokenType;
    if ((nextTokenType = peekNextToken()) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    printf("SUCCESS CHECK OF NEXT TOKEN, Type: %d\n", nextTokenType);
    //Prikaz -> Vyraz eol
    switch (nextTokenType) {
      //has to be operator
      case TOKEN_ADDITION:
      case TOKEN_SUBTRACTION:
      case TOKEN_MULTIPLICATION:
      case TOKEN_DIVISION:
      case TOKEN_EQUAL_EQUAL:
      case TOKEN_SMALLERTHEN:
      case TOKEN_SMALLERTHEN_EQUAL:
      case TOKEN_BIGGERTHEN:
      case TOKEN_BIGGERTHEN_EQUAL:
      

        //todo expression napojení (expression(token))
        
        return ERROR_CODE_SYN;
      case TOKEN_EOL:
        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
        return ERROR_CODE_OK;
    
  case TOKEN_EQUAL:
    //Pokracovani_id -> = Pokracovani_id_next
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    result = continueID();
    if (result != ERROR_CODE_OK) return result;

    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    return ERROR_CODE_OK;
  default:
    printf("default in command: NEXT TOKEN TYPE: %d", nextTokenType);
    return ERROR_CODE_SYN;
  }

  case TOKEN_STRING:
  case TOKEN_INT:
  case TOKEN_DOUBLE:
  case TOKEN_NONE:
  case TOKEN_LEFTPAR:
    //this is expression 100%
    //todo Prikaz -> Vyraz eol (vyraz muze byt string, int, float, )
    //todo expression
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    return ERROR_CODE_OK;
  
  default:
    return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

int continueID() {
  int result;
  printf("IN CONTINUE ID\n");
  switch (token.t_type) {
  case TOKEN_ID: ;
    //have to check if its expression
  token_type nextTokenType;
  if ((nextTokenType = peekNextToken()) == TOKEN_UNDEF) return ERROR_CODE_LEX;
  printf("SUCCESS CHECK OF NEXT TOKEN, Type: %d\n", nextTokenType);

  switch (nextTokenType) {
    //Pokracovani_id_next -> Vyraz eol
    //has to be operator
    case TOKEN_ADDITION:
    case TOKEN_SUBTRACTION:
    case TOKEN_MULTIPLICATION:
    case TOKEN_DIVISION:
    case TOKEN_EQUAL_EQUAL:
    case TOKEN_SMALLERTHEN:
    case TOKEN_SMALLERTHEN_EQUAL:
    case TOKEN_BIGGERTHEN:
    case TOKEN_BIGGERTHEN_EQUAL:
    case TOKEN_EOL:

    printf("WILL BE DOING EXPRESSION\n");
    //todo expression napojení (expression(token))

    //todo eol token
    
    return ERROR_CODE_SYN;

    case TOKEN_LEFTPAR:
      //Pokracovani_id_next -> id ( Termy ) eol
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      result = terms();
      if (result != ERROR_CODE_OK) return result;

      if (token.t_type != TOKEN_RIGHTPAR) return ERROR_CODE_SYN;
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      return ERROR_CODE_OK;

    default:
      return ERROR_CODE_SYN;
    }
  
  default:
    return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

int terms() {
  printf("IN TERMS NOW\n");
  int result;
  switch (token.t_type) {
    case TOKEN_ID:
      //todo sth
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      result = nextTerms();
      if (result != ERROR_CODE_OK) return result;
      if (token.t_type != TOKEN_RIGHTPAR) return ERROR_CODE_SYN;
      return ERROR_CODE_OK;
    case TOKEN_STRING:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_NONE:
      //todo sth
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      result = nextTerms();
      if (result != ERROR_CODE_OK) return result;
      if (token.t_type != TOKEN_RIGHTPAR) return ERROR_CODE_SYN;
      return ERROR_CODE_OK;
    case TOKEN_RIGHTPAR:
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

int nextTerms() {
  printf("IN NEXT TERMS:\n");
  int result;
  switch (token.t_type) {
  case TOKEN_COMMA:
    //Next_term -> , Termy
    if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    result = terms();
    if (result != ERROR_CODE_OK) return result;

    //todo maybe theres problem with foo(one, two, ) -> now its no mistake (peek maybe?)

    return ERROR_CODE_OK;
  case TOKEN_RIGHTPAR:
    return ERROR_CODE_OK;
  default:
    return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

int commands() {
  ERROR_CODE result;
  switch (token.t_type) {
    case TOKEN_ID:
    case TOKEN_LEFTPAR:
    case TOKEN_IF:
    case TOKEN_WHILE:
    case TOKEN_PASS:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_STRING:
    case TOKEN_NONE:
      //Sekvence_prikazu -> Prikaz Sekvence_prikazu
      result = command();
      if (result != ERROR_CODE_OK) return result;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      return commands();

    case TOKEN_DEDENT:
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}