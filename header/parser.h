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
 * @file parser.h Header-File for Parser
 * 
 * Forwards the functions of the parser library to another file which includes this header file.
 */

#include"lexer.h"
#ifndef _PARSER_H
  #define _PARSER_H

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
  char word[30]; /**< symbol name */
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



extern void st_init(env_ptr *);
extern void table_init(table_ptr *);
extern void st_append(env_ptr *);
extern void put(table_ptr *, char *, int);
extern table_ptr get(env_ptr *, char *);
extern int parse(list);
void block(list);
void stmt(list);
void expression(list);
void condition(list);

#endif