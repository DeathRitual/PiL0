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
 * @defgroup ast Abstract-Syntax-Tree
 * @brief creates AST structure during parsing for evaluating of syntax.
 * @ingroup parser ast
 */

#include"frontend.h"

#define AST_NAME_BLOCK "AST Block"
#define AST_NAME_STMT "AST Statement"
#define AST_NAME_EXPR "AST Expression"

/**
 * @brief error exception for null pointer in block (bl).
 */
#define AST_NULL_POINTER_BLOCK 	if (bl == NULL) ERROR_EXCEPT(AST_NAME_BLOCK, NULL_POINTER)
/**
 * @brief error exception for null pointer in statement (st).
 */
#define AST_NULL_POINTER_STMT 	if (st == NULL) ERROR_EXCEPT(AST_NAME_STMT, NULL_POINTER)
/**
 * @brief error exception for null pointer in expression (ex).
 */
#define AST_NULL_POINTER_EXPR 	if (ex == NULL) ERROR_EXCEPT(AST_NAME_EXPR, NULL_POINTER)

#ifdef PL_DEBUG

/**
 * @brief debug-only: print to standard output AST addresses
 *
 *
 * @param *desc description for which element in AST is being created
 * @param *root root element of AST knot
 * @param *branch1 first branch of AST knot
 * @param *branch2 secons branch of AST knot
 * @retval void
 **/
static void ast_debout(const char *desc, void *root, void *branch1,
		void *branch2) {
	printf("%10s->root:    %p\n", desc, root);
	if (branch1 != NULL)
		printf("%10s->branch1: %p\n", desc, branch1);
	if (branch2 != NULL)
		printf("%10s->branch2: %p\n", desc, branch2);
}

#define DEB_OUT(dummy1, dummy2, dummy3, dummy4) ast_debout(dummy1, dummy2, dummy3, dummy4)
#endif

/**
 * @enum block_ids IDs to differ between block knot elements
 *
 * For internal use only!
 */
enum block_ids {
	BLOCK_PROC, BLOCK_STMT
};

/**
 * @enum stmt_ids IDs to differ between statement knot elements
 *
 * For internal use only!
 */
enum stmt_ids {
	STMT_IF, STMT_WHILE, STMT_ASSIGN, STMT_SEQ, STMT_CARE, STMT_PRINT
};

/**
 * @enum expr_ids IDs to differ between expression knot elements
 *
 * For internal use only!
 */
enum expr_ids {
	EXPR_NUMBER, EXPR_IDENTIFIER, EXPR_ARITH, EXPR_UNARY, EXPR_REL, EXPR_ODD
};

/**
 * @struct AST_BLOCK
 *
 * @brief Block element which can be either refer to a procedure or a statement.
 **/
struct AST_BLOCK {
	enum expr_ids tag; /**< union identifier */
	/**
	 * @union un_block
	 *
	 * @brief Can either represent a procedure or a statement
	 */
	union un_block {
		/**
		 * @struct st_proc
		 *
		 * @brief Represent procedure and stores the following:
		 *
		 * Name of the procedure and two branches pointing to the block within the procedure and the following.
		 */
		struct st_proc {
			char identifier[MAX_LENGTH]; 	/**< procedure name */
			AST_BLOCK_PTR function_path;	/**< pointer to block within procedure */
			AST_BLOCK_PTR main_path;		/**< pointer to block following procedure */
		} procedure;
		AST_STMT_PTR statement; /**< statement */
	} block;
};

/**
 * @struct AST_STMT
 *
 * @brief Statement element which represents different statement types
 **/
