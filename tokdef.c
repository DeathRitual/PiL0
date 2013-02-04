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
 * @var char *keywords[]
 * @brief Stringtable of all keywords longer than 1 character
 **/
char *keywords[] = {
  "BEGIN", "CALL", "CONST", "DO", "END", "IF", "ODD", "PRINT", "PROCEDURE",
  "READ", "THEN", "VAR", "WHILE", "PASS", "==", ">=", "<=", "!="
};

/**
 * @enum id identifier number for every keyword longer than 1 character, identifier and number
 * 
 * identifying numbers start with 256
 **/
enum id {
  BEGIN = 256, CALL, CONST, DO, END, IF, ODD, PRINT,
  PROCEDURE, READ, THEN, VAR, WHILE, PASS, EQ, GE, LE, NE, IDENTIFIER, NUM
};

/**
 * @brief initialise keyword BEGIN
 *
 * @return keyword*
 **/
keyword* ini_BEGIN() {
  keyword *BEGIN_PTR;
  if ((BEGIN_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(BEGIN_PTR->w, keywords[BEGIN - 256]);
  BEGIN_PTR->ID = BEGIN;
  return BEGIN_PTR;
}

/**
 * @brief initialise keyword CALL
 *
 * @return keyword*
 **/
keyword* ini_CALL() {
  keyword *CALL_PTR;
  if ((CALL_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(CALL_PTR->w, keywords[CALL - 256]);
  CALL_PTR->ID = CALL;
  return CALL_PTR;
}

/**
 * @brief initialise keyword CONST
 *
 * @return keyword*
 **/
keyword* ini_CONST() {
  keyword *CONST_PTR;
  if ((CONST_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(CONST_PTR->w, keywords[CONST - 256]);
  CONST_PTR->ID = CONST;
  return CONST_PTR;
}

/**
 * @brief initialise keyword DO
 *
 * @return keyword*
 **/
keyword* ini_DO() {
  keyword *DO_PTR;
  if ((DO_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(DO_PTR->w, keywords[DO - 256]);
  DO_PTR->ID = DO;
  return DO_PTR;
}

/**
 * @brief initialise keyword END
 *
 * @return keyword*
 **/
keyword* ini_END() {
  keyword *END_PTR;
  if ((END_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(END_PTR->w, keywords[END - 256]);
  END_PTR->ID = END;
  return END_PTR;
}

/**
 * @brief initialise keyword IF
 *
 * @return keyword*
 **/
keyword* ini_IF() {
  keyword *IF_PTR;
  if ((IF_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(IF_PTR->w, keywords[IF - 256]);
  IF_PTR->ID = IF;
  return IF_PTR;
}

/**
 * @brief initialise keyword ODD
 *
 * @return keyword*
 **/
keyword* ini_ODD() {
  keyword *ODD_PTR;
  if ((ODD_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(ODD_PTR->w, keywords[ODD - 256]);
  ODD_PTR->ID = ODD;
  return ODD_PTR;
}

/**
 * @brief initialise keyword PRINT
 *
 * @return keyword*
 **/
keyword* ini_PRINT() {
  keyword *PRINT_PTR;
  if ((PRINT_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(PRINT_PTR->w, keywords[PRINT - 256]);
  PRINT_PTR->ID = PRINT;
  return PRINT_PTR;
}

/**
 * @brief initialise keyword PROCEDURE
 *
 * @return keyword*
 **/
keyword* ini_PROCEDURE() {
  keyword *PROCEDURE_PTR;
  if ((PROCEDURE_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(PROCEDURE_PTR->w, keywords[PROCEDURE - 256]);
  PROCEDURE_PTR->ID = PROCEDURE;
  return PROCEDURE_PTR;
}

/**
 * @brief initialise keyword READ
 *
 * @return keyword*
 **/
keyword* ini_READ() {
  keyword *READ_PTR;
  if ((READ_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(READ_PTR->w, keywords[READ - 256]);
  READ_PTR->ID = READ;
  return READ_PTR;
}

/**
 * @brief initialise keyword THEN
 *
 * @return keyword*
 **/
keyword* ini_THEN() {
  keyword *THEN_PTR;
  if ((THEN_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(THEN_PTR->w, keywords[THEN - 256]);
  THEN_PTR->ID = THEN;
  return THEN_PTR;
}

/**
 * @brief initialise keyword VAR
 *
 * @return keyword*
 **/
keyword* ini_VAR() {
  keyword *VAR_PTR;
  if ((VAR_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(VAR_PTR->w, keywords[VAR - 256]);
  VAR_PTR->ID = VAR;
  return VAR_PTR;
}

/**
 * @brief initialise keyword WHILE
 *
 * @return keyword*
 **/
keyword* ini_WHILE() {
  keyword *WHILE_PTR;
  if ((WHILE_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(WHILE_PTR->w, keywords[WHILE - 256]);
  WHILE_PTR->ID = WHILE;
  return WHILE_PTR;
}

/**
 * @brief initialise keyword PASS
 *
 * @return keyword*
 **/
keyword* ini_PASS() {
  keyword *PASS_PTR;
  if ((PASS_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(PASS_PTR->w, keywords[PASS - 256]);
  PASS_PTR->ID = PASS;
  return PASS_PTR;
}

/**
 * @brief initialise keyword ==
 *
 * @return keyword*
 **/
keyword* ini_EQ() {
  keyword *EQ_PTR;
  if ((EQ_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(EQ_PTR->w, keywords[EQ - 256]);
  EQ_PTR->ID = EQ;
  return EQ_PTR;
}

/**
 * @brief initialise keyword >=
 *
 * @return keyword*
 **/
keyword* ini_GE() {
  keyword *GE_PTR;
  if ((GE_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(GE_PTR->w, keywords[GE - 256]);
  GE_PTR->ID = GE;
  return GE_PTR;
}

/**
 * @brief initialise keyword <=
 *
 * @return keyword*
 **/
keyword* ini_LE() {
  keyword *LE_PTR;
  if ((LE_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(LE_PTR->w, keywords[LE - 256]);
  LE_PTR->ID = LE;
  return LE_PTR;
}

/**
 * @brief initialise keyword !=
 *
 * @return keyword*
 **/
keyword* ini_NE() {
  keyword *NE_PTR;
  if ((NE_PTR = malloc(sizeof(keyword))) == NULL) error(ERR_MEMORY);
  strcpy(NE_PTR->w, keywords[NE - 256]);
  NE_PTR->ID = NE;
  return NE_PTR;
}


#endif