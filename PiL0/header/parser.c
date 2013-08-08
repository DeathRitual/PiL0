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
 * @file parser.c Library which inherits all necessary functions for parsing
 *
 * @defgroup parser Parser
 * @brief check token stream (LIFO) from parser for predefined grammar
 * @ingroup global
 */

#include"frontend.h"
#define TRUE  1
#define FALSE   0

#ifndef PL_DEBUG
#define MTNT(q)     release_token(q)
#define PARSE_ERR(line, mess)   parseError(line, mess)
#else

static void parse_debout(QUEUE tok) {
	char c;
	fputs("Token: ", stdout);
	switch (getType(tok)) {
		case 'n':
			printf("%d\n", getNumber(tok));
			break;

		case 'w':
			puts(getWord(tok));
			break;

		case 't':
			c = getToken(tok);
			putchar(c);
			putchar('\n');
			break;

		default:
			puts("help!");
			break;
	}
}

#define MTNT(q)    parse_debout(q), release_token(q)
#define PARSE_ERR(line, mess) debug_output(line, mess, __func__, __LINE__)
#endif

/* prototypes */
int init_parsing(SOURCECODE);
void block(SOURCECODE);
void stmt(SOURCECODE);
void condition(SOURCECODE);
void expression(SOURCECODE);
void term(SOURCECODE);
void factor(SOURCECODE);

/*static rootBlock block_ptr = NULL;
 static rootStmt stmt_ptr = NULL;
 static rootExpr expr_ptr = NULL;*/

/**
 * @brief start with parsing process
 *
 * @param *code pointer to source code object
 * @retval TRUE/FALSE 1 or 0
 **/
int init_parsing(SOURCECODE code) {

	int exit_status;
	QUEUE token_stream = sc_get_ts(code);
	STACK symbol_table = NULL;

	sc_set_st(code, init_stack());

	symbol_table = sc_get_st(code);

	sc_set_ast_bl(code, init_block());

	block(code);

	exit_status = (getToken(token_stream) == '.') ? TRUE : FALSE;

	MTNT(token_stream);

	if (empty_stack(symbol_table))
		free_stack(symbol_table);
	if (empty_queue(token_stream))
		free_queue(token_stream);

	puts("\nFinished parsing with status: ");

	return exit_status;
}

/**
 * @brief check block grammar
 *
 * @param *code pointer to source code object
 * @retval void
 **/
