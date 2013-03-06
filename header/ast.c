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
 * @file ast.c Library which inherits all necessary functions for creating an abstract syntax tree
 */

#include"err_handling.h"
#include"ast.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

rootBlock initNewBlock() { 
  rootBlock knot_block;
  if ((knot_block = malloc(sizeof(struct _block))) == NULL) error(ERR_MEMORY);
  return knot_block;
}

rootStmt initNewStmt() {
  rootStmt knot_stmt;
  if ((knot_stmt = malloc(sizeof(struct _stmt))) == NULL) error(ERR_MEMORY);
  return knot_stmt;
}

rootExpr initNewExpr() {
  rootExpr knot_expr;
  if ((knot_expr = malloc(sizeof(struct _expr))) == NULL) error(ERR_MEMORY);
  return knot_expr;
}

rootBlock newBlock(rootBlock *block_ptr, char *s, enum block_id id) {
  printf("ro %p\n", *block_ptr);
  if (id == _PROC_) {
    (*block_ptr)->tag = id;
    strcpy((*block_ptr)->block.proc.word, s);
    printf("Word: %s\n", (*block_ptr)->block.proc.word);
    rootBlock knotRight = initNewBlock();
    (*block_ptr)->block.proc.external_block = knotRight;
    rootBlock knotLeft = initNewBlock();
    (*block_ptr)->block.proc.internal_block = knotLeft;
    printf("ex %p \n", knotRight);
    printf("in %p \n", knotLeft);
  } else if (id == _STMT_) {
    printf("ro %p\n", *block_ptr);
    (*block_ptr)->tag = id;
    rootStmt knot_stmt = initNewStmt();
    (*block_ptr)->block.stmt = knot_stmt;
    printf("st %p\n", knot_stmt);
  } else error(WRONG_ID);
  return *block_ptr;
} 

rootStmt newStmt(rootStmt *stmt_ptr, char *s, enum stmt_id id) {
  printf("ro %p\n", *stmt_ptr);
  switch (id) {
    case _IF_: 
      printf("if:\n");
      (*stmt_ptr)->tag = id;
      rootExpr knotExprIf = initNewExpr();
      (*stmt_ptr)->stmt._if.condition = knotExprIf;
      rootStmt knotStmtIf = initNewStmt();
      (*stmt_ptr)->stmt._if.stmt = knotStmtIf;
      printf("co %p\n", knotExprIf);
      printf("st %p\n", knotStmtIf);
      break;
    case _WHILE_:
      printf("wh:\n");
      (*stmt_ptr)->tag = id;
      rootExpr knotExprWhile = initNewExpr();
      (*stmt_ptr)->stmt._while.condition = knotExprWhile;
      rootStmt knotStmtWhile = initNewStmt();
      (*stmt_ptr)->stmt._while.stmt = knotStmtWhile;
      printf("co %p\n", knotExprIf);
      printf("st %p\n", knotStmtIf);
      break;
    case _ASSIGN_:
      printf("as:\n");
      (*stmt_ptr)->tag = id;
      strcpy((*stmt_ptr)->stmt.assign.word, s);
      rootExpr knotExprAssign = initNewExpr();
      (*stmt_ptr)->stmt.assign.expr = knotExprAssign;
      printf("ep %p\n", knotExprAssign);
      break;
    case _CALL_: 

      (*stmt_ptr)->tag = id;
      strcpy((*stmt_ptr)->stmt.word, s);
      break;
    case _READ_:

      (*stmt_ptr)->tag = id;
      strcpy((*stmt_ptr)->stmt.word, s);
      break;
    case _PRINT_: 
      printf("pr:\n");
      (*stmt_ptr)->tag = id;
      rootExpr knotExpr = initNewExpr();
      (*stmt_ptr)->stmt.expr = knotExpr;
      printf("ep %p\n", knotExpr);
      break;
      
    case _SEQ_:
      printf("se:\n");
      (*stmt_ptr)->tag = id;
      rootStmt knotStmtRight = initNewStmt();
      (*stmt_ptr)->stmt.seq.stmtRight = knotStmtRight;
      rootStmt knotStmtLeft = initNewStmt();
      (*stmt_ptr)->stmt.seq.stmtLeft = knotStmtLeft;
      printf("ri %p\n", knotStmtRight);
      printf("le %p\n", knotStmtLeft);
      break;
    default:	
      
      error(WRONG_ID);
      break;
  }
  return *stmt_ptr;
}