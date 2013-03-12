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
 * @file meta_list.c
 * 
 * @ingroup meta_list
 */



#include"err_handling.h"
#include"meta_list.h"
#include<stdlib.h>

/**
 * @var meta_list *head;
 * @brief pointer to top (oldest) element in the meta list
 */
ml_ptr head;

/**
  * @brief Initialize new meta list with NULL-Pointer
  *
  * @param **ml pointer to the beginning of new meta list
  * @retval void
  **/
void meta_list_init(ml_ptr *ml) {
  if (ml == NULL) error(NULL_POINTER);
  *ml = NULL;
}

/**
 * @brief check if pointed element is empty or not
 *
 * @param *ml pointer to meta list element
 * @retval int 1 or 0
 **/
static int meta_list_IsEmpty(const ml_ptr ml) {
  return ml == NULL;
}

/**
 * @brief create new element on meta list, connect it to the last element and set it to new head element
 *
 * @param **ml pointer to current meta list element
 * @param *content pointer to content element
 * @retval void
 **/
void meta_list_append(ml_ptr *ml, void *content) {
  if (ml == NULL) error(NULL_POINTER);
  ml_ptr el;
  if ((el = malloc(sizeof(meta_list))) == NULL) error(ERR_MEMORY);   
  el->content = content;      
  el->next = *ml;
  el->previous = NULL;
  if (!meta_list_IsEmpty(*ml)) (*ml)->previous = el;
  else head = el;
  *ml = el;
}

/**
 * @brief remove oldest element from the meta list
 *
 * @param **ml pointer on newst element in the meta list
 * @retval *head meta list pointer to new head element @see head
 **/
void meta_list_remove(ml_ptr *ml) {
  if (ml == NULL) error(NULL_POINTER);
  if (meta_list_IsEmpty(*ml)) error(EMPTY_LIST);
  if (head == *ml) {
    free(head->content);
    free(head);
    *ml = NULL;
  }
  else {
    ml_ptr ptr;
    ptr = head->previous;
    ptr->next = NULL;
    head->previous = NULL;
    free(head->content);
    free(head);
    head = ptr;
  }
}

/**
 * @brief get the oldest element from the meta list
 *
 * @param *ml pointer on last element of the meta list
 * @retval *head pointer to oldest element
 **/
ml_ptr meta_list_top(ml_ptr ml) {
  if (ml == NULL) error(NULL_POINTER);
  if (meta_list_IsEmpty(ml)) error(EMPTY_LIST);
  return head;
}

/**
 * @brief get the newest element of stream
 *
 * @param *ml pointer on last element of the meta list
 * @retval *ml meta list pointer to the newest element
 **/
ml_ptr meta_list_last(ml_ptr ml) {
  if (ml == NULL) error(NULL_POINTER);
  if (meta_list_IsEmpty(ml)) error(EMPTY_LIST);
  return ml;
}

