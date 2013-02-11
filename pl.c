/**
 * @file pl.c Main source file for PiL0-Compiler
 * 
 * 
 */

#include "lexer.h"

int main() {
  
  char code[50] = "BEGIN\n\tVAR i = 234 + 2;\nEND";
  
  lexer(code);
  
  return 0;
}
