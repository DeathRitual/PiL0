/**
 * @file tokdef.c Library for Token-Stream
 * 
 * Contains data structures and functions to implement a linear token stream.
 * Error handling ensures that on wrong behaviour a message will be print what went wrong.
 * With the functions in this file new Tokens can be created and connected to each other, 
 * so they represent an FIFO (new elements will be added at the end of the stream, whereas
 * the oldest element in the stream will be deketed from the top of the stream).
 * Also there are functions to read the newest and oldest element of the stream.
 * 
 */

#include "tokdef.h"
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#ifndef tokdef_C

/*
 * 
 * Error Handling
 * 
*/

/**
 * @var static char *err_msg[]
 * @brief Stringtable which stores all error messages
 **/
static char *err_msg[] = {
    "Null-Pointer", "Kein Speicher frei",
    "Liste ist leer"
  }; 
  
/**
 * @enum err_codes short strings used as variables for error messages
 */
enum err_codes {
    NULL_POINTER, ERR_MEMORY, EMPTY_LIST
  };
  

/**
  * @brief Print error message.
  * @param msg_nr short string of the error message to print
  * @return void
  **/
static void error(enum err_codes msg_nr) {
  fprintf(stderr, "FEHLER(intlist): %s!\n", err_msg[msg_nr]);
  exit(10);
}


/*
 * 
 * FIFO Implementation
 * 
 */

/**
 * @struct _tag_list  
 * 
 * @brief Element-Structure which builds Token-Stream.
 * 
 * One element can only contain one token, whereas each element is connected with its last
 * and previous neighbour.
 **/
struct _tag_list {
  /**
   * @union Token  
   * 
   * @brief Different Token for storing symbols.
   * 
   * Except for Token: token all token have an ID which helps identifying
   * the type of the keyword, identifer or number.
   **/
  union {
    /**
     * @struct token
     * 
     * Stores single character symbols like: +, -, >, <, etc.
     **/
    struct _token {
      char t; /**< single character */
    }token;
    
    /**
     * @struct number
     * 
     * Stores numbers.
     **/
    struct _number {
      int n; /**< number */ 
      unsigned int ID; /**< identifier */
    }number;
    
    /**
     * @struct word
     * 
     * Stores keywords and identifier.
     **/
    struct _word {
      char w[50]; /**< keyword / identifer */
      unsigned int ID; /**< identifier */
    }word;
  };

  char type; /**< Token-Type */
  struct _tag_list *next; /**< pointer to next element */
  struct _tag_list *previous; /**< pointer to previous element */
};

/**
 * @var struct _tag_list *head;
 * @brief pointer to oldest element in stream
 */
struct _tag_list *head;

/**
  * @brief Initialize new token stream with NULL-Pointer
  *
  * @param l pointer on beginning of new token stream
  * @return void
  **/
void l_init(list *l) {

  if (l == NULL) error(NULL_POINTER);
  *l = NULL; 
}

/**
 * @brief check if token stream is empty or not
 *
 * @param l pointer on token stream
 * @return int value 1 or 0
 **/
int l_IsEmpty(list l) {
  return l == NULL;
} 

/**
 * @brief add new element to token stream
 *
 * @param l pointer on last element of token stream
 * @param t symbol, keyword, identifier or number to generate new token of
 * @param n identifier number of keyword, identifier or number
 * @return void
 **/
void l_append(list *l, char *t, int *n) {
  if (l == NULL) error(NULL_POINTER);
  struct _tag_list *el; 
  if ((el = malloc(sizeof(list))) == NULL) error(ERR_MEMORY);
  if (strlen(t) == 1) {
    el->token.t = *t;
    el->type = 't';
  } else if (isdigit(*t) > 0) {
    el->number.n = atoi(t);
    el->number.ID = *n;
    el->type = 'n';
  } else {
    strcpy(el->word.w, t);
    el->word.ID = *n;
    el->type = 'w';
  }
  el->next = *l;
  el->previous = NULL;
  if (!l_IsEmpty(*l)) (*l)->previous = el;
  if (l_IsEmpty(*l)) head = el;
  *l = el;
}

/**
 * @brief remove oldest element in stream
 *
 * @param l pointer on last element of token stream
 * @return void
 **/
void l_remove(list *l) {
  if (l == NULL) error(NULL_POINTER);
  if (l_IsEmpty(*l)) error(EMPTY_LIST);
  if (head == *l) {
    free(head);
    *l = NULL;
  } else {
  struct _tag_list *ptr; 
  ptr = head->previous;
  ptr->next = NULL;
  head->previous = NULL;
  free(head);
  head = ptr;
  }
}

/**
 * @brief get the oldest element of stream
 *
 * @param l pointer on last element of token stream
 * @return list pointer to oldest element
 **/
list l_top(list l) {
  if (l_IsEmpty(l)) error(NULL_POINTER);
  return head;
}

/**
 * @brief get the newest element of stream
 *
 * @param l pointer on last element of token stream
 * @return list point to newest element
 **/
list l_last(list l) {
  if (l_IsEmpty(l)) error(NULL_POINTER);
  return l;
}

#endif