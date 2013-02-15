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
    "Null-Pointer", "Kein Speicher frei",
    "Liste ist leer"
  }; 
  

/**
  * @brief Print error message.
  * @param msg_nr short string of the error message to print
  * @return void
  **/
void error(enum err_codes msg_nr) {
  fprintf(stderr, "FEHLER(intlist): %s!\n", err_msg[msg_nr]);
  exit(10);
}