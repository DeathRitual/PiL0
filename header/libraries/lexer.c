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
  if (isdigit(*t) > 0) {
    el->number.n = atoi(t);
    el->number.ID = *n;
    el->type = 'n';
  } else if (isalpha(*t) > 0) {
    strcpy(el->word.w, t);
    el->word.ID = *n;
    el->type = 'w';
  } else {
    el->token.t = *t;
    el->type = 't';
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
    strcpy(resKeys[i].w,keywords[i]);
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
 * @return list token-stream
 **/
list lexer(char *raw_code) {
  keyword *reserved = init_ReservedKeys();
  int i;
  list token_stream, tok;
  l_init(&token_stream);
  

  for (i = 0; i < strlen(raw_code); i++) {
    char w[30] = "";
    int j = 0, key_NUM, ident = IDENTIFIER;
    
    while (raw_code[i] < 33)
      i++;

    if (raw_code[i] == 61 && raw_code[i + 1] == 61) {
      key_NUM = get_keyNUM(reserved, "==");
      l_append(&token_stream, reserved[key_NUM].w, &reserved[key_NUM].ID);
      i++;
    }
    if (raw_code[i] == 60 && raw_code[i + 1] == 61) {
      key_NUM = get_keyNUM(reserved, "<=");
      l_append(&token_stream, reserved[key_NUM].w, &reserved[key_NUM].ID);
      i++;
    }
    if (raw_code[i] == 62 && raw_code[i + 1] == 61) {
      key_NUM = get_keyNUM(reserved, ">=");
      l_append(&token_stream, reserved[key_NUM].w, &reserved[key_NUM].ID);
      i++;
    }
    if (raw_code[i] == 33 && raw_code[i + 1] == 61) {
      key_NUM = get_keyNUM(reserved, "!=");
      l_append(&token_stream, reserved[key_NUM].w, &reserved[key_NUM].ID);
      i++;
    }

    
    if (isalpha(raw_code[i])) {
      do {
	w[j] = raw_code[i];
	j++;
	i++;
      } while (isalnum(raw_code[i]));
      key_NUM = get_keyNUM(reserved, w);
      if (key_NUM >= 0) l_append(&token_stream, reserved[key_NUM].w, &reserved[key_NUM].ID);
      else l_append(&token_stream, w, &ident);
    }
   
    ident = NUM;
    if (isdigit(raw_code[i])) {
      do {
	w[j] = raw_code[i];
	j++;
	i++;
      } while (isdigit(raw_code[i]));
      l_append(&token_stream, w, &ident);
    }

    if (strlen(w) > 0) i--;
    else l_append(&token_stream, &raw_code[i], &ident);
    
    tok = l_last(token_stream);
    switch(tok->type) {
      case 't': printf("Token: %c\n", tok->token.t);
		break;
      case 'w': printf("Word: %s, %d\n", tok->word.w, tok->word.ID);
		break;
      case 'n': printf("Number: %d, %d\n", tok->number.n, tok->number.ID);
		break;
    }
  }
  return token_stream;
} 


#endif