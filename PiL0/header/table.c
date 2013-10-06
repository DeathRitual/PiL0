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
 * @file table.c Library for Table Generation
 *
 * @ingroup parser
 */

#include"frontend.h"

#define ST_NAME_TABLE "Symbol-Table"

/**
 * @struct TABLE_ENTRY
 *
 * @brief entry for symbol table
 *
 **/
struct TABLE_ENTRY {
	char word[MAX_LENGTH]; /**< symbol name */
	int type_ID; /**< symbol ID */
};

struct SYMBOL_TABLE {
		HASHTABLE hash;
		struct SYMBOL_TABLE *last;
};

/**
 * @brief cast given object to table element type
 *
 * @param *obj object to cast
 * @retval TEPTR element as type of table element
 */
static TEPTR stcast(void *obj) {
	return (TEPTR) obj;
}

STPTR initSymbolTable(const STPTR last) {
	STPTR new_st = NULL;
	TEPTR type;

	if ((new_st = malloc(sizeof(*new_st))) == NULL)
		ERROR_EXCEPT(ST_NAME_TABLE, ERR_MEMORY);

	new_st->hash = init_hash(type, 13, stcast);
	new_st->last = last;

	return new_st;
}

void addToSymbolTable(const STPTR st, const char *w, const int n) {
	TEPTR new_entry = NULL;

	if ((new_entry = malloc(sizeof(*new_entry))) == NULL)
		ERROR_EXCEPT(ST_NAME_TABLE, ERR_MEMORY);

	strcpy(new_entry->word, w);
	new_entry->type_ID = n;

	insertHash(st, new_entry->word, new_entry);
}

int lookUpSymbolTable(const STPTR st, const char *w) {
	return getHash(st, w) != NULL;
}

TEPTR getElement(const STPTR st, const char *w) {
	TEPTR lookup = NULL;
	if ((lookup = (TEPTR) getHash(st, w)) == NULL)
		ERROR_EXCEPT(ST_NAME_TABLE, NO_ELEMENT);
	return lookup;
}

void

/**
 * @brief clean symbol table after scope finish
 *
 * @param *symbol_table pointer to symbol table
 * @retval void
 **/
void stclean(STACK symbol_table) {
	TEPTR tmp = pop(symbol_table);

	while (tmp->type_ID != -1) {
		free(tmp);
		tmp = NULL;
		tmp = pop(symbol_table);
	}

	free(tmp);
}