void block(SOURCECODE code) {

	STACK symbol_table = sc_get_st(code);
	QUEUE token_stream = sc_get_ts(code);
	AST_BLOCK_PTR block_ptr = sc_get_ast_bl(code);
	AST_BLOCK_PTR block_tmp = NULL;

	push(symbol_table, generate_tableEntry("new scope", -1));

	/* block    -> VAR var_stmt
	 * var_stmt -> var_stmt, identifier | identifier */
	if (getWordID(token_stream) == VAR) {
		MTNT(token_stream);

		do {
			if (getWordID(token_stream) == IDENTIFIER) {
				if (!stlookup(symbol_table, getWord(token_stream)))
					push(symbol_table,
							generate_tableEntry(getWord(token_stream), VAR));
				else
					PARSE_ERR(getLine(token_stream), TYP_DOUB_DEC);

				MTNT(token_stream);
			}

			else
				PARSE_ERR(getLine(token_stream), TYP_NO_ID);

			if (getToken(token_stream) == ',')
				MTNT(token_stream);
			else if (getToken(token_stream) != ';')
				PARSE_ERR(getLine(token_stream), SYN_MISS_COM);
		} while (getToken(token_stream) != ';');

		MTNT(token_stream);
	}

	/* block      -> CONST const_stmt
	 * const_stmt -> const_stmt, identifier = number | identifier = number */
	if (getWordID(token_stream) == CONST) {
		MTNT(token_stream);

		do {
			if (getWordID(token_stream) == IDENTIFIER) {
				if (!stlookup(symbol_table, getWord(token_stream)))
					push(symbol_table,
							generate_tableEntry(getWord(token_stream), CONST));
				else
					PARSE_ERR(getLine(token_stream), TYP_DOUB_DEC);

				MTNT(token_stream);
			}

			else
				PARSE_ERR(getLine(token_stream), TYP_NO_ID);

			if (getToken(token_stream) == '=')
				MTNT(token_stream);
			else
				PARSE_ERR(getLine(token_stream), SYN_MISS_ASS);

			if (getNumberID(token_stream) == NUM)
				MTNT(token_stream);
			else
				PARSE_ERR(getLine(token_stream), TYP_CONST_NUM);

			if (getToken(token_stream) == ',')
				MTNT(token_stream);
			else if (getToken(token_stream) != ';')
				PARSE_ERR(getLine(token_stream), SYN_MISS_COM);
		} while (getToken(token_stream) != ';');

		MTNT(token_stream);
	}

	/* block     -> proc
	 * proc      -> proc proc_stmt | proc_stmt
	 * proc_stmt -> PROCEDURE identifier ; block ; */

	while (getWordID(token_stream) == PROCEDURE) {
		MTNT(token_stream);

		if (getWordID(token_stream) == IDENTIFIER) {
			if (!stlookup(symbol_table, getWord(token_stream)))
				push(symbol_table,
						generate_tableEntry(getWord(token_stream), PROCEDURE));
			else
				PARSE_ERR(getLine(token_stream), TYP_DOUB_DEC);

			MTNT(token_stream);
		} else
			PARSE_ERR(getLine(token_stream), TYP_NO_ID);

		if (getToken(token_stream) == ';')
			MTNT(token_stream);
		else
			PARSE_ERR(getLine(token_stream), SYN_MISS_COM);

		block_init_procedure(block_ptr, top(symbol_table));
		sc_set_ast_bl(code, block_get_function(block_ptr));
		block_tmp = block_get_main(block_ptr);
		block(code);
		block_ptr = block_tmp;

		if (getToken(token_stream) == ';')
			MTNT(token_stream);
		else
			PARSE_ERR(getLine(token_stream), SYN_MISS_COM);
	}

	sc_set_ast_st(code, block_init_statement(block_ptr));
	stmt(code);
	stclean(symbol_table);
}

/**
 * @brief check statement syntax
 *
 * @param *code pointer to source code object
 * @retval void
 **/
