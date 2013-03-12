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
#define MOVE 	tok = meta_list_top(ml);\
		meta_list_remove(&ml);
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
 * @var token_stream *tok
 * @brief element to store current symbol returned by token stream
 **/
ml_ptr tok;

/**
 * @var table* var
 * @brief compare pointer for identifier in symbol table
 **/
table_ptr var;



/**
 * @brief Initialize symbol-table with NULL-Pointer
 *
 * @param st Pointer to symbol-table
 * @return void
 **/
static void table_init(ml_ptr *el){
  if (el == NULL) error(NULL_POINTER);
  
  (table_ptr) (*el)->content;
  (*el)->content = NULL;
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
static table_ptr get(const ml_ptr *e, const char *s) {  
  if (e == NULL) error(NULL_POINTER); 
  if (*e == NULL) error(NO_TABLE);
  ml_ptr ptr = *e;   
  while (ptr != NULL) {
    table_ptr found = ptr->content; 
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
int parse(ml_ptr);
void block(ml_ptr);
void stmt(ml_ptr);
void condition(ml_ptr);
void expression(ml_ptr);
void term(ml_ptr);
void factor(ml_ptr);

rootBlock block_ptr = NULL;
rootStmt stmt_ptr = NULL;
rootExpr expr_ptr = NULL;

ml_ptr environment;

/**
 * @brief start with parsing process
 * 
 * @param l pointer to token stream
 * @return int TRUE:1 for correct parsing and FALSE:0 for wrong parsing
 **/
int parse(ml_ptr ml) {
  if (ml == NULL) error(NULL_POINTER);
  MOVE
  //DEBUG
  block_ptr = initNewBlock();
  meta_list_init(&environment); 
  block(ml); 
  if(TOKEN == '.') return TRUE;
  else return FALSE;
}


/**
 * @brief check block grammar
 *
 * @param l pointer to token stream
 * @return void
 **/
void block(ml_ptr ml) {
  ml_ptr env_tmp = NULL;
  rootBlock block_tmp = NULL;
  meta_list_append(&environment, NULL);
  table_init(&environment);
  /* block    -> VAR var_stmt 
   * var_stmt -> var_stmt, identifier | identifier */
  if (WORDID == VAR) {
    MOVE
    do {     
      if (WORDID == IDENTIFIER) {
	if ((var = get(&environment, WORD)) == NULL) put(&environment->content, WORD, VAR);
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
	if ((var = get(&environment, WORD)) == NULL) put(&environment->content, WORD, CONST);
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
      if ((var = get(&environment, WORD)) == NULL) put(&environment->content, WORD, PROCEDURE);
	else parseError(CODE, TYP_DOUB_DEC);
      block_ptr = newBlock(&block_ptr, environment->st->word, _PROC_);
      MOVE
    } else parseError(CODE, TYP_NO_ID);
    if (TOKEN == ';') { MOVE } else parseError(CODE, SYN_MISS_COM);				/* save environment for current iteration */
    block_tmp = block_ptr->block.proc.external_block;	/* save external branch for current iteration */
    block_ptr = block_ptr->block.proc.internal_block;	/* create subtree of function below internal branch */
    env_tmp = environment;
    block(ml);
    environment = env_tmp;
    block_ptr = block_tmp;				/* use external branch */				/* set environment to the last one */
    if (TOKEN == ';') { MOVE } else parseError(CODE, SYN_MISS_COM);
  }
  block_ptr = newBlock(&block_ptr, NULL, _STMT_);
  stmt_ptr = block_ptr->block.stmt;
  stmt(ml);  
  meta_list_remove(&environment);
}

/**
 * @brief check statement syntax
 *
 * @param l pointer to token stream
 * @return void
 **/
void stmt(ml_ptr ml) {
  rootStmt stmt_tmp = NULL;
  switch(WORDID) {
    /* stmt -> identifier = expression */
    case (IDENTIFIER): 
      
      if ((var = get(&environment, WORD)) == NULL) parseError(CODE, TYP_ID_NO_IN);
      stmt_ptr = newStmt(&stmt_ptr, var->word, _ASSIGN_);
      expr_ptr = stmt_ptr->stmt.assign.expr;
      MOVE
      if (TOKEN == '=') { MOVE } else parseError(CODE, SYN_MISS_ASS);
      expression(ml);
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
      expression(ml);
      break;
    /* stmt  -> BEGIN stmts END 
     * stmts -> stmts ; stmt | stmt */
    case (BEGIN):	
      
      MOVE
      stmt_ptr = newStmt(&stmt_ptr, NULL, _SEQ_);
      stmt_tmp = stmt_ptr->stmt.seq.stmtLeft;		/* save left branch for current iteration */
      stmt_ptr = stmt_ptr->stmt.seq.stmtRight;		/* use right branch */
      stmt(ml);
      stmt_ptr = stmt_tmp;				/* use left branch */
      while (TOKEN == ';') {
	MOVE
	stmt_ptr = newStmt(&stmt_ptr, NULL, _SEQ_);
	stmt_tmp = stmt_ptr->stmt.seq.stmtLeft;		/* save left branch for current iteration */
	stmt_ptr = stmt_ptr->stmt.seq.stmtRight;	/* use right branch */
	stmt(ml);
	stmt_ptr = stmt_tmp;				/* use left branch */
      }
      if (WORDID == END) { MOVE } else parseError(CODE, SYN_MISS_END);
      break;
    /* stmt -> IF condition THEN stmt */
    case (IF):		
      
      MOVE
      stmt_ptr = newStmt(&stmt_ptr, NULL, _IF_);
      expr_ptr = stmt_ptr->stmt._if.condition;
      condition(ml);
      if (WORDID == THEN) { MOVE } else parseError(CODE, SYN_IF);
      stmt_ptr = stmt_ptr->stmt._if.stmt;
      stmt(ml);
      break;
    /* stmt -> WHILE condition DO stmt */
    case (WHILE):	
      
      MOVE
      stmt_ptr = newStmt(&stmt_ptr, NULL, _WHILE_);
      expr_ptr = stmt_ptr->stmt._if.condition;
      condition(ml);
      if (WORDID == DO) { MOVE } else parseError(CODE, SYN_WHILE);
      stmt_ptr = stmt_ptr->stmt._if.stmt;
      stmt(ml);
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
void condition(ml_ptr ml) {
  rootExpr expr_tmp1 = NULL, expr_tmp2 = NULL;
  /* condition -> ODD expression */
  if (WORDID == ODD) {  
    MOVE
    expr_ptr = newExpr(&expr_ptr, NULL, NULL, _ODD_);
    expr_ptr = expr_ptr->expr.odd.expr;
    expression(ml);
  } 
  else {
    /* condition -> expression > expression | expression < expression */
    expr_ptr = newExpr(&expr_ptr, "", NULL, _REL_);
    expr_tmp2 = expr_ptr;				/* save current knot for later operation inserting */
    expr_tmp1 = expr_ptr->expr.rel.exprLeft;		/* save left branch for current iteration */
    expr_ptr = expr_ptr->expr.rel.exprRight;		/* use right branch */
    expression(ml); 
    expr_ptr = expr_tmp1;				/* use left branch */
    if (TOKEN == '>' || TOKEN == '<') {
      expr_tmp2->expr.rel.op[0] = TOKEN;
      MOVE
      expression(ml);
    } else {
      switch(WORDID) {
	/* condition -> expression == expression */
	case(EQ): 
	  
	  strcpy(expr_tmp2->expr.rel.op, WORD);
	  MOVE
	  expression(ml);
	  break;
	/* condition -> expression != expression */
	case(NE): 
	  
	  strcpy(expr_tmp2->expr.rel.op, WORD);
	  MOVE
	  expression(ml);
	  break;
	/* condition -> expression <= expression */
	case(LE): 
	  
	  strcpy(expr_tmp2->expr.rel.op, WORD);
	  MOVE
	  expression(ml);
	  break;
	/* condition -> expression >= expression */
	case(GE): 
	  
	  strcpy(expr_tmp2->expr.rel.op, WORD);
	  MOVE
	  expression(ml);
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
void expression(ml_ptr ml) {
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
    term(ml);
  /* expression -> term */
  } else {
    expr_ptr = newExpr(&expr_ptr, "", NULL, _ARITH_);
    expr_tmp2 = expr_ptr;				/* save current knot for later operation inserting */
    expr_tmp1 = expr_ptr->expr.arith.exprLeft;		/* save left branch for current iteration */
    expr_ptr = expr_ptr->expr.arith.exprRight;		/* use right branch */
    term(ml);
  }
  expr_ptr = expr_tmp1;					/* use left branch */
  /* expression -> expression + term | expression - term */
  while (TOKEN == '+' || TOKEN == '-') {
    expr_tmp2->expr.arith.op = TOKEN;
    MOVE
    term(ml);
  }
}

/** 
 * @brief check term syntax
 *
 * @param l token stream
 * @return void
 **/
void term(ml_ptr ml) {
  rootExpr expr_tmp1 = NULL, expr_tmp2 = NULL;
  /* term -> factor */
  expr_ptr = newExpr(&expr_ptr, "", NULL, _ARITH_);
  expr_tmp2 = expr_ptr;				/* save current knot for later operation inserting */
  expr_tmp1 = expr_ptr->expr.arith.exprLeft;		/* save left branch for current iteration */
  expr_ptr = expr_ptr->expr.arith.exprRight;		/* use right branch */
  factor(ml);
  expr_ptr = expr_tmp1;					/* use left branch */
  /* term -> term * factor | term / factor */
  while (TOKEN == '*' || TOKEN == '/') {
    expr_tmp2->expr.arith.op = TOKEN;
    MOVE
    term(ml);
  }
}

 
/**
 * @brief check factor syntax
 *
 * @param l token stream
 * @return void
 **/
void factor(ml_ptr ml) {
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
    expression(ml);
    if (TOKEN == ')') { MOVE } else parseError(CODE, SYN_MISS_CB);
  } else parseError(CODE, SYN_MISS_OB);
}









 


 



