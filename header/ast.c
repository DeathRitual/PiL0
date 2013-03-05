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

rootBlock newProc(rootBlock *block_ptr, char *s, enum block_id id) {
  printf("ro %p\n", *block_ptr);
  (*block_ptr)->tag = id;
  strcpy((*block_ptr)->block.proc.word, s);
  (*block_ptr)->block.proc.external_block = NULL;
  (*block_ptr)->block.proc.internal_block = NULL;
  printf("Word: %s\n", (*block_ptr)->block.proc.word);
  rootBlock knotRight = initNewBlock();
  (*block_ptr)->block.proc.external_block = knotRight;
  rootBlock knotLeft = initNewBlock();
  (*block_ptr)->block.proc.internal_block = knotLeft;
  printf("ex %p \n", knotRight);
  printf("in %p \n", knotLeft);
  return *block_ptr;
} 

rootStmt newStmt(rootBlock *block_ptr, enum block_id id) {
  printf("ro %p\n", *block_ptr);
  (*block_ptr)->tag = id;
  (*block_ptr)->block.stmt = NULL;
  rootStmt knot_stmt = initNewStmt();
  (*block_ptr)->block.stmt = knot_stmt;
  printf("st: %p\n", knot_stmt);
  return (*block_ptr)->block.stmt;
}