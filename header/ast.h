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
 * @file ast.h Header-File for the ast library
 * 
 * Forwards the functions of the ast library to another file which includes this header file.
 * 
 * @defgroup ast Abstract Syntax Tree
 * @ingroup parser
 * 
 * @{
 */

#ifndef __AST_H
  #define __AST_H
#include"err_handling.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LENGTH 30

typedef struct _block *rootBlock;
typedef struct _stmt *rootStmt;
typedef struct _expr *rootExpr;


enum block_id {
    _PROC_, _STMT_
  };

/**
  * @struct _block
  * 
  * @brief block root
  **/
struct _block {
  enum block_id tag; 		/**< union identifier */
  union {
    struct {
      char word[MAX_LENGTH]; 		/* identifier */
      struct _block *external_block;
      struct _block *internal_block;
    } proc;  			/**< block */
    struct _stmt *stmt; 	/**< statement */
  } block;
};

enum stmt_id {
    _IF_, _WHILE_, _ASSIGN_, _SEQ_, _CALL_, _READ_, _PRINT_
  };

/**
  * @struct _stmt
  * 
  * @brief statement root
  **/
struct _stmt {
  enum stmt_id tag; 		/**< union identifier */
  union {
    char word[MAX_LENGTH]; 	/**< identifier for CALL / READ */		
    struct _expr *expr;		/**< PRINT expression */
    struct {
      struct _expr *condition;
      struct _stmt *stmt;
    } _while;			/**< while loop */
    struct {
      struct _expr *condition;
      struct _stmt *stmt;
    } _if;			/**< if condition */
    struct {
      char word[MAX_LENGTH];
      struct _expr *expr;
    } assign;			/**< assignment */
    struct {
      struct _stmt *stmtRight;
      struct _stmt *stmtLeft;
    } seq;			/**< sequence of statements */
  } stmt;
};

enum expr_id { 
    _NUMBER_, _IDENTIFIER_, _ARITH_, _UNARY_, _REL_, _ODD_  
  };

/**
  * @struct _expr
  * 
  * @brief expression root
  **/
struct _expr {
  enum expr_id tag; 		/**< union identifier */
  union {
    int number;			/**< number */
    char word[MAX_LENGTH];	/**< identifier */
    struct {
      char op;				/* +, -, *, / */
      struct _expr *exprRight;		/* expression */
      struct _expr *exprLeft;		/* expression */
    } arith; 			/**< arithmetic expression */
    struct {
      char op[3];			/* <, >, !, ==, !=, <=, >= */
      struct _expr *exprRight;		/* expression */
      struct _expr *exprLeft;		/* expression */
    } rel;			/**< relational expression */
    struct {
      char op;				/* - */
      struct _expr *expr;		/* expression */
    } unary;			/**< unary expression */
    struct {
      struct _expr *expr;
    } odd;			/**< odd expression */
  } expr;
};


typedef struct _tac_quadruple quadruple;
typedef quadruple *quadruple_ptr;

struct _tac_quadruple {
  char op[MAX_LENGTH];
  char arg1[MAX_LENGTH];
  char arg2[MAX_LENGTH];
  char result[MAX_LENGTH];
};

extern quadruple_ptr initNewQuadruple(char *, char *, char *, char *);
extern char *temp();
extern void reset_temp();
extern rootBlock initNewBlock();
extern rootStmt initNewStmt();
extern rootExpr initNewExpr();
extern rootBlock newBlock(rootBlock *, char *, enum block_id);
extern rootStmt newStmt(rootStmt *, char *, enum stmt_id);
extern rootExpr newExpr(rootExpr *, char *, int *, enum expr_id);

#endif

/** @} */