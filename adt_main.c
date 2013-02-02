#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "adt.h"

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
};

/* Hauptprogramm */

void main(void) {
  
  list l;
  struct _tag_list *ptr;
  
  l_init(&l);
  
  int b = 6;
  
  char g[10];
  
  strcpy(g, "h");
  
  l_append(&l, g, 0);
  
  ptr = l_top(l);
  
  printf("%c\n", ptr->token.t);
  
  printf("%c\n", ptr->type);
  
  strcpy(g, "1400");
  
  
  l_append(&l, g, &b);
  
  ptr = l_last(l);
  
  printf("%d\n", ptr->number.n);
  printf("%c\n", ptr->type);
  
  l_remove(&l);
  l_remove(&l);
  
  strcpy(g, "fgdhdf");
  b = 17;
  
  l_append(&l, g, &b);
  
  ptr = l_top(l);
  
  printf("%s\n", ptr->word.w);
  printf("%d\n", ptr->word.ID);
  printf("%c\n", ptr->type);
}


