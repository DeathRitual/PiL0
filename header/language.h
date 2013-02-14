/**
 * @file language.h Header-File for language definition
 * 
 * All entries in this file defines the language. 
 * The stringtable keyworss will define all keywords. Later there will be added structs which
 * represents syntax daigrams of the language.
 */

#include<stddef.h>
#ifndef __LANGUAGE_H

/**
 * @var char *keywords[]
 * @brief Stringtable of all keywords longer than 1 character
 * 
 **/
char *keywords[] = {
  "BEGIN", "CALL", "CONST", "DO", "END", "IF", "ODD", "PRINT", "PROCEDURE",
  "READ", "THEN", "VAR", "WHILE", "PASS", "EQ", "GE", "LE", "NE", NULL
};

#endif