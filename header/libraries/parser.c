/**
 * @file parser.c Library which inherits all necessary functions for parsing
 * 
 * Further instructions
 */

#include"../err_handling.h"
#include"../parser.h" 
#include<stdlib.h>
#include<string.h>
#define DEBUG switch(tok->element.type) { \
		  case('t'): printf("Token: %c\n", tok->element.token.t); \
			     break; \
		  case('w'): printf("Word: %s, ID: %d\n", tok->element.word.w, tok->element.word.ID); \
			     break; \
		  case('n'): printf("Nummer: %d, ID: %d\n", tok->element.number.n, tok->element.number.ID); \
			     break; \
		  } 
#define TRUE	1
#define FALSE 	0
#define MOVE tok = l_remove(&l);\
	     DEBUG
#define WORD tok->element.word.w
#define WORDID tok->element.word.ID
#define TOKEN tok->element.token.t
#define NUMBER tok->element.number.n 
#define NUMBERID tok->element.number.ID
#define CODE tok->element.line

/**
 * @var token_stream *tok
 * @brief element to store current symbol returned by token stream
 **/
list tok;
/**
 * @var env* environment
 * @brief start pointer for symbol table
 **/
env_ptr environment;

/**
 * @brief Initialize symbol-table environment with NULL-Pointer
 *
 * @param e pointer to symbol-table environment
 * @return void
 **/
void st_init(env_ptr *e) {
  if (e == NULL) error(NULL_POINTER);
  
  *e = NULL;
}

/**
 * @brief Initialize symbol-table with NULL-Pointer
 *
 * @param st Pointer to symbol-table
 * @return void
 **/
void table_init(table_ptr *st){
  if (st == NULL) error(NULL_POINTER);
  
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
  if (e == NULL) error(NULL_POINTER);
  
  env_ptr el;
  if ((el = malloc(sizeof(env))) == NULL) error(ERR_MEMORY);
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
void put(table_ptr *t, char *w, int i) {
  if (t == NULL) error(NULL_POINTER);
  
  table_ptr el;
  if ((el = malloc(sizeof(table))) == NULL) error(ERR_MEMORY);
  strcpy(el->word, w);
  el->type_ID = i;
  if (*t == NULL) el->previous = NULL;
  else el->previous = *t;
  *t = el;
}

/**
 * @brief Look if symbol was already added to the symbol table
 * 
 * @param e Pointer to table environment
 * @param s Symbol looking for
 * @return table_ptr If symbol is found pointer to this symbol, otherwise NULL
 **/
table_ptr get(env_ptr *e, char *s) {  
  if (e == NULL) error(NULL_POINTER); 
  if (*e == NULL) error(NO_TABLE);
  
  env_ptr ptr = *e;
  table_ptr found = ptr->st;
  
  
  while (ptr != NULL) {
    while (found != NULL) {
      if (strcmp(found->word, s) == 0) return found;
      found = found->previous;
    }
    ptr = ptr->previous;
  }

  return NULL;
}

/**
 * @brief clean symbol table after lexical translation of each block
 *
 * @param e pointer to symbol table
 * @return void
 **/
void st_clean(env_ptr *e) {
  if (e == NULL) error(NULL_POINTER);
  table_ptr t_ptr;
  if ((*e)->st == NULL) free(*e);
  else {
    while ((*e)->st->previous != NULL) {
      t_ptr = (*e)->st->previous;
      (*e)->st->previous = NULL;
      free((*e)->st);
      (*e)->st = t_ptr;
    }
    free((*e)->st);
    free(*e);
  }
  
}


/**
 * @brief start with parsing process
 * 
 * @param l pointer to token stream
 * @return int TRUE:1 for correct parsing and FALSE:0 for wrong parsing
 **/
int parse(list l) {
  if (l == NULL) error(NULL_POINTER);
  tok = l_top(l);
  DEBUG
  st_init(&environment);  
  block(l); 
  if(TOKEN == '.') return TRUE;
  else return FALSE;
}
 
/**
 * @brief check block grammar
 * 
 * @todo add syntax diagram, document steps more properly
 *
 * @param l pointer to token stream
 * @return void
 **/
void block(list l) {
  env_ptr env_tmp;
  table_ptr var;
  st_append(&environment);
  /* VAR definitons */
  if (WORDID == VAR) {
    MOVE
    do {     
      if (WORDID == IDENTIFIER) {
	if ((var = get(&environment, WORD)) == NULL) put(&environment->st, WORD, VAR);
	else parseError(CODE, TYP_DOUB_DEC);
	MOVE
      } else parseError(CODE, TYP_NO_ID);
      if (TOKEN == ',') { MOVE } 
      else if (TOKEN != ';') parseError(CODE, SYN_MISS_COM);
    } while (TOKEN != ';');
    MOVE
  }
  
  /* CONST declaration */
  if (WORDID == CONST) {
    MOVE
    do {
      if (WORDID == IDENTIFIER) {
	if ((var = get(&environment, WORD)) == NULL) put(&environment->st, WORD, CONST);
	else parseError(CODE, TYP_DOUB_DEC);
	MOVE
      } else parseError(CODE, TYP_NO_ID);
      if (TOKEN == '=') { MOVE } else parseError(CODE, SYN_MISS_ASS);
      if (NUMBERID == NUM) { MOVE } else parseError(CODE, TYP_CONST_NUM);
      if (TOKEN == ',') { MOVE }
      else if (TOKEN != ';') parseError(CODE, SYN_MISS_COM);
    } while (TOKEN != ';'); 
    MOVE
  }
  
  /* Procedure definition */
  while (WORDID == PROCEDURE) {
    MOVE
    if (WORDID == IDENTIFIER) {
      if ((var = get(&environment, WORD)) == NULL) put(&environment->st, WORD, PROCEDURE);
	else parseError(CODE, TYP_DOUB_DEC);
      MOVE
    } else parseError(CODE, TYP_NO_ID);
    if (TOKEN == ';') { MOVE } else parseError(CODE, SYN_MISS_COM);
    env_tmp = environment;
    block(l);
    environment = env_tmp;
    if (TOKEN == ';') { MOVE } else parseError(CODE, SYN_MISS_COM);
  }
  stmt(l);  
  st_clean(&environment);
}

/**
 * @brief check statement syntax
 * 
 * @todo add functions
 *
 * @param l pointer to token stream
 * @return void
 **/
void stmt(list l) {
  switch(WORDID) {
    case (IDENTIFIER): 	MOVE
			if (TOKEN == '=') MOVE
			expression(l);
			break;
    case (CALL):	MOVE
			if (WORDID == IDENTIFIER) MOVE
			break;
    case (READ):	MOVE
			if (WORDID == IDENTIFIER) MOVE
			break;
    case (PRINT):	MOVE
			expression(l);
			break;
    case (BEGIN):	MOVE
			stmt(l);
			if (WORDID == END) MOVE
			break;
    case (IF):		MOVE
			condition(l);
			if (WORDID == THEN) MOVE
			stmt(l);
			break;
    case (WHILE):	MOVE
			condition(l);
			if (WORDID == DO) MOVE
			stmt(l);
			break;
    case (PASS):	MOVE
			break;
  }
 /* if (TOKEN == ';') { 
    MOVE
    stmt(l);
  }*/

}

void expression(list l) {
  MOVE
}

void condition(list l) {
  MOVE
}
 