struct AST_STMT {
	enum stmt_ids tag; /**< union identifier */
	/**
	 * @union un_statement
	 *
	 * @brief Can store one of the following types:
	 *
	 * Identifier for CALL and READ instructions,
	 * branch to expression outlined by PRINT,
	 * branches for IF/WHILE instructions,
	 * identifier and branch for assignment or
	 * a sequence for two or more statements.
	 */
	union un_statement {
		char identifier[MAX_LENGTH];		/**< identifier for CALL / READ */
		AST_EXPR_PTR expression; 			/**< branch to expression for PRINT */
		/**
		 * @struct st_jumpbac
		 *
		 * @brief Represent WHILE (condition) THEN (statement) instruction.
		 */
		struct st_jumpbac {
			AST_EXPR_PTR condition;			/**< branch to condition */
			AST_STMT_PTR statement;			/**< branch to statement */
		} jumpbac;
		/**
		 * @struct st_jumpfor
		 *
		 * @brief Represent IF (condition) THEN (statement) instruction.
		 */
		struct st_jumpfor {
			AST_EXPR_PTR condition;			/**< branch to condition */
			AST_STMT_PTR statement;			/**< branch to statement */
		} jumpfor;
		/**
		 * @struct st_assignment
		 *
		 * @brief Represent assignment ('=' operator) of valued expression to identifier.
		 */
		struct st_assignment {
			char identifier[MAX_LENGTH];	/**< identifier value stored to*/
			AST_EXPR_PTR expression;		/**< branch to expression for evaluating */
		} assignment;
		/**
		 * @struct st_sequence
		 *
		 * @brief Used for adding additional statements to AST.
		 */
		struct st_sequence {
			AST_STMT_PTR left_statement;	/**< current statement */
			AST_STMT_PTR right_statement;	/**< next statement */
		} sequence;
	} statement;
};

/**
 * @struct AST_EXPR
 *
 * @brief Expression element which represents different expression types.
 **/
struct AST_EXPR {
	enum expr_ids tag; /**< union identifier */
	/**
	 * @union un_expression
	 *
	 * @brief Can store one of the following:
	 *
	 * Number or identifier,
	 * arithmetic operation,
	 * relational expression,
	 * unary expression or
	 * odd (check if defined) expression.
	 */
	union un_expression {
		int number; 						/**< number */
		char identifier[MAX_LENGTH]; 		/**< identifier */
		/**
		 * @struct st_arithmetic
		 *
		 * @brief Set operator for arithmetic operation and set branches for expressions left and right of operator.
		 */
		struct st_arithmetic {
			char operator; 					/**< arithmetic operator */
			AST_EXPR_PTR left_expression;	/**< left side of operator */
			AST_EXPR_PTR right_expression;	/**< right side of operator */
		} arithmetic;
		/**
		 * @struct st_relation
		 *
		 * @brief Set operator for logical operation and set branches for expressions left and right of operator.
		 */
		struct st_relation {
			char operator[3]; 				/**< logical operator */
			AST_EXPR_PTR left_expression;	/**< left side of operator */
			AST_EXPR_PTR right_expression;	/**< right side of operator */
		} relation;
		/**
		 * @struct st_unary
		 *
		 * @brief Set unary operator and branch for its expression.
		 */
		struct st_unary {
			char operator;					/**< unary operator */
			AST_EXPR_PTR expression;		/**< branch for expression */
		} unary;
		AST_EXPR_PTR odd; 					/**< odd expression */
	} expression;
};

/**
 * @brief allocates memory for AST block knot
 *
 * @retval new_knot AST block with allocated memory
 **/
AST_BLOCK_PTR init_block() {
	AST_BLOCK_PTR new_knot;
	if ((new_knot = malloc(sizeof(*new_knot))) == NULL)
		ERROR_EXCEPT(AST_NAME_BLOCK, ERR_MEMORY);

	return new_knot;
}

/**
 * @brief allocates memory for AST statement knot
 *
 * @retval new knot AST statement with allocated memory
 **/
AST_STMT_PTR init_stmt() {
	AST_STMT_PTR new_knot;
	if ((new_knot = malloc(sizeof(*new_knot))) == NULL)
		ERROR_EXCEPT(AST_NAME_STMT, ERR_MEMORY);

	return new_knot;
}

/**
 * @brief allocates memory for AST expression knot
 *
 * @retval new_knot AST expression with allocated memory
 **/
AST_EXPR_PTR init_expr() {
	AST_EXPR_PTR new_knot;
	if ((new_knot = malloc(sizeof(*new_knot))) == NULL)
		ERROR_EXCEPT(AST_NAME_EXPR, ERR_MEMORY);

	return new_knot;
}

/**
 * @brief transforms block element to procedure knot:
 *
 * Sets procedure identifier and generates two branches, one for the block within the procedure and the other one for the following block.
 *
 * @param bl pointer to block
 * @param *s procedure name
 * @retval void
 **/
