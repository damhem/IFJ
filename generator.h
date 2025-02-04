/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file generator.h
*
* @author Daniel Pátek (xpatek08)
*/

#ifndef IFJ_GENERATOR_H
#define IFJ_GENERATOR_H

#include "string.h"


#define STARTLABEL "$$start"
#define TMP "GF@tmp"
#define LABELSYMBOL "$"
#define FRAMEGF "GF@"
#define FRAMELF "LF@"
#define FRAMETF "TF@"
#define TYPEINT "int@"
#define TYPEBOOL "bool@"
#define TYPESTRING "string@"
#define TYPEDOUBLE "float@"

//types of instructions
typedef enum {
    START,
    COMMENT,

    //prace s ramci, volani funkci
    MOVE,             //var,symb
    CREATEFRAME,
    PUSHFRAME,
    POPFRAME,
    DEFVAR,           //var
    CALL,             //label
    RETURN,

    //prace s datovym zasobnnikem
    PUSHS,            //symb
    POPS,             //var
    CLEARS,           

    //aritmeticke instrukce
    ADD,              //var symb1 symb2
    ADDS,
    SUB,              //var symb1 symb2
    SUBS,
    MUL,              //var symb1 symb2
    MULS,
    DIV,              //var symb1 symb2
    DIVS,
    IDIV,             //var symb1 symb2
    IDIVS,

    //relacni instrukce
    LT,               //var symb1 symb2
    LTS,
    GT,               //var symb1 symb2
    GTS,
    EQ,               //var symb1 symb2
    EQS,

    //bool instrukce
    AND,              //var symb1 symb2
    ANDS,
    OR,               //var symb1 symb2
    ORS,
    NOT,              //var symb1 symb2
    NOTS,

    //konverzni instrukce
    INT2FLOAT,        //var symb
    INT2FLOATS,
    FLOAT2INT,        //var symb
    FLOAT2INTS,
    INT2CHAR,         //var symb
    INT2CHARS,
    STRI2INT,         //var symb1 symb2
    STRI2INTS,

    //vstupne vystupni instrukce
    READ,             //var type
    WRITE,            //symb

    //prace s retezci
    CONCAT,           //var symb1 symb2
    STRLEN,           //var symb
    GETCHAR,          //var symb1 symb2
    SETCHAR,          //var symb1 symb2

    //prace s typy
    TYPE,             //var symb

    //rizeni toku programu
    LABEL,            //label
    JUMP,             //label
    JUMPIFEQ,         //label symb1 symb2
    JUMPIFEQS,        //label
    JUMPIFNEQ,        //label symb1 symb2
    JUMPIFNEQS,       //label
    EXIT,             //symb

    //ladici instrukce
    BREAK,
    DPRINT            //symb
} inst_type;

typedef enum {
    GF,
    LF,
    TF
} frame;

//struct for instruction
typedef struct instruction {
    inst_type instructionType;
    void *adress1;
    void *adress2;
    void *adress3;
} instruction;



//struct for operand
typedef struct operand {
    string value;
    int type;           
    frame frame;
    bool is_temp;
    bool is_label;
} operand;

//instruction as part of the list
typedef struct instruction_node {
    instruction inst;
    struct instruction_node *lptr;
    struct instruction_node *rptr;
} *instruction_node;

//instruction list
typedef struct instruction_list {
    instruction_node First;
    instruction_node Actual;
    instruction_node Last;
} instruction_list;

//declaration of three help vars for generating instruction
operand operand1;
operand operand2;
operand operand3;

//function declaration
void addInstruction(struct instruction_list *List, int instType, void *adress1, void *adress2, void *adress3);
void noOperandInstr(struct instruction_list *List, inst_type type);
operand initOperand(operand operand, char* value, int type, frame frame, bool is_temp, bool is_label);
void printInstructionList(instruction_list *List);
void oneOperandInstr(instruction_list *List, int instType, operand first);
void twoOperandInstr(instruction_list *L, int instType, operand first, operand second);
void threeOperandInstr(instruction_list *L, int instType, operand first, operand second, operand third);

void lenFunction();
void ordFunction();
void chrFunction();
void printFunction();
void inputSFunction();
void inputIFunction();
void inputFFunction();


#include "parser.h"

#endif