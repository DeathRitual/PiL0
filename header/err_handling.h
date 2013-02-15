/**
 * @file err_handling.h Header-File for error handling library
 * 
 * Forwards the functions of the error handling library to another file which includes this header file.
 */

#ifndef __ERR_HANDLING_H
  #define __ERR_HANDLING_H

/**
 * @enum err_codes short strings used as variables for error messages
 */
enum err_codes {
    NULL_POINTER, ERR_MEMORY, EMPTY_LIST
  };  
  
extern void error(enum err_codes);

#endif