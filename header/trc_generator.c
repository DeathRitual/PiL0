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
 * @file trc_generator.c Library which inherits all necessary functions for generating three adress code out of abstract syntax tree
 * 
 * @ingroup parser
 * 
 * @{
 */

#include"ast.h"

int temp_number = 0;
char t[3] = "";

char *temp() {
  sprintf(t, "t%d", temp_number);
  temp_number++;
  return t;
}

void reset_temp() {
  temp_number = 0;
}