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
 * @file lexer.c Library for Token-Stream
 *
 * Contains data structures and functions to implement a linear token stream.
 *
 *
 * @defgroup lexer Lexical Scanner
 * @brief Scans input source file for pre defined tokens and put them together in a LIFO structure
 * @ingroup global lexer
 */

#include"frontend.h"
#include"language.h"

#define LEXER "Lexer"

typedef struct {
	char w[MAX_LENGTH]; /**< keyword */
	int ID; /**< keyword identifier */
/**
 * @struct keyword
 *
 * @brief Keyword structure for storing initial keywords
 *
 * Stores string and ID of the language keywords like DO, WHILE, IF, etc.
 **/
} keyword;

/**
 * @brief Returns the number of keyword
 *
 * Checks if the given word is a keyword and returns its position number from the key_array
 *
 * @param k array of keywords
 * @param s string to be checked
 * @retval i either position number of keyword or -1 if word is not stored in keyword array
 **/
static int get_keyNUM(const keyword *k, const char *s) {
	int i;

	for (i = 0; keywords[i] != NULL; i++)
		if (strcmp(k[i].w, s) == 0)
			return i;

	return -1;
}

/**
 * @brief counts of keywords
 *
 * @retval i number of keywords
 **/
static unsigned int get_keySize() {
	int i = 1;

	while (keywords[i] != NULL)
		i++;

	return i;
}

/**
 * @brief creates an array of all keywords
 *
 * @retval reskeys pointer to keyword array
 **/
static keyword *init_ReservedKeys() {
	keyword *resKeys = NULL;

	int i;

	if ((resKeys = malloc(sizeof *resKeys * get_keySize())) == NULL)
		error("KEYgetWord(lexical_scan)", __FILE__, __func__, __LINE__,
				ERR_MEMORY);

	for (i = 0; keywords[i] != NULL; i++) {
		strcpy(resKeys[i].w, keywords[i]);
		resKeys[i].ID = 256 + i;
	}

	return resKeys;
}

/**
 * @brief Function for lexical scanning
 *
 * Reads input code and convert each element to one token / word / number and add it to the token stream
 *
 * @param *code object which stores all data for source code
 * @param *raw_code pl0 source code
 * @retval void
 **/
void lexer(SOURCECODE code, FILE *raw_code) {
	int c, lineNumber = 1;
	keyword *reserved = init_ReservedKeys();
	QUEUE token_stream = NULL;

	sc_set_ts(code, init_queue());
	token_stream = sc_get_ts(code);


	while ((c = fgetc(raw_code)) != EOF) {
		if (c == 10 || c == 13)
			lineNumber++;

		if (!((iscntrl(c) || isspace(c)))) {
			char w[30] = "";
			int i = 0, key_NUM = 0, ident = IDENTIFIER;

			/* read compare operators */
			if (c == '=' || c == '>' || c == '<' || c == '!') {
				w[i] = c;

				if ((c = fgetc(raw_code)) == '=') {
					w[i + 1] = c;

					if (strcmp(w, "==") == 0)
						key_NUM = get_keyNUM(reserved, "EQ");

					if (strcmp(w, ">=") == 0)
						key_NUM = get_keyNUM(reserved, "GE");

					if (strcmp(w, "<=") == 0)
						key_NUM = get_keyNUM(reserved, "LE");

					if (strcmp(w, "!=") == 0)
						key_NUM = get_keyNUM(reserved, "NE");

					append(token_stream,
							generate_token(reserved[key_NUM].w,
									&reserved[key_NUM].ID, &lineNumber));
				}

				else
					append(token_stream,
							generate_token(w, &ident, &lineNumber));
			}

			/* read words or identifier */
			if (isalpha(c)) {
				w[i] = c;
				c = fgetc(raw_code);

				while (isalnum(c)) {
					w[++i] = c;
					c = fgetc(raw_code);
				}

				key_NUM = get_keyNUM(reserved, w);

				if (key_NUM >= 0)
					append(token_stream,
							generate_token(reserved[key_NUM].w,
									&reserved[key_NUM].ID, &lineNumber));
				else
					append(token_stream,
							generate_token(w, &ident, &lineNumber));

				c = ungetc(c, raw_code);
			}

			/*read numbers */
			if (isdigit(c)) {
				ident = NUM;
				w[i] = c;
				c = fgetc(raw_code);

				while (isdigit(c)) {
					w[++i] = c;
					c = fgetc(raw_code);
				}

				append(token_stream, generate_token(w, &ident, &lineNumber));
				c = ungetc(c, raw_code);
			}

			/* read tokens */
			if (strlen(w) == 0) {
				w[i] = c;
				append(token_stream, generate_token(w, &ident, &lineNumber));
			}
		}
	}
}

/** @} */

