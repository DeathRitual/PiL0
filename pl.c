/**
 * @file pl.c Main source file for PiL0-Compiler
 * 
 * 
 */

#include"header/lexer.h"
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]) {
  int c, i = 0;
  char code[1000];
  FILE *raw_code;
  
  raw_code = fopen(argv[1], "r");
  
  if(raw_code != NULL) {
    while ((c = fgetc(raw_code)) != EOF) {
      code[i] = c;
      i++;
    }
  } else {
      printf("Konnte Datei nicht finden bzw. öffnen!\n");
      return EXIT_FAILURE;
   }
  
  lexer(code);
  
  return EXIT_SUCCESS;
}