void block_init_procedure(AST_BLOCK_PTR bl, const char *s) {
	AST_NULL_POINTER_BLOCK;
	bl->tag = BLOCK_PROC;
	strcpy(bl->block.procedure.identifier, s);
	bl->block.procedure.function_path = init_block();
	bl->block.procedure.main_path = init_block();
#ifdef PL_DEBUG
	DEB_OUT("procedure", bl, bl->block.procedure.function_path,
			bl->block.procedure.main_path);
#endif
}

/**
 * @brief returns pointer to block within procedure
 *
 * @param bl knot
 * @retval bl->block.procedure.function_path pointer branch refers to
 */
AST_BLOCK_PTR block_get_function(const AST_BLOCK_PTR bl) {
	AST_NULL_POINTER_BLOCK;
	return bl->block.procedure.function_path;
}

/**
 * @brief returns pointer to block after procedure
 *
 * @param bl knot
 * @retval bl->block.procedure.main_path pointer branch refers to
 */
AST_BLOCK_PTR block_get_main(const AST_BLOCK_PTR bl) {
	AST_NULL_POINTER_BLOCK;
	return bl->block.procedure.main_path;
}

/**
 * @brief transforms block element to statement knot
 *
 * Sets statement identifier and generates statement branch.
 *
 * @param bl block element
 * @retval bl->block.statement pointer branch refers to
 */
AST_STMT_PTR block_init_statement(AST_BLOCK_PTR bl) {
	AST_NULL_POINTER_BLOCK;
	bl->tag = BLOCK_STMT;
#ifdef PL_DEBUG
	bl->block.statement = init_stmt();
	DEB_OUT("statement", bl, bl->block.statement, NULL);
	return bl->block.statement;
#else
	return bl->block.statement = init_stmt();
#endif
}

/**
 * @brief transforms statement element to identifier
 *
 * Attention! This function is used for creating knots for operations like CALL and READ!
 * Sets CALL/READ identifier and stores identifier name.
 *
 * @param st statement element
 * @param *s identifier name
 * @retval void
 */
void stmt_init_care(AST_STMT_PTR st, const char *s) {
	AST_NULL_POINTER_STMT;
	st->tag = STMT_CARE;
	strcpy(st->statement.identifier, s);
#ifdef PL_DEBUG
	DEB_OUT("care", st, NULL, NULL);
#endif
}

/**
 * @brief transforms statement element to print knot and return expression branch
 *
 * Sets PRINT identifier and creates expression branch
 *
 * @param st statement element
 * @retval st->statement.expression pointer to branch refers to
 */
AST_EXPR_PTR stmt_init_print(AST_STMT_PTR st) {
	AST_NULL_POINTER_STMT;
	st->tag = STMT_PRINT;
#ifdef PL_DEBUG
	st->statement.expression = init_expr();
	DEB_OUT("print", st, st->statement.expression, NULL);
	return st->statement.expression;
#else
	return st->statement.expression = init_expr();
#endif
}

/**
 * @brief transforms statement element to whole knot
 *
 * Naming comes from three address code because while implements a jump back to a defined point.
 *
 * @param st statement element
 * @retval void
 */
void stmt_init_jumpbac(AST_STMT_PTR st) {
	AST_NULL_POINTER_STMT;
	st->tag = STMT_WHILE;
	st->statement.jumpbac.condition = init_expr();
	st->statement.jumpbac.statement = init_stmt();
#ifdef PL_DEBUG
	DEB_OUT("jumpbac", st, st->statement.jumpbac.condition,
			st->statement.jumpbac.statement);
#endif
}

/**
 * @brief returns branch to condition element of while instruction
 *
 * @param st statement element
 * @retval st->statement.jumpbac.condition condition branch
 */
AST_EXPR_PTR stmt_get_jumpbac_condition(const AST_STMT_PTR st) {
	AST_NULL_POINTER_STMT;
	return st->statement.jumpbac.condition;
}

/**
 * @brief returns branch to statement element of while instruction
 *
 * @param st statement element
 * @return st->statement.jumpbac.statement statement branch
 */
AST_STMT_PTR stmt_get_jumpbac_statement(const AST_STMT_PTR st) {
	AST_NULL_POINTER_STMT;
	return st->statement.jumpbac.statement;
}

/**
 * @brief transforms statement element to if knot
 *
 * Naming comes from three address code where if instruction is represented by a jump forwards.
 *
 * @param st statement element
 * @retval void
 */
