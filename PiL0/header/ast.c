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
 * @file ast.c Library which inherits all necessary functions for creating an abstract syntax tree
 *
 * @ingroup ast
 *
 */

#include"frontend.h"

#define __AST_BLOCK__ "AST Block"
#define __AST_STMT__ "AST Statement"
#define __AST_EXPR__ "AST Expression"

#ifndef PL_DEBUG

#define DEB_OUT(dummy1, dummy2, dummy3, dummy4)
#else

static void ast_debout(const char *desc, void *root, void *branch1, void *branch2) {
	printf("%10s->root:    %p\n", desc, root);
	if (branch1 != NULL) printf("%10s->branch1: %p\n", desc, branch1);
	if (branch2 != NULL) printf("%10s->branch2: %p\n", desc, branch2);
}

#define DEB_OUT(dummy1, dummy2, dummy3, dummy4) ast_debout(dummy1, dummy2, dummy3, dummy4)
#endif

enum block_ids {
	BLOCK_PROC, BLOCK_STMT
};

enum stmt_ids {
	STMT_IF, STMT_WHILE, STMT_ASSIGN, STMT_SEQ, STMT_CARE, STMT_PRINT
};

enum expr_ids {
	EXPR_NUMBER, EXPR_IDENTIFIER, EXPR_ARITH, EXPR_UNARY, EXPR_REL, EXPR_ODD
};

/**
 * @struct AST_BLOCK
 *
 * @brief block root
 **/
struct AST_BLOCK {
	enum expr_ids tag; /**< union identifier */
	union {
		struct {
			char identifier[MAX_LENGTH]; /* identifier */
			AST_BLOCK_PTR function_path;
			AST_BLOCK_PTR main_path;
		} procedure; /**< block */
		AST_STMT_PTR statement; /**< statement */
	} block;
};

/**
 * @struct _stmt
 *
 * @brief statement root
 **/
struct AST_STMT {
	enum stmt_ids tag; /**< union identifier */
	union {
		char identifier[MAX_LENGTH]; /**< identifier for CALL / READ */
		AST_EXPR_PTR expression; /**< PRINT expression */
		struct {
			AST_EXPR_PTR condition;
			AST_STMT_PTR statement;
		} jumpbac; /**< while loop */
		struct {
			AST_EXPR_PTR condition;
			AST_STMT_PTR statement;
		} jumpfor; /**< if condition */
		struct {
			char identifier[MAX_LENGTH];
			AST_EXPR_PTR expression;
		} assignment; /**< assignment */
		struct {
			AST_STMT_PTR left_statement;
			AST_STMT_PTR right_statement;
		} sequence; /**< sequence of statements */
	} statement;
};

/**
 * @struct _expr
 *
 * @brief expression root
 **/
struct AST_EXPR {
	enum expr_ids tag; /**< union identifier */
	union {
		int number; /**< number */
		char identifier[MAX_LENGTH]; /**< identifier */
		struct {
			char operator; /* +, -, *, / */
			AST_EXPR_PTR left_expression; /* expression */
			AST_EXPR_PTR right_expression; /* expression */
		} arithmetic; /**< arithmetic expression */
		struct {
			char operator[3]; /* <, >, !, ==, !=, <=, >= */
			AST_EXPR_PTR left_expression; /* expression */
			AST_EXPR_PTR right_expression; /* expression */
		} relation; /**< relational expression */
		struct {
			char operator; /* - */
			AST_EXPR_PTR expression; /* expression */
		} unary; /**< unary expression */
		AST_EXPR_PTR odd; /**< odd expression */
	} expression;
};

/**
 * @brief constructor for block root
 *
 * @retval rootBlock*
 **/
AST_BLOCK_PTR init_block() {
	AST_BLOCK_PTR new_knot;
	if ((new_knot = malloc(sizeof(*new_knot))) == NULL)
		error(__AST_BLOCK__, __FILE__, __func__, __LINE__, ERR_MEMORY);

	return new_knot;
}

/**
 * @brief constructor for statement root
 *
 * @retval rootStmt*
 **/
AST_STMT_PTR init_stmt() {
	AST_STMT_PTR new_knot;
	if ((new_knot = malloc(sizeof(*new_knot))) == NULL)
		error(__AST_STMT__, __FILE__, __func__, __LINE__, ERR_MEMORY);

	return new_knot;
}

/**
 * @brief constructor for expression root
 *
 * @retval rootExpr*
 **/
AST_EXPR_PTR init_expr() {
	AST_EXPR_PTR new_knot;
	if ((new_knot = malloc(sizeof(*new_knot))) == NULL)
		error(__AST_EXPR__, __FILE__, __func__, __LINE__, ERR_MEMORY);

	return new_knot;
}

void block_init_procedure(AST_BLOCK_PTR bl, const char *s) {
	bl->tag = BLOCK_PROC;
	strcpy(bl->block.procedure.identifier, s);
	bl->block.procedure.function_path = init_block();
	bl->block.procedure.main_path = init_block();
	DEB_OUT("procedure", bl, bl->block.procedure.function_path, bl->block.procedure.main_path);
}

AST_BLOCK_PTR block_get_function(const AST_BLOCK_PTR bl) {
	return bl->block.procedure.function_path;
}

AST_BLOCK_PTR block_get_main(const AST_BLOCK_PTR bl) {
	return bl->block.procedure.main_path;
}

AST_STMT_PTR block_init_statement(AST_BLOCK_PTR bl) {
	bl->tag = BLOCK_STMT;
	bl->block.statement = init_stmt();
	DEB_OUT("statement", bl, bl->block.statement, NULL);
	return bl->block.statement;
}


