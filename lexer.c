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

keyword *reserved = init_ReservedKeys();
  list token_stream;
	l_init(&token_stream);

	int key_NUM = get_keyNUM(reserved, "BEGIN");
	printf("%s, %d", reserved[key_NUM].w, reserved[key_NUM].ID);
	l_append(&token_stream, "BEGIN", (int *)256);
	//l_append(&token_stream, reserved[key_NUM].w, (int *)reserved[key_NUM].ID);

	char string[50] = "BEGIN\n\tVAR i = 234 + a;\nEND";
	printf("\n%s\n\n", string);

	int i;
	for (i = 0; i < strlen(string); i++) {
	  while (string[i] < 33)
		  i++;

	  if (string[i] == 61 && string[i + 1] == 61) {
		  printf("Token: ==");
		  i++;
	  }
	  if (string[i] == 60 && string[i + 1] == 61) {
		  printf("Token: <=");
		  i++;
	  }
	  if (string[i] == 62 && string[i + 1] == 61) {
		  printf("Token: >=");
		  i++;
	  }
	  if (string[i] == 33 && string[i + 1] == 61) {
		  printf("Token: !=");
		  i++;
	  }

	  char w[20] = "";
	  int j = 0;
	  if (isalpha(string[i])) {
		  do {
			  w[j] = string[i];
			  j++;
			  i++;
		  } while (isalnum(string[i]));
		  //int key_NUM = get_keyNUM(reserved, w);
		  //if (key_NUM > 0) l_append(&token_stream, reserved[key_NUM].w, (int *)reserved[key_NUM].ID);
		  //else l_append(&token_stream, w, (int *)IDENTIFIER);
	  }

	  if (isdigit(string[i])) {
		  do {
			  w[j] = string[i];
			  j++;
			  i++;
		  } while (isdigit(string[i]));
		  //l_append(&token_stream, w, (int *)NUM);
	  }

	  if (strlen(w) > 0) i--;
	  else printf("test");//l_append(&token_stream, &string[i], (int *)NUM);
	}


} 


