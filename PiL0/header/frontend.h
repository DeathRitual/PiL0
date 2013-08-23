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
 * @file frontend.h Header-File for lexer and parser library
 *
 * Forwards the functions of the lexer and parser library to another file which includes this header file.
 *
 * @ingroup global
 */

#ifndef __FRONTEND_H
#define __FRONTEND_H
#include"meta_data_types.h"
#include"global.h"
#include<string.h>
#include<ctype.h>

#define PL_DEBUG   1
#define MAX_LENGTH 30

typedef struct TOKEN_OBJECT *TOPTR;
typedef struct TABLE_ENTRY *TEPTR;
typedef struct AST_BLOCK *AST_BLOCK_PTR;
typedef struct AST_STMT *AST_STMT_PTR;
typedef struct AST_EXPR *AST_EXPR_PTR;
typedef struct SOURCE_OBJECT *SOURCECODE;

/* for manipulating and accessing global source code object */
extern void sc_set_ts(SOURCECODE, const QUEUE);
extern QUEUE sc_get_ts(const SOURCECODE);
extern void sc_set_st(SOURCECODE, const STACK);
extern STACK sc_get_st(const SOURCECODE);
extern void sc_set_ast_bl(SOURCECODE, const AST_BLOCK_PTR);
extern AST_BLOCK_PTR sc_get_ast_bl(const SOURCECODE);
extern void sc_set_ast_st(SOURCECODE, const AST_STMT_PTR);
extern AST_STMT_PTR sc_get_ast_st(const SOURCECODE);
extern void sc_set_ast_ex(SOURCECODE, const AST_EXPR_PTR);
extern AST_EXPR_PTR sc_get_ast_ex(const SOURCECODE);

/* for lexical analysis and access to the generated token */
extern void lexer(SOURCECODE, FILE *);
extern TOPTR generate_token(const char *, const int *, const int *);
extern void free_token(TOPTR);
extern void release_token(const QUEUE);
extern TOPTR getTOKEN(const QUEUE);
extern char getType(const QUEUE);
extern size_t getLine(const QUEUE);
extern int getNumber(const QUEUE);
extern int getNumberID(const QUEUE);
extern char *getWord(const QUEUE);
extern int getWordID(const QUEUE);
extern char getToken(const QUEUE);

/* used by parsing and symbol table generating / accessing */
extern int init_parsing(SOURCECODE);
extern TEPTR generate_tableEntry(const char *, const int);
extern void stclean(STACK);
extern TEPTR stlookup(STACK, const char *);
extern int st_get_typeID(TEPTR);

/* functions for generating abstract syntax tree */
extern AST_BLOCK_PTR init_block();
extern void block_init_procedure(AST_BLOCK_PTR, const char *);
extern AST_BLOCK_PTR block_get_function(const AST_BLOCK_PTR);
extern AST_BLOCK_PTR block_get_main(const AST_BLOCK_PTR);
extern AST_STMT_PTR block_init_statement(AST_BLOCK_PTR);
extern void stmt_init_care(AST_STMT_PTR, const char *);
extern AST_EXPR_PTR stmt_init_print(AST_STMT_PTR);
extern void stmt_init_jumpbac(AST_STMT_PTR);
extern AST_EXPR_PTR stmt_get_jumpbac_condition(const AST_STMT_PTR);
extern AST_STMT_PTR stmt_get_jumpbac_statement(const AST_STMT_PTR);
extern void stmt_init_jumpfor(AST_STMT_PTR);
extern AST_EXPR_PTR stmt_get_jumpfor_condition(const AST_STMT_PTR);
extern AST_STMT_PTR stmt_get_jumpfor_statement(const AST_STMT_PTR );
extern AST_EXPR_PTR stmt_init_assignment(AST_STMT_PTR, const char *);
extern void stmt_init_sequence(AST_STMT_PTR);
extern AST_STMT_PTR stmt_get_sequence_left(const AST_STMT_PTR);
extern AST_STMT_PTR stmt_get_sequence_right(const AST_STMT_PTR);
extern void expr_init_number(AST_EXPR_PTR, const int);
extern void expr_init_identifier(AST_EXPR_PTR, const char *);
extern void expr_init_arithmetic(AST_EXPR_PTR);
extern void expr_arithmetic_set_op(AST_EXPR_PTR, const char);
extern AST_EXPR_PTR expr_get_arithmetic_left(const AST_EXPR_PTR);
extern AST_EXPR_PTR expr_get_arithmetic_right(const AST_EXPR_PTR);
extern void expr_init_relation(AST_EXPR_PTR);
extern void expr_relation_set_op(AST_EXPR_PTR, const char *);
extern AST_EXPR_PTR expr_get_relation_left(const AST_EXPR_PTR);
extern AST_EXPR_PTR expr_get_relation_right(const AST_EXPR_PTR);
extern AST_EXPR_PTR expr_init_unary(AST_EXPR_PTR, const char);
extern AST_EXPR_PTR expr_init_odd(AST_EXPR_PTR);


/**
 * @enum special_IDs identifier number for variables and numbers
 *
 * identifying numbers start at 300.
 *
 * @note
 * If keywords exceds more than 44 words, than you have to increase the special_IDs initial number
 */
enum special_IDs {
	BEGIN = 256,
	CALL,
	CONST,
	DO,
	END,
	IF,
	ODD,
	PRINT,
	PROCEDURE,
	READ,
	THEN,
	VAR,
	WHILE,
	PASS,
	EQ,
	GE,
	LE,
	NE,
	IDENTIFIER,
	NUM
};

#endif
