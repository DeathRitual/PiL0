/**
 * @file pl.c Main source file for PiL0-Compiler
 * 
 * 
 */

#include"header/lexer.h"
#include<stdlib.h>
#define DEBUG	printf("test");
#ifdef __MSDOS__
int main(int argc, char *argv[]) {
  printf("PiL0 not tested on MSDOS!\n");
  return EXIT_SUCCESS;
}

#elif __WIN32__ || _MSC_VER
int main(int argc, char *argv[]) {
  printf("PiL0 will not run correctly on Win32!\n");
  return EXIT_SUCCESS;
}

#elif __unix__ || __linux__
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
  
  return EXIT_SUCCESS;
}
#endif