void stmt(SOURCECODE code) {

	STACK symbol_table = sc_get_st(code);
	QUEUE token_stream = sc_get_ts(code);
	AST_STMT_PTR statement_ptr = sc_get_ast_st(code);
	AST_STMT_PTR statement_tmp = NULL;
	TEPTR table_entry = NULL;

	switch (getWordID(token_stream)) {
		/* stmt -> identifier = expression */
		case (IDENTIFIER):

			table_entry = stlookup(symbol_table, getWord(token_stream));

			if (table_entry == NULL)
				PARSE_ERR(getLine(token_stream), TYP_ID_NO_IN);
			else if (st_get_typeID(table_entry) == PROCEDURE)
				PARSE_ERR(getLine(token_stream), TYP_ONLY_INT);

			MTNT(token_stream);

			if (getToken(token_stream) == '=')
				MTNT(token_stream);
			else
				PARSE_ERR(getLine(token_stream), SYN_MISS_ASS);

			sc_set_ast_ex(code, stmt_init_assignment(statement_ptr, getWord(token_stream)));

			expression(code);
			break;

		/* stmt -> CALL identifier (only procedure)*/
		case (CALL):

			MTNT(token_stream);

			table_entry = stlookup(symbol_table, getWord(token_stream));

			if (table_entry == NULL)
				PARSE_ERR(getLine(token_stream), TYP_ID_NO_IN);
			else if (st_get_typeID(table_entry) != PROCEDURE)
				PARSE_ERR(getLine(token_stream), TYP_ONLY_PROC);

			stmt_init_care(statement_ptr, getWord(token_stream));
			MTNT(token_stream);
			break;

			/* stmt -> READ identifier (only procedure)*/
		case (READ):

			MTNT(token_stream);

			table_entry = stlookup(symbol_table, getWord(token_stream));

			if (table_entry == NULL)
				PARSE_ERR(getLine(token_stream), TYP_ID_NO_IN);
			else if (st_get_typeID(table_entry) == PROCEDURE)
				PARSE_ERR(getLine(token_stream), TYP_ONLY_INT);

			stmt_init_care(statement_ptr, getWord(token_stream));
			MTNT(token_stream);
			break;

			/* stmt -> PRINT expression */
		case (PRINT):

			MTNT(token_stream);
			sc_set_ast_ex(code, stmt_init_print(statement_ptr));
			expression(code);

			break;

			/* stmt  -> BEGIN stmts END
			 * stmts -> stmts ; stmt | stmt */
		case (BEGIN):

			do {
				MTNT(token_stream);
				stmt_init_sequence(statement_ptr);
			    sc_set_ast_st(code, stmt_get_sequence_left(statement_ptr));
			    statement_tmp = stmt_get_sequence_right(statement_ptr);
				stmt(code);
				statement_ptr = statement_tmp;
			} while (getToken(token_stream) == ';');

			if (getWordID(token_stream) == END)
				MTNT(token_stream);
			else
				PARSE_ERR(getLine(token_stream), SYN_MISS_END);

			break;

			/* stmt -> IF condition THEN stmt */
		case (IF):

			MTNT(token_stream);
			stmt_init_jumpfor(statement_ptr);
			sc_set_ast_ex(code, stmt_get_jumpfor_condition(statement_ptr));
			condition(code);

			if (getWordID(token_stream) == THEN)
				MTNT(token_stream);
			else
				PARSE_ERR(getLine(token_stream), SYN_IF);

			statement_ptr = stmt_get_jumpfor_statement(statement_ptr);
			stmt(code);
			break;

			/* stmt -> WHILE condition DO stmt */
		case (WHILE):

			MTNT(token_stream);
			stmt_init_jumpbac(statement_ptr);
			sc_set_ast_ex(code, stmt_get_jumpbac_condition(statement_ptr));
			condition(code);

			if (getWordID(token_stream) == DO)
				MTNT(token_stream);
			else
				PARSE_ERR(getLine(token_stream), SYN_WHILE);

			statement_ptr = stmt_get_jumpbac_statement(statement_ptr);
			stmt(code);
			break;

			/* stmt -> PASS */
		case (PASS):

			MTNT(token_stream);
			break;
	}

	sc_set_ast_st(code, statement_ptr);
}

/**
 * @brief check condition syntax
 *
 * @param *code pointer to source code object
 * @retval void
 **/
void condition(SOURCECODE code) {

	QUEUE token_stream = sc_get_ts(code);
	AST_EXPR_PTR expression_ptr = sc_get_ast_ex(code);
	AST_EXPR_PTR expression_tmp = NULL, expression_tmp_op = expression_ptr;
	char buf[2];


	/* condition -> ODD expression */
	if (getWordID(token_stream) == ODD) {
		MTNT(token_stream);
		sc_set_ast_ex(code, expr_init_odd(expression_ptr));
		expression(code);
	}

	else {
		/* condition -> expression > expression | expression < expression */

		expr_init_relation(expression_ptr);
		sc_set_ast_ex(code, expr_get_relation_left(expression_ptr));
		expression_tmp = expr_get_relation_right(expression_ptr);
		expression(code);
		sc_set_ast_ex(code, expression_tmp);

		if (getToken(token_stream) == '>'
				|| getToken(token_stream) == '<') {
			buf[0] = getToken(token_stream), buf[1] = '\0';
			expr_relation_set_op(expression_tmp_op, buf);
			MTNT(token_stream);
			expression(code);
		}

		else {
			switch (getWordID(token_stream)) {
				/* condition -> expression == expression */
				case (EQ):

					expr_relation_set_op(expression_tmp_op, getWord(token_stream));
					MTNT(token_stream);
					expression(code);
					break;

					/* condition -> expression != expression */
				case (NE):

					expr_relation_set_op(expression_tmp_op, getWord(token_stream));
					MTNT(token_stream);
					expression(code);
					break;

					/* condition -> expression <= expression */
				case (LE):

					expr_relation_set_op(expression_tmp_op, getWord(token_stream));
					MTNT(token_stream);
					expression(code);
					break;

					/* condition -> expression >= expression */
				case (GE):

					expr_relation_set_op(expression_tmp_op, getWord(token_stream));
					MTNT(token_stream);
					expression(code);
					break;

				default:
					PARSE_ERR(getLine(token_stream), SYN_NO_COMP);
			}
		}
	}
}

