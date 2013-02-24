/**
 * @file parser.h Header-File for Parser
 * 
 * Forwards the functions of the parser library to another file which includes this header file.
 */

#include"lexer.h"
#ifndef _PARSER_H
  #define _PARSER_H

/**
 * @typedef struct _table table
 * @brief shortens struct _table to table
 * 
 */   
typedef struct _table table;
/**
 * @typedef table *table_ptr
 * @brief short form to create pointer on table
 */ 
typedef table *table_ptr;  
/**
 * @typedef struct _env env
 * @brief shortens struct _env to env
 * 
 */ 
typedef struct _env env;
/**
 * @typedef env *env_ptr
 * @brief short form to create pointer on env
 * 
 */ 
typedef env *env_ptr;

/**
 * @struct _table
 * 
 * @brief Linked list which stores symbols
 * 
 **/
struct _table {
  char word[30]; /**< symbol name */
  unsigned int type_ID; /**< symbol ID */
  table *previous; /**< points to previous entry */
};
 
/**
 * @struct _env
 * 
 * @brief Linked list which stores one symbol-table in each element
 * 
 **/
struct _env {
  table_ptr st; /**< points to symbol-table */ 
  env *previous; /**< points to previous element */
};



extern void st_init(env_ptr *);
extern void table_init(table_ptr *);
extern void st_append(env_ptr *);
extern void put(table_ptr *, char *, int);
extern table_ptr get(env_ptr *, char *);
extern int parse(list);
void block(list);
void stmt(list);
void expression(list);
void condition(list);

#endif