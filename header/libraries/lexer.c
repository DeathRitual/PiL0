/**
 * @file lexer.c Library for Token-Stream
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

#include "../lexer.h"
#include "../language.h"
#include "../err_handling.h"
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#define DEBUG	printf("test");

/**
 * @var token_stream *head;
 * @brief pointer to oldest element in stream
 */
token_stream *head;

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
  token_stream *el; 
  if ((el = malloc(sizeof(token_stream))) == NULL) error(ERR_MEMORY);
  if (isdigit(*t) > 0) {
    el->element.type = 'n';
    el->element.number.n = atoi(t);
    el->element.number.ID = *n;
  } else if (isalpha(*t) > 0) {    
    el->element.type = 'w';
    strcpy(el->element.word.w, t);   
    el->element.word.ID = *n;
  } else {
    el->element.type = 't';
    el->element.token.t = *t;
  }
  el->next = *l;
  el->previous = NULL;
  if (!l_IsEmpty(*l)) (*l)->previous = el;
  else head = el;
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
  token_stream *ptr; 
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
  return head;
}

/**
 * @brief get the newest element of stream
 *
 * @param l pointer on last element of token stream
 * @return list point to newest element
 **/
list l_last(list l) {
  return l;
}


/**
 * @brief Returns the number of keyword
 * 
 * Checks if the given word is a keyword and returns its position number from the key_array 
 *
 * @param k array of keywords...
 * @param s string to be checked...
 * @return int either position number of keyword or -1 if word is not stored in keyword array
 **/
int get_keyNUM(keyword *k, char *s) {
  int i = 0;
  for (i = 0; keywords[i] != NULL; i++) 
    if (strcmp(k[i].w, s) == 0) return i;
  return -1;
}

/**
 * @brief counts of keywords
 *
 * @return unsigned int number of keywords
 **/
static unsigned int get_keySize() {
  int i = 1;
  while (keywords[i] != NULL)
    i++;
  return i;
}

/**
 * @brief creates an array of all keywords
 *
 * @return keyword* pointer to keyword array
 **/
keyword *init_ReservedKeys() {
  keyword *resKeys = malloc(sizeof *resKeys * get_keySize());
  int i;
  for (i = 0; keywords[i] != NULL; i++) {
    strcpy(resKeys[i].w, keywords[i]);
    resKeys[i].ID = 256 + i;
  }
  return resKeys;
}


/**
 * @brief Function for lexical scanning...
 *
 * Reads input code and convert each element to one token / word / number and add it to the token stream
 * 
 * @param raw_code input source code...
 * @param token_stream pointer to list for ading tokens
 * @return list token-stream
 **/
list lexer(list token_stream, FILE *raw_code) {
  keyword *reserved = init_ReservedKeys();
  int c;
  l_init(&token_stream);
  list tok;
  while((c = fgetc(raw_code)) != EOF) {
    char w[30] = "";
    int i = 0, key_NUM, ident = IDENTIFIER;
    
    /* eat control characters */
    while (c < 33) c = fgetc(raw_code);
    
    /* read compare operators */
    if (c == '=' || c == '>' || c == '<' || c == '!') {
      w[i] = c;
      if ((c = fgetc(raw_code)) == '=') {
	w[i + 1] = c;
	if (strcmp(w, "==") == 0) key_NUM = get_keyNUM(reserved, "EQ");
	if (strcmp(w, ">=") == 0) key_NUM = get_keyNUM(reserved, "GE");
	if (strcmp(w, "<=") == 0) key_NUM = get_keyNUM(reserved, "LE");
	if (strcmp(w, "!=") == 0) key_NUM = get_keyNUM(reserved, "NE");
	l_append(&token_stream, reserved[key_NUM].w, &reserved[key_NUM].ID);
      } else l_append(&token_stream, w, &ident);
    }
    
    /* read words or identifier */
    if (isalpha(c)) {
      w[i] = c;
      c = fgetc(raw_code);
      while (isalnum(c)) {
	w[++i] = c;
	c = fgetc(raw_code);
      }
      key_NUM = get_keyNUM(reserved, w);
      if (key_NUM >= 0) l_append(&token_stream, reserved[key_NUM].w, &reserved[key_NUM].ID);
      else l_append(&token_stream, w, &ident);
      c = ungetc(c, raw_code);
    }
   
    /*read numbers */
    if (isdigit(c)) {
      ident = NUM;
      w[i] = c;
      c = fgetc(raw_code);
      while (isdigit(c)) {
	w[++i] = c;
	c = fgetc(raw_code);
      }	
      l_append(&token_stream, w, &ident);
      c = ungetc(c, raw_code);
    }
    
    /* read tokens */
    if (strlen(w) == 0) {
      w[i] = c;
      l_append(&token_stream, w, &ident);
    }
  }
  
  return token_stream;
} 
