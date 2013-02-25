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
 * @file language.h Header-File for language definition
 * 
 * All entries in this file defines the language. 
 * The stringtable keyworss will define all keywords. Later there will be added structs which
 * represents syntax daigrams of the language.
 */

#include<stddef.h>
#ifndef __LANGUAGE_H

/**
 * @var char *keywords[]
 * @brief Stringtable of all keywords longer than 1 character
 * 
 **/
char *keywords[] = {
  "BEGIN", "CALL", "CONST", "DO", "END", "IF", "ODD", "PRINT", "PROCEDURE",
  "READ", "THEN", "VAR", "WHILE", "PASS", "EQ", "GE", "LE", "NE", NULL
};

#endif