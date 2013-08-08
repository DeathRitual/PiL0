/*
 * MetaList
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
 * @file meta_list.h
 *
 *
 * @defgroup meta_list Meta List
 * @brief Usefull to generate Queues and Stacks
 * @ingroup parser lexer
 */

#include"err_handling.h"
#include<stdlib.h>

#pragma once

typedef struct META_LIST_ELEMENT *MLEPTR;
typedef struct META_LIST *MLPTR;

/**
 * @struct stack
 *
 * @brief composition object for stack
 */
struct stack {
	MLPTR stack_meta_list; /**< pointer to meta_list */
};

/**
 * @struct queue
 *
 * @brief composition object for stack
 */
struct queue {
	MLPTR queue_meta_list; /**< pointer to meta list */
};

typedef struct stack *STACK;
typedef struct queue *QUEUE;

/* stack functions */
extern STACK init_stack();
extern void push(STACK, void *);
extern void *gettmp(const STACK);
extern void *linst(const STACK, void *, void *(*getContent)(void *),
		int (*comp_func)(void *, void *));
extern void *top(const STACK);
extern void *pop(STACK);
extern void flush_stack(STACK);
extern void free_stack(STACK);
extern size_t size_stack(const STACK);
extern int empty_stack(const STACK);

/* queue functions */
extern QUEUE init_queue();
extern void append(QUEUE, void *);
extern void *head(const QUEUE);
extern void *tail(const QUEUE);
extern void *qudel(QUEUE);
extern void flush_queue(QUEUE);
extern void free_queue(QUEUE);
extern size_t size_queue(const QUEUE);
extern int empty_queue(const QUEUE);
