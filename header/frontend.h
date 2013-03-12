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
 * @file frontend.h Header-File for lexer and parser library
 * 
 * Forwards the functions of the lexer and parser library to another file which includes this header file.
 * 
 * @defgroup frontend Frontend
 * @ingroup main
 * @{
 * 
 */

#ifndef __FRONTEND_H
  #define __FRONTEND_H
#include"err_handling.h"
#include"meta_list.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LENGTH 30




ml_ptr lexer(ml_ptr, FILE *);
int parse(ml_ptr);


/**
 * @enum special_IDs identifier number for variables and numbers
 * 
 * identifying numbers start at 300. 
 *
 * @note 
 * If keywords exceds more than 44 words, than you have to increase the special_IDs initial number
 */
enum special_IDs {
  BEGIN = 256, CALL, CONST, DO, END, IF, ODD, PRINT, PROCEDURE, READ, THEN, VAR, WHILE, PASS, 
  EQ, GE, LE, NE, IDENTIFIER, NUM
};

/** @} */

#endif