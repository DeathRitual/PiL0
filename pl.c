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
  env_ptr env;
  table_ptr var;
  st_init(&env);
  st_append(&env);
  char b[10] = "Hallo";
  char a[10] = "";
  int j = 3;
  put(&env->st, b, &j);
  printf("%s, %d\n", env->st->word, env->st->type_ID);
  strcpy(a, "Haffo");
  j = 6;
  put(&env->st, a, &j);
  printf("%s, %d\n", env->st->word, env->st->type_ID);
  st_append(&env);
  strcpy(a, "Laffo");
  j = 6;
  put(&env->st, a, &j);
  printf("%s, %d\n", env->st->word, env->st->type_ID);
  var = get(&env, "Haffo"); 
  if (var != NULL) printf("%s", var->word);
  else printf("Not found");
  return EXIT_SUCCESS;
}









