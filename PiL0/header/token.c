/*
 * PiL0 - PL0 Compiler for Raspberry PI
 * Copyright (C) 2013  Philipp Wiesner
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file token.c Library for Token Generation and Global Objects
 *
 * @ingroup lexer
 */

#include"frontend.h"
#ifndef __TOKEN_C
#define __TOKEN_C

#define TOKEN_ERR "Token"

/**
 * @brief error exception for null pointer in token pointer (token_element).
 */
#define MD_NULL_POINTER_TOPTR if (token_element == NULL) ERROR_EXCEPT(TOKEN_ERR, NULL_POINTER)
/**
 * @brief error exception for null pointer in token queue (token_queue).
 */
#define MD_NULL_POINTER_TOKQU if (token_queue == NULL) ERROR_EXCEPT(TOKEN_ERR, NULL_POINTER)



/**
 * @struct TOKEN_OBJECT
 *
 * @brief stores one token, token type and line number
 *
 **/
struct TOKEN_OBJECT {
	char type; /**< Token-Type */
	size_t line; /**< code line number */

	/**
	 * @union un_element
	 *
	 * @brief Different Token for storing symbols.
	 *
	 * Except for Token: token all token have an ID which helps identifying
	 * the type of the keyword, identifier or number.
	 **/
	union un_element {
		/**
		 * @struct st_token
		 *
		 * Stores single character symbols like: +, -, >, <, etc.
		 **/
		struct st_token {
			char t; /**< single character */
		} token;

		/**
		 * @struct st_number
		 *
		 * Stores numbers.
		 **/
		struct st_number {
			int n; /**< number */
			unsigned int ID; /**< number identifier */
		} number;

		/**
		 * @struct st_word
		 *
		 * Stores keywords and identifier.
		 **/
		struct st_word {
			unsigned int ID; /**< keyword / identifier identifier */
			char w[MAX_LENGTH]; /**< keyword / identifier */
		} word;
	} element;
};

/**
 * @brief create new token
 *
 * @param *t symbol, keyword, identifier or number to generate new token of
 * @param *n identifier number of keyword, identifier or number
 * @param *ln number of program code line
 * @retval *new_token_element pointer to new token
 **/
TOPTR generate_token(const char *t, const int *n, const int *ln) {
	TOPTR new_token_element = NULL;

	if ((new_token_element = malloc(sizeof(*new_token_element))) == NULL)
		ERROR_EXCEPT(TOKEN_ERR, ERR_MEMORY);

	if (isdigit(*t) > 0) {
		new_token_element->type = 'n';
		new_token_element->element.number.n = atoi(t);
		new_token_element->element.number.ID = *n;
	}

	else if (isalpha(*t) > 0) {
		new_token_element->type = 'w';
		strcpy(new_token_element->element.word.w, t);
		new_token_element->element.word.ID = *n;
	}

	else {
		new_token_element->type = 't';
		new_token_element->element.token.t = *t;
	}

	new_token_element->line = *ln;
	return new_token_element;
}

/**
 * @brief free memory for token
 *
 * @param token_element token pointer
 * @return void
 */
void free_token(TOPTR token_element) {
	MD_NULL_POINTER_TOPTR;
	free(token_element);
	token_element = NULL;
}

/**
 * @brief remove element from qeue and free memory
 *
 * @param token_queue queue pointer
 * @return void
 */
void release_token(const QUEUE token_queue) {
	TOPTR tmp = NULL;
	MD_NULL_POINTER_TOKQU;

	tmp = (TOPTR) qudel(token_queue);
	free_token(tmp);
	tmp = NULL;
}

/**
 *
 * @brief return token
 *
 * @param token_queue queue pointer
 * @retval TOPTR
 */
TOPTR getTOKEN(const QUEUE token_queue) {
	MD_NULL_POINTER_TOKQU;
	return (TOPTR) head(token_queue);
}

/**
 * @brief return token type
 *
 * @param token_queue queue pointer
 * @retval char
 */
char getType(const QUEUE token_queue) {
	MD_NULL_POINTER_TOKQU;
	return getTOKEN(token_queue)->type;
}

/**
 * @brief return line number of token
 *
 * @param token_queue queue pointer
 * @retval size_t
 */
size_t getLine(const QUEUE token_queue) {
	MD_NULL_POINTER_TOKQU;
	return getTOKEN(token_queue)->line;
}

/**
 * @brief return number of token: number
 *
 * @param token_queue queue pointer
 * @retval int
 */
int getNumber(const QUEUE token_queue) {
	MD_NULL_POINTER_TOKQU;
	return getTOKEN(token_queue)->element.number.n;
}

/**
 * @brief return id of token: number
 *
 * @param token_queue queue pointer
 * @retval int
 */
int getNumberID(const QUEUE token_queue) {
	MD_NULL_POINTER_TOKQU;
	return getTOKEN(token_queue)->element.number.ID;
}

/**
 * @brief return pointer to word of token: word
 *
 * @param token_queue queue pointer
 * @retval char*
 */
char *getWord(const QUEUE token_queue) {
	MD_NULL_POINTER_TOKQU;
	return getTOKEN(token_queue)->element.word.w;
}

/**
 * @brief return id of token: word
 *
 * @param token_queue queue pointer
 * @retval int
 */
int getWordID(const QUEUE token_queue) {
	MD_NULL_POINTER_TOKQU;
	return getTOKEN(token_queue)->element.word.ID;
}

/**
 * @brief return symbol of token: token
 *
 * @param token_queue queue pointer
 * @retval char
 */
char getToken(const QUEUE token_queue) {
	MD_NULL_POINTER_TOKQU;
	return getTOKEN(token_queue)->element.token.t;
}

#endif

/** @} */