void stmt_init_jumpfor(AST_STMT_PTR st) {
	AST_NULL_POINTER_STMT;
	st->tag = STMT_IF;
	st->statement.jumpfor.condition = init_expr();
	st->statement.jumpfor.statement = init_stmt();
#ifdef PL_DEBUG
	DEB_OUT("jumpfor", st, st->statement.jumpfor.condition,
			st->statement.jumpfor.statement);
#endif
}

/**
 * @brief returns branch to condition element of if instruction
 *
 * @param st statement element
 * @retval st->statement.jumpfor.condition condition branch
 */
AST_EXPR_PTR stmt_get_jumpfor_condition(const AST_STMT_PTR st) {
	AST_NULL_POINTER_STMT;
	return st->statement.jumpfor.condition;
}

/**
 * @brief returns branch to statement element of while instruction
 *
 * @param st statement element
 * @return st->statement.jumpfor.statement statement branch
 */
AST_STMT_PTR stmt_get_jumpfor_statement(const AST_STMT_PTR st) {
	AST_NULL_POINTER_STMT;
	return st->statement.jumpfor.statement;
}

/**
 * @brief transforms statement element to assignment element
 *
 * Stores identifier and branch to expression which value should be stored in identifier.
 *
 * @param st statement element
 * @param *s name of identifier
 * @retval st->statement.assignment.expression branch to expression
 */
AST_EXPR_PTR stmt_init_assignment(AST_STMT_PTR st, const char *s) {
	AST_NULL_POINTER_STMT;
	st->tag = STMT_ASSIGN;
	strcpy(st->statement.assignment.identifier, s);
#ifdef PL_DEBUG
	st->statement.assignment.expression = init_expr();
	DEB_OUT("assignment", st, st->statement.assignment.expression, NULL);
	return st->statement.assignment.expression;
#else
	return st->statement.assignment.expression = init_expr();
#endif
}

/**
 * @brief transforms statement element to sequence for creating multiple statements
 *
 * @param st statement element
 * @retval void
 */
void stmt_init_sequence(AST_STMT_PTR st) {
	AST_NULL_POINTER_STMT;
	st->tag = STMT_SEQ;
	st->statement.sequence.left_statement = init_stmt();
	st->statement.sequence.right_statement = init_stmt();
#ifdef PL_DEBUG
	DEB_OUT("sequence", st, st->statement.sequence.left_statement,
			st->statement.sequence.right_statement);
#endif
}

/**
 * @brief returns branch for first statement
 *
 * @param st statement element
 * @retval st->statement.sequence.left_statement first statement branch
 */
AST_STMT_PTR stmt_get_sequence_left(const AST_STMT_PTR st) {
	AST_NULL_POINTER_STMT;
	return st->statement.sequence.left_statement;
}

/**
 * @brief returns branch for second statement
 *
 * @param st statement element
 * @retval st->statement.sequence.right_statement second statement branch
 */
AST_STMT_PTR stmt_get_sequence_right(const AST_STMT_PTR st) {
	AST_NULL_POINTER_STMT;
	return st->statement.sequence.right_statement;
}


/**
 * @brief transform expression element to number
 *
 * @param ex expression element
 * @param n number to store
 * @retval void
 */
void expr_init_number(AST_EXPR_PTR ex, const int n) {
	AST_NULL_POINTER_EXPR;
	ex->tag = EXPR_NUMBER;
	ex->expression.number = n;
#ifdef PL_DEBUG
	DEB_OUT("number", ex, NULL, NULL);
#endif
}

/**
 * @brief transform expression element to identifier
 *
 * @param ex expression element
 * @param s name of identifier
 * @retval void
 */
void expr_init_identifier(AST_EXPR_PTR ex, const char *s) {
	AST_NULL_POINTER_EXPR;
	ex->tag = EXPR_IDENTIFIER;
	strcpy(ex->expression.identifier, s);
#ifdef PL_DEBUG
	DEB_OUT("identifier", ex, NULL, NULL);
#endif
}

/**
 * @brief transform expression element to arithmetic operation by generating to branches for the left and right side of the arithmetic operator
 *
 * @param ex expression element
 * @retval void
 */
