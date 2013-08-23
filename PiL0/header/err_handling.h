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
 * @file err_handling.h Header-File for error handling library
 *
 * Forwards the functions of the error handling library to another file which includes this header file.
 *
 * @defgroup error_handling Error Handler
 * @brief defines error and debug messages for all modules
 *
 * @ingroup meta_list lexer parser global ast error_handling
 */

#ifndef __ERR_HANDLING_H
#define __ERR_HANDLING_H

#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif

#define ERROR_EXCEPT(module, error_code) error(module, __FILE__, __func__, __LINE__, error_code)

/**
 * @enum err_codes short strings used as variables for error messages
 */
enum err_codes {
	NULL_POINTER, ERR_MEMORY, EMPTY_LIST, EMPTY_TABLE, WRONG_ID, NO_EMPTY_LIST, HASH_FULL
};

enum parse_err_codes {
	SYN_END,
	SYN_MISS_ASS,
	SYN_NO_ID,
	SYN_MISS_SEP,
	SYN_MISS_COM,
	SYN_MISS_END,
	SYN_IF,
	SYN_WHILE,
	SYN_STMT,
	SYN_NO_COMP,
	SYN_MISS_CB,
	SYN_MISS_OB,
	TYP_CONST_NUM,
	TYP_ID_NO_IN,
	TYP_NO_ID,
	TYP_ONLY_PROC,
	TYP_ONLY_INT,
	TYP_DOUB_DEC
};

extern void error(const char *, const char *, const char *, int, enum err_codes);
extern void parseError(int, enum parse_err_codes);
extern void debug_output(int, enum parse_err_codes, const char *, int);

#endif
