#include "parser.h"

ERROR_CODE parse() {

  ERROR_CODE result;

  line_flag = true;
  nowExpression = false;
  paramIndex = 0;
  labelCounter = 0;
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
    if (result != ERROR_CODE_OK) return result;
  }

  //have to check if all the functions in global symtable are defined
  result = checkDefinedFunctions(glSymtable.root);
  if (result != ERROR_CODE_OK) return result;

  //free memory
  stringDispose(&functionName);
  exp_stackClear(&stack_expression);
  stackClear(&s);
  SYMDispose(&glSymtable);
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
        return ERROR_CODE_SYN;
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

      string functionLabel;
      stringInit(&functionLabel);
      stringAddChars(&functionLabel, functionName.value);
      stringAddChars(&functionLabel, "%");
      operand1 = initOperand(operand1, functionLabel.value, TOKEN_ID, GF, false, true);
      oneOperandInstr(&instrList, JUMP, operand1);

      operand operand_label = initOperand(operand_label, functionName.value, TOKEN_ID, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand_label);

      noOperandInstr(&instrList, PUSHFRAME);

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
        return ERROR_CODE_INTERNAL;
      }
      else {
        //now we have to add paramnames to local symtable
        for (int i = 0; i < helper->parametrs; i++) {
          SYMInsert(&lcSymtable, helper->paramName[i], false);
        }
      }

      //generate parametres
      for (int i = 0; i < helper->parametrs; i++) {
        operand1 = initOperand(operand1, helper->paramName[i].value, TOKEN_ID, LF, false, false);
        oneOperandInstr(&instrList, DEFVAR, operand1);
        string operandS;
        stringInit(&operandS);
        stringAddChar(&operandS, '%');
        char number[3];
        sprintf(number, "%d", i);
        stringAddChars(&operandS, number);
        operand2 = initOperand(operand2, operandS.value, TOKEN_ID, LF, false, false);
        twoOperandInstr(&instrList, MOVE, operand1, operand2);
      }

      //going deeper to function body
      result = functionBody();
      if (result != ERROR_CODE_OK) return result;

      //there has to be dedent
      if (token.t_type != TOKEN_DEDENT) {
        return ERROR_CODE_SYN;
      }

      noOperandInstr(&instrList, RETURN);

      operand1 = initOperand(operand1, functionLabel.value, TOKEN_ID, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand1);

      stringClear(&functionName);
      SYMDispose(&lcSymtable);

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
          //result = SYMInsert(&glSymtable, functionName, true);
          //if (result != ERROR_CODE_OK) return result;
          numberOfPrevParams = helper->parametrs;
          declared = true;
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
          return ERROR_CODE_INTERNAL;
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
        if (numberOfPrevParams != paramIndex) {
          fprintf(stderr, "Funkce \"%s\" byla volana se spatnym poctem parametru.\n", functionName.value);
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
        //(helper->parametrs)++;

        //increment the index
        paramIndex++;

      }
      else {
        return ERROR_CODE_SEM_OTHER;
      }

      helper->parametrs = paramIndex;

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
      if (type == 8) {
        fprintf(stderr, "Vyraz za IF nemohl byt vyhodnocen.\n");
        return ERROR_CODE_SYN;
      }
      nowExpression = false;

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

      //vyhodnoceni vyrazu if
      //popnu hodnotu z vyrazu do tmp
      operand1 = initOperand(operand1, "", TOKEN_ID, GF, true, false);
      oneOperandInstr(&instrList, POPS, operand1); 

      operand1 = initOperand(operand1, "tmp1", TOKEN_ID, GF, false, false);
      operand2 = initOperand(operand2, "", TOKEN_ID, GF, true, false);
      twoOperandInstr(&instrList, TYPE, operand1, operand2);
      
      string lableOne;
      stringInit(&lableOne);
      stringAddChars(&lableOne, "compare_int%");
      char someInt[6];
      sprintf(someInt, "%d", labelCounter);
      stringAddChars(&lableOne, someInt);
      operand1 = initOperand(operand1, lableOne.value, LABEL, GF, false, true);
      operand2 = initOperand(operand2, "tmp1", TOKEN_ID, GF, false, false);
      operand3 = initOperand(operand3, "int", TOKEN_STRING, GF, false, false);
      threeOperandInstr(&instrList, JUMPIFEQ, operand1, operand2, operand3);
      

      string lableTwo;
      stringInit(&lableTwo);
      stringAddChars(&lableTwo, "compare_double%");
      char someInt2[6];
      sprintf(someInt2, "%d", labelCounter);
      stringAddChars(&lableTwo, someInt2);
      operand1 = initOperand(operand1, lableTwo.value, LABEL, GF, false, true);
      operand2 = initOperand(operand2, "tmp1", TOKEN_ID, GF, false, false);
      operand3 = initOperand(operand3, "float", TOKEN_STRING, GF, false, false);
      threeOperandInstr(&instrList, JUMPIFEQ, operand1, operand2, operand3);


      string lableTwoHalf;
      stringInit(&lableTwoHalf);
      stringAddChars(&lableTwoHalf, "compare_bool%");
      char someInt25[6];
      sprintf(someInt25, "%d", labelCounter);
      stringAddChars(&lableTwoHalf, someInt25);
      operand1 = initOperand(operand1, lableTwoHalf.value, LABEL, GF, false, true);
      operand2 = initOperand(operand2, "tmp1", TOKEN_ID, GF, false, false);
      operand3 = initOperand(operand3, "bool", TOKEN_STRING, GF, false, false);
      threeOperandInstr(&instrList, JUMPIFEQ, operand1, operand2, operand3);
      

      string lableThree;
      stringInit(&lableThree);
      stringAddChars(&lableThree, "continue%");
      char someInt3[6];
      sprintf(someInt3, "%d", labelCounter);
      stringAddChars(&lableThree, someInt3);

      operand1 = initOperand(operand1, lableThree.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, JUMP, operand1);

      //compare for int
      operand1 = initOperand(operand1, lableOne.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand1);

      string lableFour;
      stringInit(&lableFour);
      stringAddChars(&lableFour, "else%");
      char someInt4[6];
      sprintf(someInt4, "%d", labelCounter);
      stringAddChars(&lableFour, someInt4);
      operand1 = initOperand(operand1, lableFour.value, LABEL, GF, false, true);
      operand2 = initOperand(operand2, "", TOKEN_ID, GF, true, false);
      operand3 = initOperand(operand3, "0", TOKEN_INT, GF, false, false);
      threeOperandInstr(&instrList, JUMPIFEQ, operand1, operand2, operand3);

      operand1 = initOperand(operand1, lableThree.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, JUMP, operand1);

      //compare for double
      operand1 = initOperand(operand1, lableTwo.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand1);

      operand1 = initOperand(operand1, lableFour.value, LABEL, GF, false, true);
      operand2 = initOperand(operand2, "", TOKEN_ID, GF, true, false);
      operand3 = initOperand(operand3, "0x0p+0", TOKEN_DOUBLE, GF, false, false);
      threeOperandInstr(&instrList, JUMPIFEQ, operand1, operand2, operand3);

      operand1 = initOperand(operand1, lableThree.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, JUMP, operand1);

      //compare for bool
      operand1 = initOperand(operand1, lableTwoHalf.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand1);
      
      string tempstring;
      stringInit(&tempstring);
      stringAddChars(&tempstring, "$");
      stringAddChars(&tempstring, lableFour.value);
      addInstruction(&instrList, JUMPIFEQ, tempstring.value, "GF@tmp", "bool@false");

      //this is normal continue
      operand1 = initOperand(operand1, lableThree.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand1);


      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      result = commands();
      if (result != ERROR_CODE_OK) {
        return result;
      }

      if (token.t_type != TOKEN_DEDENT) {
        fprintf(stderr, "Za prikazem if se nevyskytuje token DEDENT.\n");
        return ERROR_CODE_SYN;
      }

      //there has to be jump to an end
      string lableFive;
      stringInit(&lableFive);
      stringAddChars(&lableFive, "end%");
      char someInt5[6];
      sprintf(someInt5, "%d", labelCounter);
      stringAddChars(&lableFive, someInt5);
      operand1 = initOperand(operand1, lableFive.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, JUMP, operand1);


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


      //else now
      operand1 = initOperand(operand1, lableFour.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand1);

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      result = commands();
      if (result != ERROR_CODE_OK) {
        return result;
      }      


      if (token.t_type != TOKEN_DEDENT) {
        fprintf(stderr, "Za prikazem else se nevyskytuje token DEDENT.\n");
        return ERROR_CODE_SYN;
      }

      operand1 = initOperand(operand1, lableFive.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand1);

      labelCounter++;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      return ERROR_CODE_OK;

    case TOKEN_WHILE:;
      //Prikaz -> while Vyraz : eol indent Sekvence_prikazu dedent

      //have to generate lable for another start while
      //there has to be jump to an end
      string lableStart;
      stringInit(&lableStart);
      stringAddChars(&lableStart, "start_while%");
      char someInt6[6];
      sprintf(someInt6, "%d", labelCounter);
      stringAddChars(&lableStart, someInt6);
      operand1 = initOperand(operand1, lableStart.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand1);

      nowExpression = true;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      VarType type_while;
      result = expression(&type_while);
      if (result != ERROR_CODE_OK) return result;
      if (type_while == 8) {
        fprintf(stderr, "Vyraz za WHILE nemohl byt vyhodnocen.\n");
        return ERROR_CODE_SYN;
      }
      nowExpression = false;

      if (token.t_type != TOKEN_DOUBLEDOT) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_EOL) return ERROR_CODE_SYN;

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      if (token.t_type != TOKEN_INDENT) return ERROR_CODE_SYN;


      //vyhodnoceni vyrazu while
      //popnu hodnotu z vyrazu do tmp
      operand1 = initOperand(operand1, "", TOKEN_ID, GF, true, false);
      oneOperandInstr(&instrList, POPS, operand1); 

      operand1 = initOperand(operand1, "tmp1", TOKEN_ID, GF, false, false);
      operand2 = initOperand(operand2, "", TOKEN_ID, GF, true, false);
      twoOperandInstr(&instrList, TYPE, operand1, operand2);

      //jumps for comparing exit value
      string lableInt;
      stringInit(&lableInt);
      stringAddChars(&lableInt, "compare_int_w%");
      char someIntw2[6];
      sprintf(someIntw2, "%d", labelCounter);
      stringAddChars(&lableInt, someIntw2);
      operand1 = initOperand(operand1, lableInt.value, LABEL, GF, false, true);
      operand2 = initOperand(operand2, "tmp1", TOKEN_ID, GF, false, false);
      operand3 = initOperand(operand3, "int", TOKEN_STRING, GF, false, false);
      threeOperandInstr(&instrList, JUMPIFEQ, operand1, operand2, operand3);
      

      string lableDouble;
      stringInit(&lableDouble);
      stringAddChars(&lableDouble, "compare_double_w%");
      char someInt2w[6];
      sprintf(someInt2w, "%d", labelCounter);
      stringAddChars(&lableDouble, someInt2w);
      operand1 = initOperand(operand1, lableDouble.value, LABEL, GF, false, true);
      operand2 = initOperand(operand2, "tmp1", TOKEN_ID, GF, false, false);
      operand3 = initOperand(operand3, "float", TOKEN_STRING, GF, false, false);
      threeOperandInstr(&instrList, JUMPIFEQ, operand1, operand2, operand3);


      string lableTwoHalfW;
      stringInit(&lableTwoHalfW);
      stringAddChars(&lableTwoHalfW, "compare_bool_w%");
      char someInt256[6];
      sprintf(someInt256, "%d", labelCounter);
      stringAddChars(&lableTwoHalfW, someInt256);
      operand1 = initOperand(operand1, lableTwoHalfW.value, LABEL, GF, false, true);
      operand2 = initOperand(operand2, "tmp1", TOKEN_ID, GF, false, false);
      operand3 = initOperand(operand3, "bool", TOKEN_STRING, GF, false, false);
      threeOperandInstr(&instrList, JUMPIFEQ, operand1, operand2, operand3);
      

      string lableThreeW;
      stringInit(&lableThreeW);
      stringAddChars(&lableThreeW, "continue_w%");
      char someInt3W[6];
      sprintf(someInt3W, "%d", labelCounter);
      stringAddChars(&lableThreeW, someInt3);

      operand1 = initOperand(operand1, lableThreeW.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, JUMP, operand1);





      //compare for int
      operand1 = initOperand(operand1, lableInt.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand1);

      string lableFourW;
      stringInit(&lableFourW);
      stringAddChars(&lableFourW, "end_w%");
      char someInt4W[6];
      sprintf(someInt4W, "%d", labelCounter);
      stringAddChars(&lableFourW, someInt4W);
      operand1 = initOperand(operand1, lableFourW.value, LABEL, GF, false, true);
      operand2 = initOperand(operand2, "", TOKEN_ID, GF, true, false);
      operand3 = initOperand(operand3, "0", TOKEN_INT, GF, false, false);
      threeOperandInstr(&instrList, JUMPIFEQ, operand1, operand2, operand3);

      operand1 = initOperand(operand1, lableThreeW.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, JUMP, operand1);

      //compare for double
      operand1 = initOperand(operand1, lableDouble.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand1);

      operand1 = initOperand(operand1, lableFourW.value, LABEL, GF, false, true);
      operand2 = initOperand(operand2, "", TOKEN_ID, GF, true, false);
      operand3 = initOperand(operand3, "0x0p+0", TOKEN_DOUBLE, GF, false, false);
      threeOperandInstr(&instrList, JUMPIFEQ, operand1, operand2, operand3);

      operand1 = initOperand(operand1, lableThreeW.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, JUMP, operand1);

      //compare for bool
      operand1 = initOperand(operand1, lableTwoHalfW.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand1);
      
      string tempstringW;
      stringInit(&tempstringW);
      stringAddChars(&tempstringW, "$");
      stringAddChars(&tempstringW, lableFourW.value);
      addInstruction(&instrList, JUMPIFEQ, tempstringW.value, "GF@tmp", "bool@false");

      //this is normal continue
      operand1 = initOperand(operand1, lableThreeW.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand1);

      //prikazy ve while
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      result = commands();
      if (result != ERROR_CODE_OK) {
        return result;
      }

      if (token.t_type != TOKEN_DEDENT) {
        fprintf(stderr, "Za prikazem while se nevyskytuje token DEDENT.\n");
        return ERROR_CODE_SYN;
      }

      operand1 = initOperand(operand1, lableStart.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, JUMP, operand1);

      operand1 = initOperand(operand1, lableFourW.value, LABEL, GF, false, true);
      oneOperandInstr(&instrList, LABEL, operand1);

      labelCounter++;

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
      
      nowExpression = true;
      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
      VarType type_return;
      result = expression(&type_return);
      if (result != ERROR_CODE_OK) return result;

      //todo retvalue?
      nowExpression = false;

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

        case TOKEN_BIGGERTHEN_EQUAL: ;
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

              //promenna jeste nebyla vytvorena -> vytvorim
              SYMInsert(&glSymtable, token.t_data.ID, false);

              helper = SYMSearch(&glSymtable, token.t_data.ID);

              

              //budu generovat instrukci pro vytvoreni promenne
              operand var_operand = initOperand(var_operand, token.t_data.ID.value, TOKEN_ID, GF, false, false);
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
              operand var_operand = initOperand(var_operand, token.t_data.ID.value, TOKEN_ID, LF, false, false);
              oneOperandInstr(&instrList, DEFVAR, var_operand);
            }
          }

          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //EOL
          if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //NEXT
          return ERROR_CODE_OK;

        case TOKEN_EQUAL:;
          //Pokracovani_id -> = Pokracovani_id_next
          //going into variable assignment

          //have to update symtable
          if (functionName.length == 0) {
            //now im in main program -> looking into global symtable
            string insertInGlobal;
            stringInit(&insertInGlobal);

            //make sure that this var is not in table
            tBSTNodePtr helper = SYMSearch(&glSymtable, token.t_data.ID);
            if (helper == NULL) {
              //promenna jeste nebyla vytvorena
              //later, variable will be added to symtable
              stringAddChars(&insertInGlobal, token.t_data.ID.value);

              //generate instruction for creating the variable
              operand var_operand = initOperand(var_operand, token.t_data.ID.value, TOKEN_ID, GF, false, false);
              oneOperandInstr(&instrList, DEFVAR, var_operand);
            }
            else {
              if (helper->DataType == Function) {
                fprintf(stderr, "Jiz byla definovana funkce s nazvem : \"%s\"\n", token.t_data.ID.value);
                return ERROR_CODE_SEM;
              }
            }

            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //=
            
            nowExpression = true;
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //vyraz

            //now we have to make sure that we get expression value
            if (token.t_type == TOKEN_EOL || token.t_type == TOKEN_EOF || token.t_type == TOKEN_DOUBLEDOT) {
              fprintf(stderr, "Nezadali jste zadnou hodnotu do prirazeni k vyrazu.\n");
              return ERROR_CODE_SYN;
            }

            //proceed the expression
            VarType sth;
            result = expression(&sth);
            if (result != ERROR_CODE_OK) return result;
            nowExpression = false;

            //now add var to symtable to avoid using it in expression
            if (insertInGlobal.length != 0) {
              result = SYMInsert(&glSymtable, insertInGlobal, false);
              if (result != ERROR_CODE_OK) return result;
              
              helper = SYMSearch(&glSymtable, insertInGlobal);
              if (helper == NULL) return ERROR_CODE_INTERNAL;
            }

            //update symtable type of variable
            helper->Vartype = sth;

            //generate POPS for assigment to variable
            operand var_operand = initOperand(var_operand, helper->Key.value, TOKEN_ID, GF, false, false);
            oneOperandInstr(&instrList, POPS, var_operand);

            stringDispose(&insertInGlobal);
          }
          else {
            //now im in function -> looking into local symtable
            string insertInLocal;
            stringInit(&insertInLocal);

            //make sure that this var is not in the table already
            tBSTNodePtr helper = SYMSearch(&lcSymtable, token.t_data.ID);
            if (helper == NULL) {
              //var is not in symtable yet -> create new in local
              //will have to update symtable after expression
              stringAddChars(&insertInLocal, token.t_data.ID.value);

              //generate code for cretaing the variable
              operand var_operand = initOperand(var_operand, token.t_data.ID.value, TOKEN_ID, LF, false, false);
              oneOperandInstr(&instrList, DEFVAR, var_operand);
            }
            
            tBSTNodePtr helper2 = SYMSearch(&glSymtable, token.t_data.ID);
            if (helper2 != NULL) {
              if (helper2->DataType == Function) {
              fprintf(stderr, "Jiz byla definovana funkce s nazvem : \"%s\"\n", token.t_data.ID.value);
              return ERROR_CODE_SEM;
              }
            }
            
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //=

            nowExpression = true;
            if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer; //vyraz

            //now we have to be sure, that next token will be expression-friendly
            if (token.t_type == TOKEN_EOL || token.t_type == TOKEN_EOF || token.t_type == TOKEN_DOUBLEDOT) {
              fprintf(stderr, "Nezadali jste zadnou hodnotu do prirazeni k vyrazu.\n");
              return ERROR_CODE_SYN;
            }

            VarType sth;
            result = expression(&sth);
            if (result != ERROR_CODE_OK) return result;
            nowExpression = false;

            //now add var to symtable to avoid using it in expression
            if (insertInLocal.length != 0) {
              result = SYMInsert(&lcSymtable, insertInLocal, false);
              if (result != ERROR_CODE_OK) return result;

              helper = SYMSearch(&lcSymtable, insertInLocal);
              if (helper == NULL) return ERROR_CODE_INTERNAL;
            }

            //update variable type in symtable
            helper->Vartype = sth;

            //generate code for assinment to variable (POPS)
            operand var_operand = initOperand(var_operand, helper->Key.value, TOKEN_ID, LF, false, false);
            oneOperandInstr(&instrList, POPS, var_operand);

            stringDispose(&insertInLocal);
          }
          return ERROR_CODE_OK;

        case TOKEN_LEFTPAR:
          //just a function call (without actually var to assign to) - procedure
          
          nowExpression = true;
          
          //if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;
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
      
      operand operand_bye = initOperand(operand_bye, "", TOKEN_ID, GF, true, false);
      oneOperandInstr(&instrList, POPS, operand_bye);
      

      if (((token = getNextToken(&line_flag, &s)).t_type) == TOKEN_UNDEF) return token.t_data.integer;

      return ERROR_CODE_OK;

    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

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
      if (result != ERROR_CODE_OK) {
        return result;
      }



      result = skipEol();
      if (result != ERROR_CODE_OK) return result;

      return commands();

    case TOKEN_DEDENT:
      return ERROR_CODE_OK;
    default:
      return ERROR_CODE_SYN;
  }
  return ERROR_CODE_SYN;
}

//finální rekurzivní kontrola, zda jsou všechny použité funkce definované
ERROR_CODE checkDefinedFunctions(struct tBSTNode* Root) {
  ERROR_CODE result;
  if (Root != NULL) {
    if (Root->DataType == Function) {
      if (Root->defined == false) {
        fprintf(stderr, "Funkce \"%s\" nebyla definovana.\n", Root->Key.value);
        return ERROR_CODE_SEM;
      }
      //je funkce, ale je definovana, takze chill
    }
    //neni null, ale je to promenna, takze chill
    result = checkDefinedFunctions(Root->lPtr);
    if (result != ERROR_CODE_OK) return result;
    result = checkDefinedFunctions(Root->rPtr);
    if (result != ERROR_CODE_OK) return result;
  }
  return ERROR_CODE_OK;
}