/*     
 * PiL0 - PL0 Compiler for Raspberry PI
 * Copyright (C) 2013  Philipp Wiesner
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file parser.c Library which inherits all necessary functions for parsing
 * 
 * @todo add further instructions, abstract syntax tree and output of three adress code
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
 * @var table* var
 * @brief compare pointer for identifier in symbol table
 **/
table_ptr var;

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
 * @param i varaible-type ID
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
  while (ptr != NULL) {
    table_ptr found = ptr->st; 
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
  st_append(&environment);
  /* block    -> VAR var_stmt 
   * var_stmt -> var_stmt, identifier | identifier */
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
  
  /* block      -> CONST const_stmt 
   * const_stmt -> const_stmt, identifier = number | identifier = number */
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
  
  /* block     -> proc  
   * proc      -> proc proc_stmt | proc_stmt
   * proc_stmt -> PROCEDURE identifier ; block ; */
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
    /* stmt -> identifier = expression */
    case (IDENTIFIER): 	if ((var = get(&environment, WORD)) == NULL) parseError(CODE, TYP_ID_NO_IN);
			MOVE
			if (TOKEN == '=') { MOVE } else parseError(CODE, SYN_MISS_ASS);
			expression(l);
			break;
    /* stmt -> CALL identifier (only procedure)*/
    case (CALL):	MOVE
			var = get(&environment, WORD);
			if (var == NULL) parseError(CODE, TYP_ID_NO_IN);
			else if (var->type_ID != PROCEDURE) parseError(CODE, TYP_ONLY_PROC);
			MOVE
			break;
    /* stmt -> READ identifier (only procedure)*/
    case (READ):	MOVE
			var = get(&environment, WORD);
			if (var == NULL) parseError(CODE, TYP_ID_NO_IN);
			if (WORDID == IDENTIFIER && var->type_ID != PROCEDURE) { MOVE } else parseError(CODE, TYP_ONLY_INT);			
			break;
    /* stmt -> PRINT expression */
    case (PRINT):	MOVE
			expression(l);
			break;
    /* stmt  -> BEGIN stmts END 
     * stmts -> stmts ; stmt | stmt */
    case (BEGIN):	MOVE
			stmt(l);
			while (TOKEN == ';') {
			  MOVE
			  stmt(l);
			}
			if (WORDID == END) { MOVE } else parseError(CODE, SYN_MISS_END);
			break;
    /* stmt -> IF condition THEN stmt */
    case (IF):		MOVE
			condition(l);
			if (WORDID == THEN) { MOVE } else parseError(CODE, SYN_IF);
			stmt(l);
			break;
    /* stmt -> WHILE condition DO stmt */
    case (WHILE):	MOVE
			condition(l);
			if (WORDID == DO) { MOVE } else parseError(CODE, SYN_WHILE);
			stmt(l);
			break;
    /* stmt -> PASS */
    case (PASS):	MOVE
			break;
  }
}

/**
 * @brief check condition syntax
 *
 * @param l token stream
 * @return void
 **/
void condition(list l) {
  /* condition -> ODD expression */
  if (WORDID == ODD) { 
    MOVE 
    expression(l);
  } 
  else {
    /* condition -> expression > expression | expression < expression */
    expression(l);   
    if (TOKEN == '>' || TOKEN == '<') {      
      MOVE
      expression(l);
    } else {
      switch(WORDID) {
	/* condition -> expression == expression */
	case(EQ): MOVE
		  expression(l);
		  break;
	/* condition -> expression != expression */
	case(NE): MOVE
		  expression(l);
		  break;
	/* condition -> expression <= expression */
	case(LE): MOVE
		  expression(l);
		  break;
	/* condition -> expression >= expression */
	case(GE): MOVE
		  expression(l);
		  break;
	default: parseError(CODE, SYN_NO_COMP);
      }
    }
  }
}

/**
 * @brief check expression syntax
 *
 * @param l token stream
 * @return void
 **/
void expression(list l) {
  /* expression -> - term */
  if (TOKEN == '-') { 
    MOVE
    term(l);
  /* expression -> term */
  } else {
    term(l);
  }
  /* expression -> expression + term | expression - term */
  while (TOKEN == '+' || TOKEN == '-') {
    MOVE
    term(l);
  }
}

/**
 * @brief check term syntax
 *
 * @param l token stream
 * @return void
 **/
void term(list l) {
  /* term -> factor */
  factor(l);
  /* term -> term * factor | term / factor */
  while (TOKEN == '*' || TOKEN == '/') {
    MOVE
    term(l);
  }
}

/**
 * @brief check factor syntax
 *
 * @param l token stream
 * @return void
 **/
void factor(list l) {
  /* factor -> identifier */
  if (WORDID == IDENTIFIER) {
    if ((var = get(&environment, WORD)) == NULL) parseError(CODE, TYP_ID_NO_IN);
    MOVE
  /* factor -> number */
  } else if (NUMBERID == NUM) { MOVE }
  /* factor -> ( expression ) */
  else if (TOKEN == '(') {
    MOVE
    expression(l);
    if (TOKEN == ')') { MOVE } else parseError(CODE, SYN_MISS_CB);
  } else parseError(CODE, SYN_MISS_OB);
}
 