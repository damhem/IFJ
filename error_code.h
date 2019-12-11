/**
* Projekt IFJ/IAL 2019 - Překladač imperativního jazyka IFJ19
*
* @file error_code.h
*
* @author Daniel Pátek (xpatek08)
*/

#ifndef IFJ_ERROR_CODE_H
#define IFJ_ERROR_CODE_H

#include <stdio.h>

typedef enum error_code{
    ERROR_CODE_OK = 0,
    ERROR_CODE_LEX = 1, //lexical analysis error
    ERROR_CODE_SYN  = 2, //syntactic analysis error
    ERROR_CODE_SEM = 3, // semantic analysis error
    ERROR_CODE_SEM_COMP = 4, //runtime error - operator compatibility
    ERROR_CODE_SEM_FUNC = 5, //semantic error - bad number of args
    ERROR_CODE_SEM_OTHER = 6, //other semantic errors
    ERROR_CODE_ZERO_DEV = 9, //deviding zero
    ERROR_CODE_INTERNAL = 99, //internal errors (bad allocation, ..)
} ERROR_CODE;

#endif
