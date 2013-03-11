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
 * @file meta_list.h
 * 
 * @defgroup meta_list
 * @ingroup lexer parser main frontend
 */

/**
 * @typedef struct_meta_list meta_list
 * 
 * @brief shortend struct _meta_list to meta_list
 * 
 */ 
typedef struct struct_meta_list meta_list;  
/**
 * @typedef _meta_list *ml_ptr
 * @brief short form to create pointer to one element of the meta list
 * 
 */
typedef meta_list *ml_ptr;

/**
 * @struct _meta_list
 * 
 * @brief element structure to for creating a double linked list
 * 
 **/
struct struct_meta_list{

  void **content; /**< pointer to any element */
  
  meta_list *next; /**< pointer to next element */
  meta_list *previous; /**< pointer to previous element */
};

extern ml_ptr meta_list_top(ml_ptr);
extern ml_ptr meta_list_last(ml_ptr);
extern ml_ptr meta_list_remove(ml_ptr *);
extern void meta_list_init(ml_ptr *);
extern void meta_list_append(ml_ptr *, void *);