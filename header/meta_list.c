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

#include"meta_list.h"

/**
  * @brief constructor for new meta list
  *
  * @retval ml_ptr* 
  **/
ml_ptr meta_list_init() {
  ml_ptr new_list = NULL;
  if ((new_list = malloc(sizeof(*new_list))) == NULL) error(ERR_MEMORY);
  new_list->first = NULL;
  new_list->list = NULL;
  return new_list;
}

/**
 * @brief check if  element is empty or not
 *
 * @param *ml pointer to meta list element
 * @retval int 1 or 0
 **/
static int meta_list_IsEmpty(const mle_ptr ml) {
  return ml == NULL;
}



/**
 * @brief create new element on meta list, connect it to the last element and actualize first element
 *
 * @param **ml pointer to current meta list element
 * @param *content pointer to content element
 * @retval void
 **/
void meta_list_append(ml_ptr *ml, void *content) {  
  if (ml == NULL) error(NULL_POINTER);
  mle_ptr new_element = NULL, old_element = (*ml)->list;
  if ((new_element = malloc(sizeof(*new_element))) == NULL) error(ERR_MEMORY); 
  new_element->content = content;      
  new_element->next = old_element;
  new_element->previous = NULL;
  if (!meta_list_IsEmpty(old_element)) {
    new_element->number = old_element->number + 1;
    old_element->previous = new_element;
  } else {
    new_element->number = 1;
    (*ml)->first = new_element;
  }
  (*ml)->list = new_element;
}

/**
 * @brief remove oldest element in the meta list
 *
 * @param **ml pointer on newest element in the meta list
 * @retval void
 **/
void meta_list_remove_fifo(ml_ptr *ml) {
  if (ml == NULL) error(NULL_POINTER);
  if (meta_list_IsEmpty((*ml)->list)) error(EMPTY_LIST);
  if ((*ml)->first == (*ml)->list) {
    free((*ml)->list->content);
    (*ml)->list->content = NULL;
    free((*ml)->list);
    (*ml)->list = NULL;
    free((*ml));
    *ml = NULL;
  }
  else {
    mle_ptr ptr = (*ml)->first->previous;;
    ptr->next = NULL;
    (*ml)->first->previous = NULL;
    free((*ml)->first->content);
    free((*ml)->first);
    (*ml)->first = ptr;
  }
}

/**
 * @brief remove newest element in the meta list
 * 
 * @param **ml pointer to newest element in the meta list
 * @retval void
 **/
void meta_list_remove_lifo(ml_ptr *ml) {
  if (ml == NULL) error(NULL_POINTER);
  if (meta_list_IsEmpty((*ml)->list)) error(EMPTY_LIST);
  if ((*ml)->first == (*ml)->list) {
    free((*ml)->list->content);
    (*ml)->list->content = NULL;
    free((*ml)->list);
    (*ml)->list = NULL;
    free((*ml));
    *ml = NULL;
  }
  else {
    mle_ptr ptr = (*ml)->list->next;
    (*ml)->list->next = NULL;
    ptr->previous = NULL;
    free((*ml)->list->content);
    (*ml)->list->content = NULL;
    free((*ml)->list);
    (*ml)->list = ptr;
  }
}

/**
 * @brief get the oldest element from the meta list
 *
 * @param *ml pointer on last element of the meta list
 * @retval ml->first pointer to oldest element
 **/
mle_ptr meta_list_top(ml_ptr ml) {
  if (ml == NULL) error(NULL_POINTER);
  if (meta_list_IsEmpty(ml->list)) error(EMPTY_LIST);
  return ml->first;
}


