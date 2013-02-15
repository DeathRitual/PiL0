/**
 * @file pl.c Main source file for PiL0-Compiler
 * 
 * 
 */

#include"header/lexer.h"
#include"header/parser.h"
#include<stdlib.h>
#include <string.h>
#define DEBUG	printf("test");


int main(int argc, char *argv[]) {
  int i = 10;
  list token_stream, tok;

  FILE *raw_code;
  
  raw_code = fopen(argv[1], "r");
  
  if(raw_code != NULL) {
    token_stream = lexer(token_stream, raw_code); 
  } else {
    printf("Couldn't open Source Code!\n");
    return EXIT_FAILURE;
  }
  
  
  while (token_stream != NULL) { 
      tok = l_top(token_stream);
      switch(tok->element.type) {
	case 't': printf("Token: %c\n", tok->element.token.t);
		  break;
	case 'w': printf("Word: %s, %d\n", tok->element.word.w, tok->element.word.ID);
		  break;
	case 'n': printf("Number: %d, %d\n", tok->element.number.n, tok->element.number.ID);
		  break;
      }
      
      l_remove(&token_stream);
  }
  
  /* Testoutput for symbol-table */
  printf("\n");
  env_ptr st;
  st_init(&st);
  st_append(&st);
  char a[10] = "Hallo";
  int j = 3;
  table_append(&st->st, a, &j);
  printf("%s, %d\n", st->st->word, st->st->type_ID);
  strcpy(a, "Haffo");
  j = 6;
  table_append(&st->st, a, &j);
  printf("%s, %d\n", st->st->word, st->st->type_ID);
  st_append(&st);
  strcpy(a, "Laffo");
  j = 6;
  table_append(&st->st, a, &j);
  printf("%s, %d\n", st->st->word, st->st->type_ID);

  return EXIT_SUCCESS;
}









