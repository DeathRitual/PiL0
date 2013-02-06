/**
 * @file lexer.c library for reading Symbol-Stream and converting it into Token-Stream
 * 
 * bla bla bla
 */

#include "tokdef.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>



void main(void) {

  struct key_array reserved = init_ReservedKeys();
  
  printf("Wort: %s, ID: %d\n", reserved.resKeys[15].w, reserved.resKeys[15].ID);


} 


