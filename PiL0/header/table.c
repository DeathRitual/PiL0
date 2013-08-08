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

#define TABLE "Symbol-Table"

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

/**
 * @brief create new symbol table entry
 *
 * @param *w Symbol which should be stored
 * @param n identifier-type ID
 * @retval new_entry
 **/
TEPTR generate_tableEntry(const char *w, const int n) {
	TEPTR new_entry = NULL;
	
	if ((new_entry = malloc(sizeof(*new_entry))) == NULL)
		error(TABLE, __FILE__, __func__, __LINE__, ERR_MEMORY);
	
	strcpy(new_entry->word, w);
	new_entry->type_ID = n;
	return new_entry;
}

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

/**
 * @brief compare word of two table entries
 *
 * @param comp1 entry one
 * @param comp2 entry two
 * @retval int
 */
static int stcompare(TEPTR comp1, TEPTR comp2) {
	if (comp1 == NULL || comp2 == NULL)
		return 0;
	else
		return strcmp(comp1->word, comp2->word);
}

/**
 * @brief cast given object to table element type
 *
 * @param *obj object to cast
 * @retval TEPTR element as type of table element
 */
static TEPTR stcast(void *obj) {
	return (TEPTR) obj;
}

/**
 * @brief look for word in symbol table and return it
 *
 * @param symbol_table symbol table
 * @param *w word looking for
 * @retval TEPTR
 */
TEPTR stlookup(STACK symbol_table, const char *w) {
	TEPTR tmp = NULL;
	
	if ((tmp = malloc(sizeof(*tmp))) == NULL)
		error(TABLE, __FILE__, __func__,
		__LINE__, ERR_MEMORY);
	
	strcpy(tmp->word, w);
	tmp->type_ID = 0;
	
	return (TEPTR) linst(symbol_table, tmp, (void *(*)(void *)) stcast,
			(int (*)(void *, void *)) stcompare);
}

/**
 * @brief get type ID from table entry
 *
 * @param te pointer to table entry
 * @retval int
 */
int st_get_typeID(TEPTR te) {
	return (te == NULL) ? 0 : te->type_ID;
}
