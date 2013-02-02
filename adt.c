/* Header adt.h - abstract data types like stack (FIFO) */

#include "adt.h"
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#ifndef ADT_C

/*
 * 
 * Error Handling
 * 
*/

/* error messages */
static char *err_msg[] = {
    "Null-Pointer", 		"Stack ist leer", 
    "Stack ueberschreibt", 	"Kein Speicher frei",
    "Liste ist leer"
  };
  
/* error codes */
enum err_codes {
    NULL_POINTER, EMPTY_STACK, STACK_OVERWRITE, ERR_MEMORY, EMPTY_LIST
  };
  
/* print error messages */
static void error(enum err_codes msg_nr) {
    fprintf(stderr, "FEHLER(intadt): %s!\n", err_msg[msg_nr]);
    exit(10);
  }

  
/*
 * 
 * FILO Implementation
 * 
/* 

/* stack element */
typedef struct _tag_stack *stack;


struct _tag_stack {
  unsigned int alter;
  struct _tag_stack *next;
};   
  
/* initialise stack with NULL-Pointer */
void s_init(stack *s) {
  if (s == NULL) error(NULL_POINTER);
  //if (*s != NULL) error(STACK_OVERWRITE);
  *s = NULL;
}

/* check if stack is empty */
int s_IsEmpty(stack s) {
  return s == NULL;
}

/* put element on given stack */
void push(stack *s, int *n) {
  if (s == NULL) error(NULL_POINTER);
  struct _tag_stack *el;
  if ((el = malloc(sizeof(stack))) == NULL) error(ERR_MEMORY);
  el->alter = *n;
  el->next = *s;
  *s = el;
}

/* drop element from given stack */
void pop(stack *s) {
  if (s == NULL) error(NULL_POINTER);
  if (s_IsEmpty(*s)) error(EMPTY_STACK);
  struct _tag_stack *old;
  old = *s;
  *s = (*s)->next;
  free(old);
}

/* print top element of stack */
int s_top(stack s){
  if (s_IsEmpty(s)) error(EMPTY_STACK);
  return s->alter;
}

/* remove all elements from stack */
void s_free(stack *s){
  if (s_IsEmpty(*s)) error(EMPTY_STACK);
  struct _tag_stack *run1, *run2;
  run1 = *s;
  while(run1 != NULL) {
    run2 = run1->next;
    free(run1);
    run1 = run2;
  }
  *s = NULL;
}


/*
 * 
 * FIFO Implementation
 * 
/* 

/* list element */
typedef struct _tag_list *list;

struct _tag_list {
  unsigned int alter;
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

void l_append(list *l, int *n) {
  if (l == NULL) error(NULL_POINTER);
  struct _tag_list *el;
  if ((el = malloc(sizeof(list))) == NULL) error(ERR_MEMORY);
  el->alter = *n;
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

int l_top(list l) {
  if (l_IsEmpty(l)) error(NULL_POINTER);
  return head->alter;
}

int l_last(list l) {
  if (l_IsEmpty(l)) error(NULL_POINTER);
  return l->alter;
}

#endif