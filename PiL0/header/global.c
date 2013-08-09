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
 * @file global.c Library for defining global objects used by the whole compiler
 * @ingroup global
 **/

#include"frontend.h"

#define SC_ERR "Source-Code Object"

/**
 * @struct SOURCE_OBJECT
 *
 * @brief Global object which stores all pointers and temporary information.
 *
 **/
struct SOURCE_OBJECT {
	QUEUE token_stream; 		/**< pointer to token stream */
	STACK symbol_table; 		/**< pointer to symbol table */
	AST_BLOCK_PTR block_tmp; 	/**< pointer to temporary block */
	AST_STMT_PTR stmt_tmp; 		/**< pointer to temporary statement */
	AST_EXPR_PTR expr_tmp; 		/**< pointer to temporary expression */
};

/**
 * @brief initialize new global source code object
 *
 * @retval SOURCECODE
 */
static SOURCECODE sc_init() {
	SOURCECODE new_code = NULL;

	if ((new_code = malloc(sizeof(*new_code))) == NULL)
		error(SC_ERR, __FILE__, __func__, __LINE__, ERR_MEMORY);

	new_code->symbol_table = NULL;
	new_code->token_stream = NULL;
	new_code->block_tmp = NULL;
	new_code->stmt_tmp = NULL;
	new_code->expr_tmp = NULL;

	return new_code;
}

/**
 * @brief delete global source code object
 *
 * @param sc pointer to source code object
 * @retval void
 */
static void sc_destroy(SOURCECODE sc) {
	free(sc);
	sc = NULL;
}

/**
 * @brief set token stream
 *
 * @param *sc pointer to source code
 * @param *ts pointer to token stream
 * @retval void
 */
void sc_set_ts(SOURCECODE sc, const QUEUE ts) {
	sc->token_stream = ts;
}

/**
 * @brief return token stream
 *
 * @param sc pointer to source code
 * @retval sc->token_stream
 */
QUEUE sc_get_ts(const SOURCECODE sc) {
	return sc->token_stream;
}

/**
 * @brief set symbol table
 *
 * @param sc pointer to source code
 * @param st pointer to symbol table
 * @retval void
 */
void sc_set_st(SOURCECODE sc, const STACK st) {
	sc->symbol_table = st;
}

/**
 * @brief get symbol table
 *
 * @param sc pointer to source code
 * @retval sc->symbol_table
 */
STACK sc_get_st(const SOURCECODE sc) {
	return sc->symbol_table;
}

/**
 * @brief set AST block
 *
 * @param sc pointer to source code
 * @param b pointer to AST block
 * @retval void
 */
void sc_set_ast_bl(SOURCECODE sc, const AST_BLOCK_PTR b) {
	sc->block_tmp = b;
}

/**
 * @brief return AST block
 *
 * @param sc pointer to source code
 * @retval sc->block_tmp
 */
AST_BLOCK_PTR sc_get_ast_bl(const SOURCECODE sc) {
	return sc->block_tmp;
}

/**
 * @brief set AST statement
 *
 * @param sc pointer to source code
 * @param s pointer to AST statement
 * @retval void
 */
void sc_set_ast_st(SOURCECODE sc, const AST_STMT_PTR s) {
	sc->stmt_tmp = s;
}

/**
 * @brief return AST statement
 *
 * @param sc pointer to source code
 * @retval sc->stmt_tmp
 */
AST_STMT_PTR sc_get_ast_st(const SOURCECODE sc) {
	return sc->stmt_tmp;
}

/**
 * @brief set AST expression
 *
 * @param sc pointer to source code
 * @param e pointer to AST expression
 * @retval void
 */
void sc_set_ast_ex(SOURCECODE sc, const AST_EXPR_PTR e) {
	sc->expr_tmp = e;
}

/**
 * @brief return AST expression
 *
 * @param sc pointer to source code
 * @retval sc->expr_tmp
 */
AST_EXPR_PTR sc_get_ast_ex(const SOURCECODE sc) {
	return sc->expr_tmp;
}

/**
 * @brief compile handler which starts lexing and parsing
 *
 * @param raw_code pl0 source code
 * @retval int TRUE or FALSE
 */
int compile(FILE *raw_code) {
	SOURCECODE pl0_code = sc_init();
	int status;

	puts("Start lexical scanning...");

	lexer(pl0_code, raw_code);

	puts("Finished lexical scanning!\n");

	puts("Start parsing...\n");

	status = init_parsing(pl0_code);

	sc_destroy(pl0_code);

	return status;
}
