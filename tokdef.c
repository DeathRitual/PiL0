/**
 * @file tokdef.c Library for Token-Stream
 * 
 * Contains data structures and functions to implement a linear token stream.
 * 
 * Error handling ensures that on wrong behaviour a message will be print what went wrong.
 * 
 * With the functions in this file new Tokens can be created and connected to each other, 
 * so they represent an FIFO (new elements will be added at the end of the stream, whereas
 * the oldest element in the stream will be deketed from the top of the stream).
 * Also there are functions to read the newest and oldest element of the stream.
 * 
 * Further functions are included to initalise the keywords with unique identifiers.
 * The lexer scans the input file and creates a new token with the data he gets from the 
 * fitting keyword. if no keyword matches the input string must either be a token (non 
 * alphanumerical), a number or an identifier.
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
 * @var char *err_msg[]
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

/*
 * 
 * keyword definition
 * 
 */



/**
 * @enum special_IDs identifier number for variables and numbers
 * 
 * identifying numbers start at 300. 
 * NOTE: If keywords exceds more than 44 words, than you have to increase this number 
 **/
enum special_IDs {
  IDENTIFIER = 300, NUM
};


/**
 * @var char *keywords[]
 * @brief Stringtable of all keywords longer than 1 character
 * 
 **/
static char *keywords[] = {
  "BEGIN", "CALL", "CONST", "DO", "END", "IF", "ODD", "PRINT", "PROCEDURE",
  "READ", "THEN", "VAR", "WHILE", "PASS", "==", ">=", "<=", "!=", NULL
};


/**
 * @brief Creates an array with all reserved keywords and their IDs
 *
 * @return key_array array with reserved keys
 * 
 * The IDs of the keywords start from 256 at keyword BEGIN and are incremented for every next
 * word. 
 * NOTE: If keywords number increases 44 you have to increase the special_Ids number!
 **/
struct key_array init_ReservedKeys() {
  int i;
  for (i = 0; keywords[i] != NULL; i++) {
    strcpy(keys.resKeys[i].w,keywords[i]);
    keys.resKeys[i].ID = 256 + i;
  }
  return keys;
}


#endif