/**
 * @file lexer.h Header-File for Token-Stream library
 * 
 * Forwards the functions of the Token-Stream library to another file which includes this header file.
 * 
 */
 
#include<stdio.h>
#ifndef __LEXER_H
  #define __LEXER_H

/**
 * @typedef struct _token_stream token_stream
 * @brief shortend struct _token_stream to token_stream
 * 
 */ 
typedef struct _token_stream token_stream;  
/**
 * @typedef _token_stream *list
 * @brief short form to create pointer on Token-Stream
 * 
 */
typedef token_stream *list;

/**
 * @struct _token_stream  
 * 
 * @todo export struct element to own struct and add pointer in _token_stream
 * 
 * @brief Element-Structure which builds Token-Stream.
 * 
 * One element can only contain one token, whereas each element is connected with its last
 * and previous neighbour.
 **/
struct _token_stream{
    
  /**
   * @struct _element 
   * 
   * @brief Different Token for storing symbols.
   * 
   * Except for Token: token all token have an ID which helps identifying
   * the type of the keyword, identifer or number.
   **/
  struct _element{
    char type; /**< Token-Type */
    unsigned int line; /**< code line number */
    
    /**
     * @struct token
     * 
     * Stores single character symbols like: +, -, >, <, etc.
     **/
    struct _token {
      char t; /**< single character */
    }token; /**< Can store single characters */
    
    /**
     * @struct number
     * 
     * Stores numbers.
     **/
    struct _number {
      int n; /**< number */ 
      unsigned int ID; /**< number identifier */
    }number; /**< Can store numbers and the NUM-ID */
    
    /**
     * @struct word
     * 
     * Stores keywords and identifier.
     **/
    struct _word {
      unsigned int ID; /**< keyword / identifier identifier */
      char w[30]; /**< keyword / identifer */
    }word; /**< Can store words and either the Keyword-ID or IDENTIFIER-ID */
    
  }element; /**< Structure to store different types of lexical tokens */
  
  token_stream *next; /**< pointer to next element */
  token_stream *previous; /**< pointer to previous element */
};

extern void l_init(list *);
extern int l_IsEmpty(list);
extern void l_append(list *, char *, int *, int *);
extern list l_remove(list *);
extern list l_top(list);
extern list l_last(list);
extern list lexer(list, FILE *);


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

typedef struct{
  char w[30]; /**< keyword */
  int ID; /**< keyword identifier */
  /**
  * @struct keyword  
  * 
  * @brief Keyword structure for storing initial keywords
  * 
  * Stores string and ID of the language keywords like DO, WHILE, IF, etc.
  **/
}keyword;


extern int get_keyNUM(keyword *, char *);
extern keyword *init_ReservedKeys(void);

#endif