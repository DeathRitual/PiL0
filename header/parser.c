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
 * 
 * @defgroup parser Parser module
 * @ingroup frontend
 */

#include"err_handling.h"
#include"frontend.h" 
#include"ast.h"
#define DEBUG switch(tok->type) { \
		  case('t'): printf("Token: %c\n", tok->element.token.t); \
			     break; \
		  case('w'): printf("Word: %s, ID: %d\n", tok->element.word.w, tok->element.word.ID); \
			     break; \
		  case('n'): printf("Nummer: %d, ID: %d\n", tok->element.number.n, tok->element.number.ID); \
			     break; \
		  } 
#define TRUE	1
#define FALSE 	0
#define MOVE tok = l_remove(&l);
//	     DEBUG
#define WORD tok->element.word.w
#define WORDID tok->element.word.ID
#define TOKEN tok->element.token.t
#define NUMBER tok->element.number.n 
#define NUMBERID tok->element.number.ID
#define CODE tok->line

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
  char word[MAX_LENGTH]; /**< symbol name */
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
static void st_init(env_ptr *e) {
  if (e == NULL) error(NULL_POINTER);
  
  *e = NULL;
}

/**
 * @brief Initialize symbol-table with NULL-Pointer
 *
 * @param st Pointer to symbol-table
 * @return void
 **/
