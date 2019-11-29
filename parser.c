#include "parser.h"

ERROR_CODE parse() {

  ERROR_CODE result;

  line_flag = true;
  nowExpression = false;
  paramIndex = 0;
  peekToken.t_type = TOKEN_UNDEF;

  //initialize structures
  if (stackInit(&s) == ERROR_CODE_INTERNAL) {
    return ERROR_CODE_INTERNAL;
  }
  if (SYMInit(&glSymtable) == ERROR_CODE_INTERNAL) {
    return ERROR_CODE_INTERNAL;
  }

  stringInit(&functionName);

  //get first token
  token = getNextToken(&line_flag, &s);
  if (token.t_type == TOKEN_UNDEF) {
    return token.t_data.integer;
  } else {
    result = program();
  }

  //free memory
  stringDispose(&functionName);
  exp_stackClear(&stack_expression);
  stackClear(&s);
  SYMDispose(&glSymtable);
  SYMDispose(&lcSymtable);

  return result;
}

ERROR_CODE skipEol() {
  ERROR_CODE result;
  switch (token.t_type) {
    case TOKEN_EOL:
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      return skipEol();
    default:
      return ERROR_CODE_OK;
  }
  return result;
}

ERROR_CODE program() {
  // Program -> Telo_Programu EOF

  ERROR_CODE result = skipEol();
      if (result != ERROR_CODE_OK) return result;

  switch (token.t_type) {
    //case TOKEN_EOL:
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
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
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

ERROR_CODE programBody() {
  ERROR_CODE result;

  //printf("IN PROGRAM BODY\n");

  switch (token.t_type) {

    case TOKEN_DEF:
      //Telo_programu -> Definice_funkce Telo_programu
      result = functionDef();
      if (result != ERROR_CODE_OK) return result;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      return programBody();

    case TOKEN_ID:
    case TOKEN_PASS:
    case TOKEN_IF:
    case TOKEN_WHILE:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_STRING:
    case TOKEN_LEFTPAR:
    case TOKEN_NONE:
      //Telo_programu -> Prikaz Telo_programu
      result = command();
      if (result != ERROR_CODE_OK) return result;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      return programBody();


    case TOKEN_EOF:
      //Telo_programu -> eps
      return ERROR_CODE_OK;

    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

ERROR_CODE functionDef() {

  ERROR_CODE result;

  switch (token.t_type) {

    case TOKEN_DEF:
      //Definice_funkce -> Hlavicka_funkce eol indent Telo_funkce dedent
      result = functionHead();
      if (result != ERROR_CODE_OK) return result;

      operand operand_label = initOperand(operand_label, functionName, TOKEN_ID, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand_label);

      //there has to be EOL
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_EOL) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      //there has to be indent
      if (token.t_type != TOKEN_INDENT) {
        return ERROR_CODE_SYN;
      }

       if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      //hey there can be a lot of eols izi
      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      //create local symtable
      if (SYMInit(&lcSymtable) == ERROR_CODE_INTERNAL) {
        return ERROR_CODE_INTERNAL;
      }

      //set global symtable to pointer
      tBSTNodePtr helper = SYMSearch(&glSymtable, functionName);
      if (helper == NULL) {
        return ERROR_CODE_SEM_OTHER;
      }
      else {
        //now we have to add paramnames to local symtable
        for (int i = 0; i < helper->parametrs; i++) {
          SYMInsert(&lcSymtable, helper->paramName[i], false);
        }
      }

      //going deeper to function body
      result = functionBody();
      if (result != ERROR_CODE_OK) return result;

      //there has to be dedent
      if (token.t_type != TOKEN_DEDENT) {
        return ERROR_CODE_SYN;
      }

      //dispose function name
      //stringClear(&functionName);

      //clear paramIndex
      paramIndex = 0;
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      return ERROR_CODE_OK;

    default:
      return ERROR_CODE_SYN;
    }
  return ERROR_CODE_SYN;
}

ERROR_CODE functionHead() {

  ERROR_CODE result;

  switch (token.t_type) {

    case TOKEN_DEF: ;
      int numberOfPrevParams = 0;
      bool declared = false;
      //Hlavicka_funkce -> def id ( Parametry ) :
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_ID) {
        return ERROR_CODE_SYN;
      }

      // check id of function (maybe its already declared by our program or sth)
      stringAddChars(&functionName, token.t_data.ID.value);
      tBSTNodePtr helper = SYMSearch(&glSymtable, functionName);
      if (helper != NULL) {
        //oooo lol its already declared
        //have to check if its not variable
        if (helper->DataType == Variable) {
          fprintf(stderr, "Uz byla vytvorena promenna s nazvem: %s", functionName.value);
          return ERROR_CODE_SEM;
        }
        //when function has been declared, but not defined
        else if (strcmp(helper->Key.value, functionName.value) == 0 && (helper->defined == false)) {

          result = SYMInsert(&glSymtable, functionName, true);
          numberOfPrevParams = helper->parametrs;
          declared = true;

          if (result != ERROR_CODE_OK) return result;
          helper->defined = true;
        }
        else {
          //there is this function already
          fprintf(stderr, "Uz byla vytvorena funkce s jmenem: %s\n", functionName.value);
          return ERROR_CODE_SEM;
        }
      }
      else {
        //this is new entry to the function
        result = SYMInsert(&glSymtable, functionName, true);
        if (result != ERROR_CODE_OK) return result;
        if ((helper = SYMSearch(&glSymtable, functionName)) == NULL) {
          return ERROR_CODE_SEM_OTHER;
        }
        else {
          //add parametres
          helper->defined = true;
          helper->DataType = Function;
          helper->parametrs = 0;
        }
      }


      //there has to be (
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_LEFTPAR) {
        return ERROR_CODE_SYN;
      }

      //function parametres
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = functionParam();
      if (result != ERROR_CODE_OK) return result;

      //checking if it was called with right number of params
      if (declared) {
        if (numberOfPrevParams != paramIndex + 1) {
          return ERROR_CODE_SEM;
        }
      }

      //there has to be )
      if (token.t_type != TOKEN_RIGHTPAR) {
        return ERROR_CODE_SYN;
      }

      //there has to be :
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_DOUBLEDOT) {
        return ERROR_CODE_SYN;
      }

      return ERROR_CODE_OK;

    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

ERROR_CODE functionParam() {
  ERROR_CODE result;
  //printf("IN FUNCTION PARAMS\n");

  switch (token.t_type) {
    case TOKEN_ID: ;
      //Parametry -> id Dalsi_parametr

      string Paramname;
      stringInit(&Paramname);
      stringAddChars(&Paramname, token.t_data.ID.value);

      tBSTNodePtr helper = SYMSearch(&glSymtable, Paramname);
      if (helper != NULL) {
        //there has been function with the same ID
        fprintf(stderr, "Nemuzete mit stejny nazev funkce/promenne a nazev parametru funkce: %s\n", Paramname.value);
        return ERROR_CODE_SEM;
      }

      helper = SYMSearch(&glSymtable, functionName);

      if (helper != NULL) {
        //means that function is in symtable
        //have to check previous paramentres (might have the same name)
        for (int i = 0; i < paramIndex; i++) {
          if (strcmp(helper->paramName[i].value, Paramname.value) == 0) {
            fprintf(stderr, "Uz mate definovany parametr: %s\n", Paramname.value);
            return ERROR_CODE_SEM;
          }
        }

        //now we can actually insert a parameter
        stringInit(&(helper->paramName[paramIndex]));
        stringAddChars(&(helper->paramName[paramIndex]), Paramname.value);

        //increment number of function paramentres
        (helper->parametrs)++;

        //increment the index
        paramIndex++;

      }
      else {
        return ERROR_CODE_SEM_OTHER;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = nextFunctionParam();
      if (result != ERROR_CODE_OK) return result;

      return ERROR_CODE_OK;

    case TOKEN_RIGHTPAR:
      //Parametry -> eps
      return ERROR_CODE_OK;

    default:
      return ERROR_CODE_SYN;

  }
  return ERROR_CODE_SYN;
}

ERROR_CODE nextFunctionParam() {

  switch (token.t_type) {
    case TOKEN_COMMA:
      //Dalsi_parametr -> , Parametry
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      if (token.t_type == TOKEN_RIGHTPAR) return ERROR_CODE_SYN;

      return functionParam();

    case TOKEN_RIGHTPAR:
      //Dalsi_parametr -> eps
      return ERROR_CODE_OK;

    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

ERROR_CODE functionBody() {
  ERROR_CODE result;

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

      //todo????????????? some var infunction

      result = command();
      if (result != ERROR_CODE_OK) return result;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      return functionBody();

    case TOKEN_DEDENT:
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

ERROR_CODE command() {

  ERROR_CODE result;

  switch (token.t_type) {
    case TOKEN_IF:
      //Prikaz -> if Vyraz : eol indent Sekvence_prikazu dedent else : eol indent Sekvence_prikazu dedent
      nowExpression = true;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      VarType type;
      result = expression(&type);
      if (result != ERROR_CODE_OK) return result;
      
      nowExpression = false;

      //todo vyhodnoceni vyrazu

      //have to end with :
      if (token.t_type != TOKEN_DOUBLEDOT) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_EOL) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_INDENT) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      result = commands();
      if (result != ERROR_CODE_OK) {
        return result;
      }

      if (token.t_type != TOKEN_DEDENT) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      if (token.t_type != TOKEN_ELSE) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_DOUBLEDOT) {
        return ERROR_CODE_SYN;
      }
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_EOL) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_INDENT) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      result = commands();
      if (result != ERROR_CODE_OK) {
        return result;
      }

      if (token.t_type != TOKEN_DEDENT) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      return ERROR_CODE_OK;

    case TOKEN_WHILE:
      //Prikaz -> while Vyraz : eol indent Sekvence_prikazu dedent
      nowExpression = true;

       if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      VarType type_while;
      result = expression(&type_while);
      if (result != ERROR_CODE_OK) return result;
      
      //todo vyhodnoceni vyrazu
      nowExpression = false;

      if (token.t_type != TOKEN_DOUBLEDOT) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_INDENT) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      result = commands();
      if (result != ERROR_CODE_OK) {
        return result;
      }

      if (token.t_type != TOKEN_DEDENT) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      return ERROR_CODE_OK;

    case TOKEN_PASS:
      //Prikaz -> pass eol
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      return ERROR_CODE_OK;

    case TOKEN_RETURN:
      //Prikaz -> return Vyraz eol
      //todo vyraz again lol
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_EOL) {
        return ERROR_CODE_SYN;
      }

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      return ERROR_CODE_OK;

    case TOKEN_ID: ;

      //have to check if its expression
      token_type nextTokenType;
      if ((nextTokenType = peekNextToken()) == TOKEN_UNDEF) return ERROR_CODE_LEX;

      //Prikaz -> Vyraz eol
      switch (nextTokenType) {
        case TOKEN_ADDITION:
        case TOKEN_SUBTRACTION:
        case TOKEN_MULTIPLICATION:
        case TOKEN_DIVISION:
        case TOKEN_EQUAL_EQUAL:
        case TOKEN_SMALLERTHEN:
        case TOKEN_SMALLERTHEN_EQUAL:
        case TOKEN_BIGGERTHEN:
        case TOKEN_BIGGERTHEN_EQUAL:
          //has to be operator
        
          VarType type_id;
          result = expression(&type_id);
          if (result != ERROR_CODE_OK) return result;

          return ERROR_CODE_OK;

        case TOKEN_EOL: ;
          //just a variable
          if (functionName.length == 0) {
            tBSTNodePtr helper = SYMSearch(&glSymtable, token.t_data.ID);
            if (helper == NULL) {
              //promenna jeste nebyla vytvorena -> vytvorime
              SYMInsert(&glSymtable, token.t_data.ID, false);

              //budu generovat instrukci pro vytvoreni promenne
              operand var_operand = initOperand(var_operand, token.t_data.ID, TOKEN_ID, GF, false, false);
              oneOperandInstr(&instrList, DEFVAR, var_operand);
            }
          }
          else {
            //now im in function
            tBSTNodePtr helper = SYMSearch(&lcSymtable, token.t_data.ID);
            if (helper == NULL) {
              //todo nepouzival jsem ji predtim?
              //promenna jeste nebyla vytvorena -> vytvorime
              SYMInsert(&lcSymtable, token.t_data.ID, false);

              //budu generovat instrukci pro vytvoreni promenne
              operand var_operand = initOperand(var_operand, token.t_data.ID, TOKEN_ID, LF, false, false);
              oneOperandInstr(&instrList, DEFVAR, var_operand);
            }
          }

          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //EOL
          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //NEXT
          return ERROR_CODE_OK;

        case TOKEN_EQUAL:
          //Pokracovani_id -> = Pokracovani_id_next
          //going into variable assign

          //have to update symtable
          if (strlen(functionName.value) == 0) {

            //now im in main program -> looking into global symtable
            //make sure that this var is not in table
            tBSTNodePtr helper;
            if ((helper = SYMSearch(&glSymtable, token.t_data.ID)) == NULL) {
              //promenna jeste nebyla vytvorena
              SYMInsert(&glSymtable, token.t_data.ID, false);
              //nastavim si helper na prave vytvorenou promennnou
              helper = SYMSearch(&glSymtable, token.t_data.ID);

              //budu generovat instrukci pro vytvoreni promenne
              operand var_operand = initOperand(var_operand, token.t_data.ID, TOKEN_ID, GF, false, false);
              oneOperandInstr(&instrList, DEFVAR, var_operand);
            }
            else {
              //promenna uz byla vytvorena (nemusim generovat instrukci pro generovani promenne)
              //helper is set to global that variable
              helper = SYMSearch(&glSymtable, token.t_data.ID);
            }

            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //=
            nowExpression = true;
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //vyraz

            VarType sth;
            result = expression(&sth);
            if (result != ERROR_CODE_OK) return result;
            nowExpression = false;

            // var -> vysledek_expression
            helper->DataType = sth;

            operand var_operand = initOperand(var_operand, helper->Key, TOKEN_ID, GF, false, false);
            oneOperandInstr(&instrList, POPS, var_operand);
          }

          else {
            //now im in function -> looking into local symtable
            //make sure that this var is not in the table already
            tBSTNodePtr helper;
            if ((helper = SYMSearch(&glSymtable, token.t_data.ID)) != NULL) {
              //var is in the global symtable -> create new in local
              //todo if I already used it
              SYMInsert(&lcSymtable, token.t_data.ID, false);
              //set helper to the symtable initalized node
              helper = SYMSearch(&lcSymtable, token.t_data.ID);
              //generate code
              operand var_operand = initOperand(var_operand, token.t_data.ID, TOKEN_ID, LF, false, false);
              oneOperandInstr(&instrList, DEFVAR, var_operand);
            }
            else if ((helper = SYMSearch(&lcSymtable, token.t_data.ID)) == NULL) {
              //var is nowhere -> create new in local
              SYMInsert(&lcSymtable, token.t_data.ID, false);
              //set helper to the symtable initalized node
              helper = SYMSearch(&lcSymtable, token.t_data.ID);
              //generate code of defvar
              operand var_operand = initOperand(var_operand, token.t_data.ID, TOKEN_ID, LF, false, false);
              oneOperandInstr(&instrList, DEFVAR, var_operand);
            }
            else {
              //promenna uz byla vytvorena v lc (nemusim generovat instrukci pro generovani promenne)
              //helper is set to local that variable
              helper = SYMSearch(&lcSymtable, token.t_data.ID);
            }

            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //=
            nowExpression = true;
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //vyraz

            VarType sth;
            result = expression(&sth);
            if (result != ERROR_CODE_OK) return result;
            nowExpression = false;

            // var -> vysledek_expression
            helper->DataType = sth;

            operand var_operand = initOperand(var_operand, helper->Key, TOKEN_ID, LF, false, false);
            oneOperandInstr(&instrList, POPS, var_operand);
          }

          return ERROR_CODE_OK;


        case TOKEN_LEFTPAR:
          //just a function call (without actually var to assign to) - procedure
          
          nowExpression = true;
          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
          VarType type_proc;
          result = expression(&type_proc);
          if (result != ERROR_CODE_OK) return result;
          
          nowExpression = false;

          //todo? co bude vracet expression? nebo nic?

          if (token.t_type != TOKEN_EOL) {
            return ERROR_CODE_SYN;
          }

          return ERROR_CODE_OK;
        default:
          return ERROR_CODE_SYN;
      }

    case TOKEN_STRING:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_NONE:
    case TOKEN_LEFTPAR: ;

      // Prikaz -> Vyraz eol (vyraz muze byt string, int, float, )
      nowExpression = true;
      VarType sth;
      result = expression(&sth);
      if (result != ERROR_CODE_OK) return result;
      nowExpression = false;

      if (token.t_type != TOKEN_EOL) {
        return ERROR_CODE_SYN;
      }
      string nil;
      stringInit(&nil);
      operand operand_bye = initOperand(operand_bye, nil, TOKEN_ID, GF, true, false);
      oneOperandInstr(&instrList, POPS, operand_bye);
      stringDispose(&nil);

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      return ERROR_CODE_OK;

    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}


