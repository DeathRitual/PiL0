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
 * @file err_handling.c
 *
 * Error handling for PiL0-Compiler
 *
 * @ingroup error_handling
 */

#include "err_handling.h"
#include<stdio.h>
#include<stdlib.h>

/**
 * @var char *err_msg[]
 * @brief Stringtable which stores all error messages
 **/
static const char *err_msg[] = { "Null-Pointer", "No memory left", "Empty list",
		"Table already empty", "Wrong ID", "List not empty" };

/**
 * @var char *err_msg[]
 * @brief Stringtable which stores all error for parser messages
 **/
static const char *parse_err_msg[] =
		{ "Syntax-Error: Program must end with '.'",
				"Syntax-Error: Missing assign operator '='",
				"Syntax-Error: Missing identifier after CONST, VAR or PROCEDURE declaration",
				"Syntax-Error: Missing block seperator ';'",
				"Syntax-Error: Missing ',' seperator or ';' deliminator",
				"Syntax-Error: Missing END or statement seperator ';'",
				"Syntax-Error: After IF condition THEN statement must follow",
				"Syntax-Error: After WHILE condition DO statement must follow",
				"Syntax-Error: Wrong syntax in statement",
				"Syntax-Error: No compare operator",
				"Syntax-Error: Missing ')'", "Syntax-Error: Missing '('",
				"Type-Error: Can only assign number to constant",
				"Type-Error: Identifier not initialized",
				"Type-Error: No identifier given",
				"Type-Error: Can only call a procedure",
				"Type-Error: Operation only for Integer type",
				"Type-Error: Double declaration of identifier" };

/**
 * @brief Print error message.
 * @param *module module in which error raised
 * @param *file name of file
 * @param *function function which raised error
 * @param line line number
 * @param msg_nr short string of the error message to print
 * @retval void
 **/
void error(const char *module, const char *file, const char *function, int line,
		enum err_codes msg_nr) {
	fprintf(stderr, "ERROR(%s):\n\t%s:%d -> %s\n\t\t%s!\n", module, file, line,
			function, err_msg[msg_nr]);
	exit(10);
}

/**
 * @brief Print parsing error messages during compile
 *
 * @param ln line number
 * @param parse_err_nr enum to print error message
 * @retval void
 */
void parseError(int ln, enum parse_err_codes parse_err_nr) {
	fprintf(stderr, "%s in line %d!\n", parse_err_msg[parse_err_nr], ln);
	exit(10);
}

/**
 * @brief internal debuging prints also in which module error occures
 *
 * @param ln line number
 * @param parse_err_nr enum to print error message
 * @param *function function which raised error
 * @param line line number
 * @retval void
 */
void debug_output(int ln, enum parse_err_codes parse_err_nr,
		const char *function, int line) {
	fprintf(stderr, "\n\nDEBUG: %s in line %d!\n\tFunction: %s Line: %d\n\n",
			parse_err_msg[parse_err_nr], ln, function, line);
	exit(10);
}
