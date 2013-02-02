/* Header adt.h - abstract data types like stack (FIFO) */

#include "adt.h"
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#ifndef LEXER_C

/*
 * 
 * Error Handling
 * 
*/

/* error messages */
static char *err_msg[] = {
    "Null-Pointer", "Kein Speicher frei",
    "Liste ist leer"
  };
  
/* error codes */
enum err_codes {
    NULL_POINTER, ERR_MEMORY, EMPTY_LIST
  };
  
/* print error messages */
static void error(enum err_codes msg_nr) {
    fprintf(stderr, "FEHLER(intlist): %s!\n", err_msg[msg_nr]);
    exit(10);
  }


/*
 * 
 * FIFO Implementation
 * 
/* 

/* list element */

struct _tag_list {
  union {
    struct _token {
      char t;
    }token;
    
    struct _number {
      int n;
      unsigned int ID;
    }number;
    
    struct _word {
      char w[50];
      unsigned int ID;
    }word;
  };

  char type;
  struct _tag_list *next;
  struct _tag_list *previous;
};

struct _tag_list *head;

void l_init(list *l) {
  if (l == NULL) error(NULL_POINTER);
  *l = NULL; 
}

int l_IsEmpty(list l) {
  return l == NULL;
} 

void l_append(list *l, char *t, int *n) {
  if (l == NULL) error(NULL_POINTER);
  struct _tag_list *el;
  if ((el = malloc(sizeof(list))) == NULL) error(ERR_MEMORY);

  if (strlen(t) == 1) {
    el->token.t = *t;
    el->type = 't';
  } else if (isdigit(*t) > 0) {
    el->number.n = atoi(t);
    fprintf(stderr, "");
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

list l_top(list l) {
  if (l_IsEmpty(l)) error(NULL_POINTER);
  return head;
}

list l_last(list l) {
  if (l_IsEmpty(l)) error(NULL_POINTER);
  return l;
}

#endif