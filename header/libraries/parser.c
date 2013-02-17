/**
 * @file parser.c Library which inherits all necessary functions for parsing
 * 
 * Further instructions
 */

#include"../err_handling.h"
#include"../parser.h" 
#include<stdlib.h>
#include<string.h>

/**
 * @brief Initialize symbol-table environment with NULL-Pointer
 *
 * @param e pointer to symbol-table environment
 * @return void
 **/
void st_init(env_ptr *e) {
  *e = NULL;
}

/**
 * @brief Initialize symbol-table with NULL-Pointer
 *
 * @param st Pointer to symbol-table
 * @return void
 **/
void table_init(table_ptr *st){
  *st = NULL;
}

/**
 * @brief Add new symbol-table environment
 *
 * @param e Pointer to current symbol-table environment
 * @return void
 * 
 * If no symbol-table environment is initialized then the previous pointer will point to NULL. 
 * Otherwise the new element will point to the current one.
 * When adding a new enviornment a new symbol table with table_init(&el->st) will be initialized.
 **/
void st_append(env_ptr *e) {
  env *el;
  el = malloc(sizeof(env));
  table_init(&el->st);
  if (*e == NULL) el->previous = NULL;
  else el->previous = *e;
  *e = el;
}

/**
 * @brief Add a new entry to the symbol table
 *
 * @param t Identifies the current symbol table.
 * @param w Symbol which should be stored
 * @param i ID of the symbol
 * @return void
 **/
void put(table_ptr *t, char *w, int *i) {
  table *el;
  el = malloc(sizeof(table));
  strcpy(el->word, w);
  el->type_ID = *i;
  if (*t == NULL) el->previous = NULL;
  else el->previous = *t;
  *t = el;
}

/**
 * @brief Look if symbol was already added to the symbol table
 *
 * @param e Pointer to table enviornment
 * @param s Symbol looking for
 * @return table_ptr If symbol is found pointer to this symbol, otherwise NULL
 **/
table_ptr get(env_ptr *e, char *s) {
  table_ptr found = (*e)->st;
  while (strcmp(found->word, s) != 0) {
    if (found->previous != NULL) found = found->previous;
    else if ((*e)->previous != NULL) {
      *e = (*e)->previous;
      found = (*e)->st;
    } else {
      found = NULL;
      break;
    }
  }
  return found;  
}

 