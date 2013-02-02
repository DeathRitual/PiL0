#include<stdio.h>
#include<stdlib.h>
#include "adt.h"


/* Hauptprogramm */

void main(void) {
  
  list l;

  
  l_init(&l);
  
  int a = 5, b = 6;
  
  l_append(&l, &a);
  l_remove(&l);
  l_append(&l, &b);
  l_remove(&l);
  b = 7;
  l_append(&l, &b);
  
  printf("l_top: %d\n", l_top(l));
  printf("l_last: %d\n", l_last(l));
}


