/**
 * @file err_handling.c
 * 
 * Error handling for PiL0-Compiler
 * 
 */

#include "../err_handling.h"
#include<stdio.h>
#include<stdlib.h>


/**
 * @var char *err_msg[]
 * @brief Stringtable which stores all error messages
 **/
static char *err_msg[] = {
    "Null-Pointer", "No memory left",
    "Empty list", "No table initialised"
  }; 
  
static char *parse_err_msg[] = {
    "Syntax-Error: Program must end with '.'",
    "Syntax-Error: Missing assign operator '='",
    "Syntax-Error: Missing identifier after CONST, VAR or PROCEDURE declaration",
    "Syntax-Error: Missing block seperator ';'",
    "Syntax-Error: Missing ',' seperator or ';' deliminator",
    "Syntax-Error: Missing END or statement seperator ';'",
    "Syntax-Error: After IF condition THEN statement must follow",
    "Syntax-Error: After WHILE condition DO statement must follow",
    "Syntax-Error: Wrong syntax in statement",
    "Syntax-Error: No compare operator",
    "Syntax-Error: Missing ')'",
    "Syntax-Error: Missing '('",
    "Type-Error: Can only assign number to constant",
    "Type-Error: Identifier not initialized",
    "Type-Error: No identifier given",
    "Type-Error: Can only call a procedure",
    "Type-Error: Operation only for Integer type",
    "Type-Error: Double declaration of identifier"
  };
  

/**
  * @brief Print error message.
  * @param msg_nr short string of the error message to print
  * @return void
  **/
void error(enum err_codes msg_nr) {
  fprintf(stderr, "ERROR(list): %s!\n", err_msg[msg_nr]);
  exit(10);
}

void parseError(int ln, enum parse_err_codes parse_err_nr) {
  fprintf(stderr, "%s in line %d!\n", parse_err_msg[parse_err_nr], ln);
  exit(10);
}