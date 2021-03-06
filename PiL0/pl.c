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
 * @file pl.c Main source file for PiL0-Compiler
 *
 * @ingroup global
 */

#include"global.h"
#include<stdlib.h>

int main(int argc, char *argv[]) {

	FILE *raw_code;
	int status;

	raw_code = fopen("../source_code.pl0", "r");

	if (raw_code != NULL)
		status = compile(raw_code);
	else {
		puts("Couldn't open Source Code!");
		return EXIT_FAILURE;
	}

	if (status)
		puts("Successful!");
	else
		puts("Ugh...there went something wrong!");

	return EXIT_SUCCESS;
}

