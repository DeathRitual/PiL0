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
void table_append(table_ptr *t, char *w, int *i) {
  table *el;
  el = malloc(sizeof(table));
  strcpy(el->word, w);
  el->type_ID = *i;
  if (*t == NULL) el->previous = NULL;
  else el->previous = *t;
  *t = el;
}

/*
 * TODO:	Add function which searches if a symbol exists in current or any symbol table before.
 * 		If the symbol exists then its ID should be returned otherwise NULL should be returned.
 */ 