/**
 * @brief check expression syntax
 *
 * @param *code pointer to source code object
 * @retval void
 **/
void expression(SOURCECODE code) {

	QUEUE token_stream = sc_get_ts(code);
	AST_EXPR_PTR expression_ptr = sc_get_ast_ex(code);
	AST_EXPR_PTR expression_tmp = NULL, expression_tmp_op = expression_ptr;

	/* expression -> - term */
	if (getToken(token_stream) == '-') {
		expression_ptr = expr_init_unary(expression_ptr, getToken(token_stream));
		MTNT(token_stream);
	}

	/* expression -> term */
	expr_init_arithmetic(expression_ptr);
	sc_set_ast_ex(code, expr_get_arithmetic_left(expression_ptr));
	expression_tmp = expr_get_arithmetic_right(expression_ptr);
	term(code);
	sc_set_ast_ex(code, expression_tmp);

	/* expression -> expression + term | expression - term */
	while (getToken(token_stream) == '+' || getToken(token_stream) == '-') {
		expr_arithmetic_set_op(expression_tmp_op, getToken(token_stream));
		MTNT(token_stream);
		term(code);
	}
}

/**
 * @brief check term syntax
 *
 * @param *code pointer to source code object
 * @retval void
 **/
void term(SOURCECODE code) {

	QUEUE token_stream = sc_get_ts(code);
	AST_EXPR_PTR expression_ptr = sc_get_ast_ex(code);
	AST_EXPR_PTR expression_tmp = NULL, expression_tmp_op = expression_ptr;

	/* term -> factor */
	expr_init_arithmetic(expression_ptr);
	sc_set_ast_ex(code, expr_get_arithmetic_left(expression_ptr));
	expression_tmp = expr_get_arithmetic_right(expression_ptr);
	factor(code);
	sc_set_ast_ex(code, expression_tmp);

	/* term -> term * factor | term / factor */
	while (getToken(token_stream) == '*' || getToken(token_stream) == '/') {
		expr_arithmetic_set_op(expression_tmp_op, getToken(token_stream));
		MTNT(token_stream);
		term(code);
	}
}

/**
 * @brief check factor syntax
 *
 * @param *code pointer to source code object
 * @retval void
 **/
void factor(SOURCECODE code) {

	STACK symbol_table = sc_get_st(code);
	QUEUE token_stream = sc_get_ts(code);
	TEPTR table_entry = NULL;
	AST_EXPR_PTR expression_ptr = sc_get_ast_ex(code);

	/* factor -> identifier */
	if (getWordID(token_stream) == IDENTIFIER) {

		table_entry = stlookup(symbol_table, getWord(token_stream));

		if (table_entry == NULL)
			PARSE_ERR(getLine(token_stream), TYP_ID_NO_IN);
		else if (st_get_typeID(table_entry) == PROCEDURE)
			PARSE_ERR(getLine(token_stream), TYP_ONLY_INT);

		expr_init_identifier(expression_ptr, getWord(token_stream));
		MTNT(token_stream);
		/* factor -> number */
	}

	else if (getNumberID(token_stream) == NUM) {
		expr_init_number(expression_ptr, getNumber(token_stream));
		MTNT(token_stream);
		/* factor -> ( expression ) */
	}

	else if (getToken(token_stream) == '(') {
		MTNT(token_stream);
		expression(code);

		if (getToken(token_stream) == ')')
			MTNT(token_stream);
		else
			PARSE_ERR(getLine(token_stream), SYN_MISS_CB);
	}

	else
		PARSE_ERR(getLine(token_stream), SYN_MISS_OB);
}