void stmt_init_care(AST_STMT_PTR st, const char *s) {
	st->tag = STMT_CARE;
	strcpy(st->statement.identifier, s);
	DEB_OUT("care", st, NULL, NULL);
}

AST_EXPR_PTR stmt_init_print(AST_STMT_PTR st) {
	st->tag = STMT_PRINT;
	st->statement.expression = init_expr();
	DEB_OUT("print", st, st->statement.expression, NULL);
	return st->statement.expression;
}

void stmt_init_jumpbac(AST_STMT_PTR st) {
	st->tag = STMT_WHILE;
	st->statement.jumpbac.condition = init_expr();
	st->statement.jumpbac.statement = init_stmt();
	DEB_OUT("jumpbac", st, st->statement.jumpbac.condition, st->statement.jumpbac.statement);
}

AST_EXPR_PTR stmt_get_jumpbac_condition(const AST_STMT_PTR st) {
	return st->statement.jumpbac.condition;
}

AST_STMT_PTR stmt_get_jumpbac_statement(const AST_STMT_PTR st) {
	return st->statement.jumpbac.statement;
}

void stmt_init_jumpfor(AST_STMT_PTR st) {
	st->tag = STMT_IF;
	st->statement.jumpfor.condition = init_expr();
	st->statement.jumpfor.statement = init_stmt();
	DEB_OUT("jumpfor", st, st->statement.jumpfor.condition, st->statement.jumpfor.statement);
}

AST_EXPR_PTR stmt_get_jumpfor_condition(const AST_STMT_PTR st) {
	return st->statement.jumpfor.condition;
}

AST_STMT_PTR stmt_get_jumpfor_statement(const AST_STMT_PTR st) {
	return st->statement.jumpfor.statement;
}

AST_EXPR_PTR stmt_init_assignment(AST_STMT_PTR st, const char *s) {
	st->tag = STMT_ASSIGN;
	strcpy(st->statement.assignment.identifier, s);
	st->statement.assignment.expression = init_expr();
	DEB_OUT("assignment", st, st->statement.assignment.expression, NULL);
	return st->statement.assignment.expression;
}

void stmt_init_sequence(AST_STMT_PTR st) {
	st->tag = STMT_SEQ;
	st->statement.sequence.left_statement = init_stmt();
	st->statement.sequence.right_statement = init_stmt();
	DEB_OUT("sequence", st, st->statement.sequence.left_statement, st->statement.sequence.right_statement);
}

AST_STMT_PTR stmt_get_sequence_left(const AST_STMT_PTR st) {
	return st->statement.sequence.left_statement;
}

AST_STMT_PTR stmt_get_sequence_right(const AST_STMT_PTR st) {
	return st->statement.sequence.right_statement;
}


void expr_init_number(AST_EXPR_PTR ex, const int n) {
	ex->tag = EXPR_NUMBER;
	ex->expression.number = n;
	DEB_OUT("number", ex, NULL, NULL);
}

void expr_init_identifier(AST_EXPR_PTR ex, const char *s) {
	ex->tag = EXPR_IDENTIFIER;
	strcpy(ex->expression.identifier, s);
	DEB_OUT("identifier", ex, NULL, NULL);
}

void expr_init_arithmetic(AST_EXPR_PTR ex) {
	ex->tag = EXPR_ARITH;
	ex->expression.arithmetic.left_expression = init_expr();
	ex->expression.arithmetic.right_expression = init_expr();
	DEB_OUT("arithmetic", ex, ex->expression.arithmetic.left_expression, ex->expression.arithmetic.right_expression);
}

void expr_arithmetic_set_op(AST_EXPR_PTR ex, const char c) {
	ex->expression.arithmetic.operator = c;
}

AST_EXPR_PTR expr_get_arithmetic_left(const AST_EXPR_PTR ex) {
	return ex->expression.arithmetic.left_expression;
}

AST_EXPR_PTR expr_get_arithmetic_right(const AST_EXPR_PTR ex) {
	return ex->expression.arithmetic.right_expression;
}

void expr_init_relation(AST_EXPR_PTR ex) {
	ex->tag = EXPR_REL;
	ex->expression.relation.left_expression = init_expr();
	ex->expression.relation.right_expression = init_expr();
	DEB_OUT("relation", ex, ex->expression.relation.left_expression, ex->expression.relation.right_expression);
}

void expr_relation_set_op(AST_EXPR_PTR ex, const char *s) {
	strcpy(ex->expression.relation.operator, s);
}

AST_EXPR_PTR expr_get_relation_left(const AST_EXPR_PTR ex) {
	return ex->expression.relation.left_expression;
}

AST_EXPR_PTR expr_get_relation_right(const AST_EXPR_PTR ex) {
	return ex->expression.relation.right_expression;
}

AST_EXPR_PTR expr_init_unary(AST_EXPR_PTR ex, const char c) {
	ex->tag = EXPR_UNARY;
	ex->expression.unary.operator = c;
	ex->expression.unary.expression = init_expr();
	DEB_OUT("unary", ex, ex->expression.unary.expression, NULL);
	return ex->expression.unary.expression;
}

AST_EXPR_PTR expr_init_odd(AST_EXPR_PTR ex) {
	ex->tag = EXPR_ODD;
	ex->expression.odd = init_expr();
	DEB_OUT("odd", ex, ex->expression.odd, NULL);
	return ex->expression.odd;
}
