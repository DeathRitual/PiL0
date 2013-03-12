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
 */

#ifndef __AST_H
  #define __AST_H
#define MAX_LENGTH 30

typedef struct _block *rootBlock;
typedef struct _stmt *rootStmt;
typedef struct _expr *rootExpr;

enum block_id {
    _PROC_, _STMT_
  };

struct _block {
  enum block_id tag;
  union {
    struct {
      char word[MAX_LENGTH]; 		/* identifier */
      struct _block *external_block;
      struct _block *internal_block;
    } proc;
    struct _stmt *stmt;
  } block;
};

enum stmt_id {
    _IF_, _WHILE_, _ASSIGN_, _SEQ_, _CALL_, _READ_, _PRINT_
  };

struct _stmt {
  enum stmt_id tag;
  union {
    char word[MAX_LENGTH];		/* CALL, READ */
    struct _expr *expr;			/* PRINT */
    struct {
      struct _expr *condition;
      struct _stmt *stmt;
    } _while;
    struct {
      struct _expr *condition;
      struct _stmt *stmt;
    } _if;
    struct {
      char word[MAX_LENGTH];
      struct _expr *expr;
    } assign;
    struct {
      struct _stmt *stmtRight;
      struct _stmt *stmtLeft;
    } seq;
  } stmt;
};

enum expr_id { 
    _NUMBER_, _IDENTIFIER_, _ARITH_, _UNARY_, _REL_, _ODD_  
  };

struct _expr {
  enum expr_id tag;
  union {
    int number;				/* number */
    char word[MAX_LENGTH];		/* identifier */
    struct {
      char op;				/* +, -, *, / */
      struct _expr *exprRight;		/* expression */
      struct _expr *exprLeft;		/* expression */
    } arith; 
    struct {
      char op[3];			/* <, >, !, ==, !=, <=, >= */
      struct _expr *exprRight;		/* expression */
      struct _expr *exprLeft;		/* expression */
    } rel;
    struct {
      char op;				/* - */
      struct _expr *expr;		/* expression */
    } unary;
    struct {
      struct _expr *expr;
    } odd;
  } expr;
};

extern rootBlock initNewBlock();
extern rootStmt initNewStmt();
extern rootExpr initNewExpr();
extern rootBlock newBlock(rootBlock *, char *, enum block_id);
extern rootStmt newStmt(rootStmt *, char *, enum stmt_id);
extern rootExpr newExpr(rootExpr *, char *, int *, enum expr_id);

#endif