/*
ERROR_CODE continueID() {

  ERROR_CODE result;
  //printf("IN CONTINUE ID\n");

  switch (token.t_type) {
    case TOKEN_ID: ;
      //have to check if its expression
      token_type nextTokenType;
      if ((nextTokenType = peekNextToken()) == TOKEN_UNDEF) return ERROR_CODE_LEX;
      //printf("SUCCESS CHECK OF NEXT TOKEN, Type: %d\n", nextTokenType);

    switch (nextTokenType) {
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
        //Pokracovani_id_next -> Vyraz eol
        //has to be operator

        //printf("WILL BE DOING EXPRESSION\n");
        //tod expression napojení (expression(token))

        //toeol token

        return ERROR_CODE_SYN;

      case TOKEN_LEFTPAR:
        //Pokracovani_id_next -> id ( Termy ) eol
        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

        result = terms();
        if (result != ERROR_CODE_OK) return result;

        if (token.t_type != TOKEN_RIGHTPAR) return ERROR_CODE_SYN;

        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
        if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;

        if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
        return ERROR_CODE_OK;

      default:
        return ERROR_CODE_SYN;
      }
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

ERROR_CODE terms() {

  //printf("IN TERMS NOW\n");
  ERROR_CODE result;

  switch (token.t_type) {
    case TOKEN_ID:
      // sth with symtable
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = nextTerms();
      if (result != ERROR_CODE_OK) return result;

      if (token.t_type != TOKEN_RIGHTPAR) return ERROR_CODE_SYN;

      return ERROR_CODE_OK;

    case TOKEN_STRING:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_NONE:

      // sth
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

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

ERROR_CODE nextTerms() {

  //printf("IN NEXT TERMS:\n");
  

  switch (token.t_type) {
    case TOKEN_COMMA:
      //Next_term -> , Termy
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      //tod maybe theres problem with foo(one, two, ) -> now its no mistake (peek maybe?)

      return terms();

    case TOKEN_RIGHTPAR:
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

*/
ERROR_CODE commands() {

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



      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      //printf("after skip elo, %d\n", token.t_type);

      return commands();

    case TOKEN_DEDENT:
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}