static void table_init(table_ptr *st){
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
static void st_append(env_ptr *e) {
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
static void put(table_ptr *t, const char *w, const int i) {
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
static table_ptr get(const env_ptr *e, const char *s) {  
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
static void st_clean(env_ptr *e) {
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

/* dummies */
int parse(list);
void block(list);
void stmt(list);
void condition(list);
void expression(list);
void term(list);
void factor(list);

rootBlock block_ptr = NULL;
rootStmt stmt_ptr = NULL;
rootExpr expr_ptr = NULL;

/**
 * @brief start with parsing process
 * 
 * @param l pointer to token stream
 * @return int TRUE:1 for correct parsing and FALSE:0 for wrong parsing
 **/
int parse(list l) {
  if (l == NULL) error(NULL_POINTER);
  tok = l_top(l);
  //DEBUG
  block_ptr = initNewBlock();
  st_init(&environment); 
  block(l); 
  if(TOKEN == '.') return TRUE;
  else return FALSE;
}


/**
 * @brief check block grammar
 *
 * @param l pointer to token stream
 * @return void
 **/
void block(list l) {
  env_ptr env_tmp = NULL;
  rootBlock block_tmp = NULL;
  char g[10];
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
      //printf("%s", t);
      block_ptr = newBlock(&block_ptr, environment->st->word, _PROC_);
      MOVE
    } else parseError(CODE, TYP_NO_ID);
    if (TOKEN == ';') { MOVE } else parseError(CODE, SYN_MISS_COM);
    env_tmp = environment;				/* save environment for current iteration */
    block_tmp = block_ptr->block.proc.external_block;	/* save external branch for current iteration */
    block_ptr = block_ptr->block.proc.internal_block;	/* create subtree of function below internal branch */
    block(l);
    block_ptr = block_tmp;				/* use external branch */
    environment = env_tmp;				/* set environment to the last one */
    if (TOKEN == ';') { MOVE } else parseError(CODE, SYN_MISS_COM);
  }
  block_ptr = newBlock(&block_ptr, NULL, _STMT_);
  stmt_ptr = block_ptr->block.stmt;
  stmt(l);  
  st_clean(&environment);
}

/**
 * @brief check statement syntax
 *
 * @param l pointer to token stream
 * @return void
 **/
void stmt(list l) {
  rootStmt stmt_tmp = NULL;
  switch(WORDID) {
    /* stmt -> identifier = expression */
    case (IDENTIFIER): 
      
      if ((var = get(&environment, WORD)) == NULL) parseError(CODE, TYP_ID_NO_IN);
      stmt_ptr = newStmt(&stmt_ptr, var->word, _ASSIGN_);
      expr_ptr = stmt_ptr->stmt.assign.expr;
      MOVE
      if (TOKEN == '=') { MOVE } else parseError(CODE, SYN_MISS_ASS);
      expression(l);
      break;
    /* stmt -> CALL identifier (only procedure)*/
    case (CALL):
       
      MOVE
      var = get(&environment, WORD);
      if (var == NULL) parseError(CODE, TYP_ID_NO_IN);
      else if (var->type_ID != PROCEDURE) parseError(CODE, TYP_ONLY_PROC);
      stmt_ptr = newStmt(&stmt_ptr, var->word, _CALL_);
      MOVE
      break;
    /* stmt -> READ identifier (only procedure)*/
    case (READ):	
      
      MOVE
      var = get(&environment, WORD);
      if (var == NULL) parseError(CODE, TYP_ID_NO_IN);
      if (WORDID == IDENTIFIER && var->type_ID != PROCEDURE) { MOVE } else parseError(CODE, TYP_ONLY_INT);
      stmt_ptr = newStmt(&stmt_ptr, var->word, _READ_);
      break;
    /* stmt -> PRINT expression */
    case (PRINT):	
      
      stmt_ptr = newStmt(&stmt_ptr, NULL, _PRINT_);
      expr_ptr = stmt_ptr->stmt.expr;
      MOVE
      expression(l);
      break;
    /* stmt  -> BEGIN stmts END 
     * stmts -> stmts ; stmt | stmt */
    case (BEGIN):	
      
      MOVE
      stmt_ptr = newStmt(&stmt_ptr, NULL, _SEQ_);
      stmt_tmp = stmt_ptr->stmt.seq.stmtLeft;		/* save left branch for current iteration */
      stmt_ptr = stmt_ptr->stmt.seq.stmtRight;		/* use right branch */
      stmt(l);
      stmt_ptr = stmt_tmp;				/* use left branch */
      while (TOKEN == ';') {
	MOVE
	stmt_ptr = newStmt(&stmt_ptr, NULL, _SEQ_);
	stmt_tmp = stmt_ptr->stmt.seq.stmtLeft;		/* save left branch for current iteration */
	stmt_ptr = stmt_ptr->stmt.seq.stmtRight;	/* use right branch */
	stmt(l);
	stmt_ptr = stmt_tmp;				/* use left branch */
      }
      if (WORDID == END) { MOVE } else parseError(CODE, SYN_MISS_END);
      break;
    /* stmt -> IF condition THEN stmt */
    case (IF):		
      
      MOVE
      stmt_ptr = newStmt(&stmt_ptr, NULL, _IF_);
      expr_ptr = stmt_ptr->stmt._if.condition;
      condition(l);
      if (WORDID == THEN) { MOVE } else parseError(CODE, SYN_IF);
      stmt_ptr = stmt_ptr->stmt._if.stmt;
      stmt(l);
      break;
    /* stmt -> WHILE condition DO stmt */
    case (WHILE):	
      
      MOVE
      stmt_ptr = newStmt(&stmt_ptr, NULL, _WHILE_);
      expr_ptr = stmt_ptr->stmt._if.condition;
      condition(l);
      if (WORDID == DO) { MOVE } else parseError(CODE, SYN_WHILE);
      stmt_ptr = stmt_ptr->stmt._if.stmt;
      stmt(l);
      break;
    /* stmt -> PASS */
    case (PASS):	
      
      MOVE
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
  rootExpr expr_tmp1 = NULL, expr_tmp2 = NULL;
  /* condition -> ODD expression */
  if (WORDID == ODD) {  
    MOVE 
    expr_ptr = newExpr(&expr_ptr, NULL, NULL, _ODD_);
    expr_ptr = expr_ptr->expr.odd.expr;
    expression(l);
  } 
  else {
    /* condition -> expression > expression | expression < expression */
    expr_ptr = newExpr(&expr_ptr, "", NULL, _REL_);
    expr_tmp2 = expr_ptr;				/* save current knot for later operation inserting */
    expr_tmp1 = expr_ptr->expr.rel.exprLeft;		/* save left branch for current iteration */
    expr_ptr = expr_ptr->expr.rel.exprRight;		/* use right branch */
    expression(l); 
    expr_ptr = expr_tmp1;				/* use left branch */
    if (TOKEN == '>' || TOKEN == '<') {
      expr_tmp2->expr.rel.op[0] = TOKEN;
      MOVE
      expression(l);
    } else {
      switch(WORDID) {
	/* condition -> expression == expression */
	case(EQ): 
	  
	  strcpy(expr_tmp2->expr.rel.op, WORD);
	  MOVE
	  expression(l);
	  break;
	/* condition -> expression != expression */
	case(NE): 
	  
	  strcpy(expr_tmp2->expr.rel.op, WORD);
	  MOVE
	  expression(l);
	  break;
	/* condition -> expression <= expression */
	case(LE): 
	  
	  strcpy(expr_tmp2->expr.rel.op, WORD);
	  MOVE
	  expression(l);
	  break;
	/* condition -> expression >= expression */
	case(GE): 
	  
	  strcpy(expr_tmp2->expr.rel.op, WORD);
	  MOVE
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
  rootExpr expr_tmp1 = NULL, expr_tmp2 = NULL;
  /* expression -> - term */
  if (TOKEN == '-') { 
    expr_ptr = newExpr(&expr_ptr, &TOKEN, NULL, _UNARY_);
    expr_ptr = expr_ptr->expr.unary.expr;
    expr_ptr = newExpr(&expr_ptr, "", NULL, _ARITH_);
    expr_tmp2 = expr_ptr;				/* save current knot for later operation inserting */
    expr_tmp1 = expr_ptr->expr.arith.exprLeft;		/* save left branch for current iteration */
    expr_ptr = expr_ptr->expr.arith.exprRight;		/* use right branch */
    MOVE
    term(l);
  /* expression -> term */
  } else {
    expr_ptr = newExpr(&expr_ptr, "", NULL, _ARITH_);
    expr_tmp2 = expr_ptr;				/* save current knot for later operation inserting */
    expr_tmp1 = expr_ptr->expr.arith.exprLeft;		/* save left branch for current iteration */
    expr_ptr = expr_ptr->expr.arith.exprRight;		/* use right branch */
    term(l);
  }
  expr_ptr = expr_tmp1;					/* use left branch */
  /* expression -> expression + term | expression - term */
  while (TOKEN == '+' || TOKEN == '-') {
    expr_tmp2->expr.arith.op = TOKEN;
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
  rootExpr expr_tmp1 = NULL, expr_tmp2 = NULL;
  /* term -> factor */
  expr_ptr = newExpr(&expr_ptr, "", NULL, _ARITH_);
  expr_tmp2 = expr_ptr;				/* save current knot for later operation inserting */
  expr_tmp1 = expr_ptr->expr.arith.exprLeft;		/* save left branch for current iteration */
  expr_ptr = expr_ptr->expr.arith.exprRight;		/* use right branch */
  factor(l);
  expr_ptr = expr_tmp1;					/* use left branch */
  /* term -> term * factor | term / factor */
  while (TOKEN == '*' || TOKEN == '/') {
    expr_tmp2->expr.arith.op = TOKEN;
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
    expr_ptr = newExpr(&expr_ptr, var->word, NULL, _IDENTIFIER_);
    MOVE
  /* factor -> number */
  } else if (NUMBERID == NUM) { 
    expr_ptr = newExpr(&expr_ptr, "", &NUMBER, _NUMBER_);
    MOVE 
  /* factor -> ( expression ) */
  } else if (TOKEN == '(') {
    MOVE
    expression(l);
    if (TOKEN == ')') { MOVE } else parseError(CODE, SYN_MISS_CB);
  } else parseError(CODE, SYN_MISS_OB);
}









 


 



