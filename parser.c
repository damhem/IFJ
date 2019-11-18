
#include "parser.h"
#include "stack.h"

int parse() {


  line_flag = true;
  tStack s;
  stackInit(&s);
  
  int result;
  
  paramIndex = 0;

  stringInit(&functionName);


  

  //get first token
  token = getNextToken(&line_flag);
  if (token.t_type == TOKEN_UNDEF) {
    return token.t_data.integer;
  } else {
    result = program();
  }
  
/*
  int token_counter = 1;
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
*/
  stringDispose(&functionName);
  return result;
}
/*
@
*/
int skipEol() {
  int result;
  switch (token.t_type) {
  case TOKEN_EOL:
    if (((token = getNextToken(&line_flag)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    result = skipEol();
    break;
  default:
    return ERROR_CODE_OK;
  }
  return result;
}

// Program -> Telo_Programu EOF
int program() {
  int result;

  switch (token.t_type) {
  case TOKEN_EOL:
    result = skipEol();
    if (result != ERROR_CODE_OK) return ERROR_CODE_LEX;
    break;

  case TOKEN_ID:
  case TOKEN_DEF:
  case TOKEN_WHILE:
  case TOKEN_IF:
  case TOKEN_PASS:
    //get into program body
    result = programBody();
    if (result != ERROR_CODE_OK) return ERROR_CODE_LEX;

    //there can be some eols
    if (((token = getNextToken(&line_flag)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    result = skipEol();
    if (result != ERROR_CODE_OK) return ERROR_CODE_LEX;

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
    return ERROR_CODE_LEX;
  }
  return ERROR_CODE_LEX;
}

int programBody() {
  int result;

  switch (token.t_type) {
  
  //Telo_programu -> Definice_funkce Telo_programu
  case TOKEN_DEF:
    result = functionDef();
    if (result != ERROR_CODE_OK) return ERROR_CODE_LEX;

    result = programBody();
    if (result != ERROR_CODE_OK) return ERROR_CODE_LEX;

   return ERROR_CODE_OK;

  //Telo_programu -> Prikaz Telo_programu
  case TOKEN_ID:
  case TOKEN_PASS:
  case TOKEN_IF:
  case TOKEN_WHILE:
    result = command();
    if (result != ERROR_CODE_OK) return ERROR_CODE_LEX;

    result = programBody();
    if (result != ERROR_CODE_OK) return ERROR_CODE_LEX;

   return ERROR_CODE_OK;

  //Telo_programu -> eps 
  case TOKEN_EOF:
    return ERROR_CODE_OK;

  default:
    return ERROR_CODE_LEX;
  }
}

/*

*/
int functionDef() {
  int result;

  switch (token.t_type) {
  //Definice_funkce -> Hlavicka_funkce eol indent Telo_funkce dedent
  case TOKEN_DEF:
     result = functionHead();
     //todo zapis instrukce?
     if (result != ERROR_CODE_OK) return result;

     //todo symtable shit

    //there has to be EOL
     if (((token = getNextToken(&line_flag)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

     if (token.t_type != TOKEN_EOL) {
       return ERROR_CODE_LEX;
     }

    //hey there can be a lot of eols izi
    result = skipEol();
    if (result != ERROR_CODE_OK) return ERROR_CODE_LEX;

    //there has to be indent
    if (((token = getNextToken(&line_flag)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_INDENT) {
      return ERROR_CODE_LEX;
    }

    //going deeper to function body
    result = functionBody();
    if (result != ERROR_CODE_OK) return result;

    //there has to be dedent
    //if (((token = getNextToken(&line_flag)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
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

  switch (token.t_type) {
  //Hlavicka_funkce -> def id ( Parametry ) :
  case TOKEN_DEF:
    if (((token = getNextToken(&line_flag)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_ID) {
      return ERROR_CODE_LEX;
    }

    //todo check id of function (maybe its already declared by our program or sth)
    stringAddChars(&functionName, token.t_data.ID.value);

    //there has to be (
    if (((token = getNextToken(&line_flag)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_LEFT_PAR) {
      return ERROR_CODE_LEX;
    }
    
    //function parametres
    result = functionParam();
    if (result != ERROR_CODE_OK) return result;

    //there has to be )
    //if (((token = getNextToken(&line_flag)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_RIGHT_PAR) {
      return ERROR_CODE_LEX;
    }

    //there has to be :
    if (((token = getNextToken(&line_flag)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    if (token.t_type != TOKEN_DOUBLE_DOT) {
      return ERROR_CODE_LEX;
    }

    return ERROR_CODE_OK;

  default:
    return ERROR_CODE_LEX;
  }
}

int functionParam() {
  int result;
  switch (token.t_type) {
    case TOKEN_ID:
    //Parametry -> id Dalsi_parametr
      //todo vytvořit string na paramname a práce se symtable
      
      paramIndex++;

      if (((token = getNextToken(&line_flag)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      result = nextFunctionParam();
      if (result != ERROR_CODE_OK) return result;

      return ERROR_CODE_OK;
    case TOKEN_RIGHT_PAR:
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
  switch (token.t_type) {
  case TOKEN_COMMA:
  //Dalsi_parametr -> , Parametry
    if (((token = getNextToken(&line_flag)).t_type) == TOKEN_UNDEF) return ERROR_CODE_LEX;
    result = functionParam();
    if (result != ERROR_CODE_OK) return result;

    return ERROR_CODE_OK;
  
  case TOKEN_RIGHT_PAR:
  //Dalsi_parametr -> eps
    return ERROR_CODE_OK;
  default:
    return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

int functionBody() {
  int result;
  switch (token.t_type) {
  case TOKEN_ID:
  case TOKEN_PASS:
  case TOKEN_WHILE:
  case TOKEN_IF:
  case TOKEN_RETURN:
  //Telo_funkce -> Prikaz Telo_funkce
  //todo lokalni tabulka symbolu?
    result = command();
    if (result != ERROR_CODE_OK) return result;

    result = functionBody();
    if (result != ERROR_CODE_OK) return result;
    return ERROR_CODE_OK;
  
  case TOKEN_DEDENT:
    return ERROR_CODE_OK;
  default:
    return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

int command() {
  /*int result;
  switch (token.t_type) {
  case TOKEN_IF:
  //Prikaz -> if Vyraz : eol indent Sekvence_prikazu dedent else : eol indent Sekvence_prikazu dedent
    
  case TOKEN_WHILE:
  //Prikaz -> while Vyraz : eol indent Sekvence_prikazu dedent

  case TOKEN_PASS:
  //Prikaz -> pass eol

  case TOKEN_RETURN:
  //Prikaz -> return Vyraz eol

  case TOKEN_ID:
  //id Pokracovani_id

  //todo Prikaz -> Vyraz eol (vyraz muze byt string, int, float, )
  
  default:
    break;
  }*/
  return ERROR_CODE_SYN;
}