void expr_init_arithmetic(AST_EXPR_PTR ex) {
	AST_NULL_POINTER_EXPR;
	ex->tag = EXPR_ARITH;
	ex->expression.arithmetic.left_expression = init_expr();
	ex->expression.arithmetic.right_expression = init_expr();
#ifdef PL_DEBUG
	DEB_OUT("arithmetic", ex, ex->expression.arithmetic.left_expression,
			ex->expression.arithmetic.right_expression);
#endif
}

/**
 * @brief stores the operator to arithmetic expression object
 *
 * Arithmetic object generation and operator storing are separated due to infix parsing
 *
 * @param ex arithmetic expression object
 * @param c operator
 * @retval void
 */
void expr_arithmetic_set_op(AST_EXPR_PTR ex, const char c) {
	AST_NULL_POINTER_EXPR;
	ex->expression.arithmetic.operator = c;
}

/**
 * @brief return left branch of arithmetic expression object
 *
 * @param ex arithmetic expression object
 * @retval ex->expression.arithmetic.left_expression left branch
 */
AST_EXPR_PTR expr_get_arithmetic_left(const AST_EXPR_PTR ex) {
	AST_NULL_POINTER_EXPR;
	return ex->expression.arithmetic.left_expression;
}

/**
 * @brief return right branch of arithmetic expression object
 *
 * @param ex arithmetic expression object
 * @retval ex->expression.arithmetic.right_expression right branch
 */
AST_EXPR_PTR expr_get_arithmetic_right(const AST_EXPR_PTR ex) {
	AST_NULL_POINTER_EXPR;
	return ex->expression.arithmetic.right_expression;
}

/**
 * @brief transform expression element to logical operation by generating to branches for the left and right side of the logical operator
 *
 * @param ex expression element
 * @retval void
 */
void expr_init_relation(AST_EXPR_PTR ex) {
	AST_NULL_POINTER_EXPR;
	ex->tag = EXPR_REL;
	ex->expression.relation.left_expression = init_expr();
	ex->expression.relation.right_expression = init_expr();
#ifdef PL_DEBUG
	DEB_OUT("relation", ex, ex->expression.relation.left_expression,
			ex->expression.relation.right_expression);
#endif
}

/**
 * @brief stores the operator to logical expression object
 *
 * Logical object generation and operator storing are separated due to infix parsing
 *
 * @param ex logical expression object
 * @param c operator
 * @retval void
 */
void expr_relation_set_op(AST_EXPR_PTR ex, const char *s) {
	AST_NULL_POINTER_EXPR;
	strcpy(ex->expression.relation.operator, s);
}

/**
 * @brief return right branch of logical expression object
 *
 * @param ex logical expression object
 * @retval ex->expression.relation.left_expression left branch
 */
AST_EXPR_PTR expr_get_relation_left(const AST_EXPR_PTR ex) {
	AST_NULL_POINTER_EXPR;
	return ex->expression.relation.left_expression;
}

/**
 * @brief return right branch of logical expression object
 *
 * @param ex logical expression object
 * @retval ex->expression.relation.left_expression right branch
 */
AST_EXPR_PTR expr_get_relation_right(const AST_EXPR_PTR ex) {
	AST_NULL_POINTER_EXPR;
	return ex->expression.relation.right_expression;
}

/**
 * @brief transform expression element to unary expression
 *
 * @param ex expression element
 * @param c unary operator
 * @retval ex->expression.unary.expression expression branch
 */
AST_EXPR_PTR expr_init_unary(AST_EXPR_PTR ex, const char c) {
	AST_NULL_POINTER_EXPR;
	ex->tag = EXPR_UNARY;
	ex->expression.unary.operator = c;
#ifdef PL_DEBUG
	ex->expression.unary.expression = init_expr();
	DEB_OUT("unary", ex, ex->expression.unary.expression, NULL);
	return ex->expression.unary.expression;
#else
	return ex->expression.unary.expression = init_expr();
#endif
}

/**
 * @brief transform expression element to odd expression
 *
 * @param ex expression element
 * @retval ex->expression.odd expression branch
 */
AST_EXPR_PTR expr_init_odd(AST_EXPR_PTR ex) {
	AST_NULL_POINTER_EXPR;
	ex->tag = EXPR_ODD;
#ifdef PL_DEBUG
	ex->expression.odd = init_expr();
	DEB_OUT("odd", ex, ex->expression.odd, NULL);
	return ex->expression.odd;
#else
	return ex->expression.odd = init_expr();
#endif
}
