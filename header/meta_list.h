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
 * @defgroup meta_list Meta List
 * @ingroup lexer parser main frontend
 */

#include"err_handling.h"
#include<stdlib.h>
#include<stdio.h>

typedef struct struct_meta_list_element meta_list_element;
typedef meta_list_element *mle_ptr;

/**
 * @struct struct_meta_list_element
 * 
 * @brief meta_list element
 **/
struct struct_meta_list_element {
    void **content; 	/**< pointer to any element */
    int number; 	/**< element number */
    
    mle_ptr next; 	/**< pointer to next element */
    mle_ptr previous; 	/**< pointer to previous element */
};


typedef struct struct_meta_list meta_list;  
typedef meta_list *ml_ptr;

/**
 * @struct struct_meta_list
 * 
 * @brief wrapper struct for double linked list
 * 
 **/
struct struct_meta_list{
  mle_ptr first;	/** < first element in list */
  mle_ptr list;  	/** < current and last element in list */
};



extern mle_ptr meta_list_top(ml_ptr);
extern void meta_list_remove_fifo(ml_ptr *);
extern void meta_list_remove_lifo(ml_ptr *);
extern ml_ptr meta_list_init();
extern void meta_list_append(ml_ptr *, void *);


