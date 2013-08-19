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
 * @file meta_data_types.c
 *
 * @ingroup meta_data
 */

#include"meta_data_types.h"

/**
 * @brief module error codes
 **/
enum err_modules {
	ML_ERR, ST_ERR, QU_ERR, HA_ERR
};

/**
 * @brief module error messages
 **/
static const char *mod[] = { "meta-list", "stack", "queue" , "hash"};

/**
 * @struct META_LIST_ELEMENT
 *
 * @brief meta_list element
 **/
struct META_LIST_ELEMENT {
	void *content; /**< pointer to any element */

	struct META_LIST_ELEMENT *next; /**< pointer to next element */
	struct META_LIST_ELEMENT *previous; /**< pointer to previous element */
};

/**
 * @struct META_LIST
 *
 * @brief root for double linked list
 *
 **/
struct META_LIST {
	struct META_LIST_ELEMENT *first; /** < first element in list */
	struct META_LIST_ELEMENT *last; /** < last element in list */
	struct META_LIST_ELEMENT *tmp; /** < temporary element in list */
	size_t count; /** < count number of elements in list */
};

/**
 * @brief constructor for new meta list
 *
 * @retval MLPTR*
 **/
static MLPTR init_meta_list() {
	MLPTR new_list = NULL;

	if ((new_list = malloc(sizeof(*new_list))) == NULL)
		error(mod[ML_ERR],
		__FILE__, __func__, __LINE__, ERR_MEMORY);

	new_list->first = NULL;
	new_list->last = NULL;
	new_list->tmp = NULL;
	new_list->count = 0;
	return new_list;
}

/**
 * @brief check if element is empty or not
 *
 * @param *mle pointer to meta list element
 * @retval int mle == NULL: 1; mle != NULL: 0
 **/
static int mlIsEmpty(const MLEPTR mle) {
	return mle == NULL;
}

/**
 * @brief last-in implementation
 *
 * @param *ml pointer to current meta list
 * @param *content pointer to content element
 * @retval void
 **/
static void mlal(MLPTR ml, void *content) {
	MLEPTR new_element = NULL, old_element = ml->last;

	if (ml == NULL)
		error(mod[ML_ERR], __FILE__, __func__, __LINE__, NULL_POINTER);

	if ((new_element = malloc(sizeof(*new_element))) == NULL)
		error(mod[ML_ERR],
		__FILE__, __func__, __LINE__, ERR_MEMORY);

	new_element->content = content;
	new_element->next = old_element;
	new_element->previous = NULL;

	if (!mlIsEmpty(old_element))
		old_element->previous = new_element;

	else
		ml->first = new_element;

	ml->last = new_element;
	ml->count++;
}

/**
 * @brief first-in implementation
 *
 * @param *ml pointer to current meta list
 * @param *content pointer to content element
 * @retval void
 **/
static void mlaf(MLPTR ml, void *content) {
	MLEPTR new_element = NULL, old_element = ml->first;

	if (ml == NULL)
		error(mod[ML_ERR], __FILE__, __func__, __LINE__, NULL_POINTER);

	if ((new_element = malloc(sizeof(*new_element))) == NULL)
		error(mod[ML_ERR],
		__FILE__, __func__, __LINE__, ERR_MEMORY);

	new_element->content = content;
	new_element->next = NULL;
	new_element->previous = old_element;

	if (!mlIsEmpty(old_element))
		old_element->next = new_element;

	else
		ml->last = new_element;

	ml->first = new_element;
	ml->count++;
}

/**
 * @brief remove oldest element in the meta list
 *
 * @param *ml pointer on meta list
 * @retval void
 **/
static void mldel(MLPTR ml) {
	if (ml == NULL)
		error(mod[ML_ERR], __FILE__, __func__, __LINE__, NULL_POINTER);

	if (mlIsEmpty(ml->last) || mlIsEmpty(ml->first))
		error(mod[ML_ERR],
		__FILE__, __func__, __LINE__, EMPTY_LIST);

	if (ml->first == ml->last) {
		ml->last->content = NULL;
		free(ml->last);
		ml->last = NULL;
		ml->first = NULL;
	}

	else {
		MLEPTR tmp = ml->first->previous;
		tmp->next = NULL;
		ml->first->previous = NULL;
		free(ml->first);
		ml->first = tmp;
	}

	ml->count--;
}

/**
 * @brief remove all elements from the meta list
 *
 * @param *ml pointer on meta list
 * @retval void
 **/
static void mlflush(MLPTR ml) {
	if (ml == NULL)
		error(mod[ML_ERR], __FILE__, __func__, __LINE__, NULL_POINTER);

	if (mlIsEmpty(ml->last) || mlIsEmpty(ml->first))
		error(mod[ML_ERR],
		__FILE__, __func__, __LINE__, EMPTY_LIST);

	do
		mldel(ml);

	while (!mlIsEmpty(ml->last) && !mlIsEmpty(ml->first));
}

/**
 * @brief delete list
 *
 * @param *ml pointer on meta list
 * @retval void
 **/
static void mlfree(MLPTR ml) {
	if (ml == NULL)
		error(mod[ML_ERR], __FILE__, __func__, __LINE__, NULL_POINTER);

	if (!mlIsEmpty(ml->last) && !mlIsEmpty(ml->first))
		error(mod[ML_ERR],
		__FILE__, __func__, __LINE__, NO_EMPTY_LIST);

	free(ml);
	ml = NULL;
}

/**
 * @brief get the first element from the meta list
 *
 * @param *ml pointer on meta list
 * @retval ml->first->content pointer to first element
 **/
static void *mlfirst(const MLPTR ml) {
	if (ml == NULL)
		error(mod[ML_ERR], __FILE__, __func__, __LINE__, NULL_POINTER);

	if (mlIsEmpty(ml->last) || mlIsEmpty(ml->first))
		error(mod[ML_ERR],
		__FILE__, __func__, __LINE__, EMPTY_LIST);

	return ml->first->content;
}

/**
 * @brief get the last element from the meta list
 *
 * @param *ml pointer on meta list
 * @retval ml->first->content pointer to last element
 **/
static void *mllast(const MLPTR ml) {
	if (ml == NULL)
		error(mod[ML_ERR], __FILE__, __func__, __LINE__, NULL_POINTER);

	if (mlIsEmpty(ml->last) || mlIsEmpty(ml->first))
		error(mod[ML_ERR],
		__FILE__, __func__, __LINE__, EMPTY_LIST);

	return ml->last->content;
}

/**
 * @brief get the tempory element from meta list
 *
 * @param *ml pointer on meta list
 * @retval ml->tmp->content pointer to temporary element
 */
static void *mltmp(const MLPTR ml) {
	if (ml == NULL)
		error(mod[ML_ERR], __FILE__, __func__, __LINE__, NULL_POINTER);

	if (mlIsEmpty(ml->last) || mlIsEmpty(ml->first))
		error(mod[ML_ERR],
		__FILE__, __func__, __LINE__, EMPTY_LIST);

	return ml->tmp->content;
}

/**
 * @brief get number of elements from the meta list
 *
 * @param *ml pointer on meta list
 * @retval ml->count number of elements in list
 **/
static int mlcount(const MLPTR ml) {
	if (ml == NULL)
		error(mod[ML_ERR], __FILE__, __func__, __LINE__, NULL_POINTER);

	return ml->count;
}

/**
 * @brief check if meta list is empty
 *
 * @param *ml pointer on meta list
 * @retval int ml->count == 0: 1; ml->count != 0: 0
 **/
static int mlempty(const MLPTR ml) {
	if (ml == NULL)
		error(mod[ML_ERR], __FILE__, __func__, __LINE__, NULL_POINTER);

	return ml->count == 0;
}

/**
 * @brief search for element in meta list
 *
 * @param *ml pointer to meta list
 * @param *compWith element which contains data looking for
 * @param *getContent function to cast content to correct type
 * @param *comp_func compare function
 * @retval int 1 not found; 0 found
 */
static void *mllookup(const MLPTR ml, void *compWith,
		void * (*getContent)(void *), int (*comp_func)(void *, void *)) {

	ml->tmp = ml->first;

	while (ml->tmp != NULL
			&& (*comp_func)((*getContent)(ml->tmp->content), compWith))
		ml->tmp = ml->tmp->previous;

	return ml->tmp;
}

/**
 * @brief initialize stack
 *
 * @retval STACK new stack
 **/
STACK init_stack() {
	STACK stack;

	if ((stack = malloc(sizeof(*stack))) == NULL)
		error(mod[ST_ERR], __FILE__, __func__, __LINE__, ERR_MEMORY);

	stack->stack_meta_list = init_meta_list();
	return stack;
}

/**
 * @brief return temporary element
 *
 * @param *st stack
 * @retval void* pointer to element
 */
void *gettmp(const STACK st) {
	return mltmp(st->stack_meta_list);
}

/**
 * @brief look for element in stack
 *
 * @param *st stack
 * @param *content element which contains data looking for
 * @param *getContent function to cast content to correct type
 * @param *comp_func compare function
 * @retval void* if element found return element else return NULL
 */
void *linst(const STACK st, void *content, void * (*getContent)(void *),
		int (*comp_func)(void *, void *)) {
	void *element;
	return ((element = mllookup(st->stack_meta_list, content, getContent,
			comp_func)) == NULL) ? element : mltmp(st->stack_meta_list);
}

/**
 * @brief push element on stack
 *
 * @param *st stack
 * @param *content content for pushing
 * @retval void
 **/
void push(STACK st, void *content) {
	mlaf(st->stack_meta_list, content);
}

/**
 * @brief return top element of stack
 *
 * @param *st stack
 * @retval void* pointer to element
 **/
void *top(const STACK st) {
	return mlfirst(st->stack_meta_list);
}

/**
 * @brief pop element from stack and return content
 *
 * @param *st stack
 * @retval void* pointer to content
 **/
void *pop(STACK st) {
	void *ext;

	ext = top(st);

	mldel(st->stack_meta_list);
	return ext;
}

/**
 * @brief empty stack
 *
 * @param st stack
 * @retval void
 **/
void flush_stack(STACK st) {
	mlflush(st->stack_meta_list);
}

/**
 * @brief delete stack
 *
 * @param st stack
 * @retval void
 **/
void free_stack(STACK st) {
	mlfree(st->stack_meta_list);
}

/**
 * @brief get size of stack
 *
 * @param st stack
 * @retval size_t stacksize
 **/
size_t size_stack(const STACK st) {
	return mlcount(st->stack_meta_list);
}

/**
 * @brief check if stack is empty
 *
 * @param st stack
 * @retval int 1 if empty, 0 if full
 **/
int empty_stack(const STACK st) {
	return mlempty(st->stack_meta_list);
}

/**
 * @brief initialize new queue
 *
 * @retval QUEUE new queue
 **/
QUEUE init_queue() {
	QUEUE queue;

	if ((queue = malloc(sizeof(*queue))) == NULL)
		error(mod[QU_ERR], __FILE__, __func__, __LINE__, ERR_MEMORY);

	queue->queue_meta_list = init_meta_list();
	return queue;
}

/**
 * @brief add new element to queue
 *
 * @param *qu queue
 * @param *content pointer to element insert in queue
 * @retval void
 **/
void append(QUEUE qu, void *content) {
	mlal(qu->queue_meta_list, content);
}

/**
 * @brief return head element of queue
 *
 * @param *qu to queue
 * @retval void* pointer to content
 **/
void *head(const QUEUE qu) {
	return mlfirst(qu->queue_meta_list);
}

/**
 * @brief return tail element of queue
 *
 * @param *qu to queue
 * @retval void* pointer to content
 **/
void *tail(const QUEUE qu) {
	return mllast(qu->queue_meta_list);
}

/**
 * @brief remove first element and return content
 *
 * @param *qu queue
 * @retval void* pointer to content
 **/
void *qudel(QUEUE qu) {
	void *ext;

	ext = head(qu);

	mldel(qu->queue_meta_list);
	return ext;
}

/**
 * @brief empty queue
 *
 * @param *qu queue
 * @retval void
 **/
void flush_queue(QUEUE qu) {
	mlflush(qu->queue_meta_list);
}

/**
 * @brief delete queue
 *
 * @param *qu to queue
 * @retval void
 **/
void free_queue(QUEUE qu) {
	mlfree(qu->queue_meta_list);
}
/**
 * @brief get size of queue
 *
 * @param *qu queue
 * @retval size_t queuesize
 **/

size_t size_queue(const QUEUE qu) {
	return mlcount(qu->queue_meta_list);
}

/**
 * @brief check for empty queue
 *
 * @param *qu queue
 * @retval int 1 if empty, 0 if full
 **/
int empty_queue(const QUEUE qu) {
	return mlempty(qu->queue_meta_list);
}


#define FULL_HASH -1

/**
 * @struct hash_table
 *
 * @brief Simple implementation for a hashtable.
 */
struct hash_table {
		unsigned HASH_SIZE;
		void **table;
};

/**
 * @brief initialize new hash
 *
 * @param data_type define data which should be stored
 * @param size how much entries should be stored into hash table
 * @param cast function pointer to cast table type
 * @retval new_hash pointer to new hash
 */
HASHTABLE init_hash(void *data_type, size_t *size, void *(*cast)(void *)) {
	HASHTABLE new_hash = NULL;

	if ((new_hash = malloc(sizeof(*new_hash))) == NULL)
		error(mod[ML_ERR],
		__FILE__, __func__, __LINE__, ERR_MEMORY);

	if ((data_type = malloc(sizeof((cast)(data_type)) * *size)) == NULL)
		error(mod[HA_ERR],
			__FILE__, __func__, __LINE__, ERR_MEMORY);

	new_hash->HASH_SIZE = (1 << (*size + 1)) - 1;
	new_hash->table = &data_type;

	return new_hash;
}

/**
 * @brief check for size of hash.
 *
 * Size left of zero means hash is full
 *
 * @param hash hash table
 * @retval size size of hash table
 */
int full_hash(HASHTABLE hash) {
	return hash->HASH_SIZE;
}

/**
 * @brief generate hash key.
 *
 * Hash key generating by double hashing.
 *
 * Hash functions used:\n
 * \f$ h(k)  := (h(k) * 128 + k)\;mod\;13 \f$\n
 * \f$ h'(k) := (h'(k) * 128 + k)\;mod\;11 \f$
 *
 * With probing function: \n
 * \f$ h(k, i) := (h(k) + i * h'(k))\;mod\;13 \f$
 *
 * @param hash hash table
 * @param s name for which hash key should be generated
 * @retval hash key positive hash key
 */
static unsigned genHashKey(HASHTABLE hash, char *s) {
	unsigned hashval1, hashval2, hashkey;
	unsigned int i;

	for (hashval1 = hashval2 = 0; *s != '\0'; s++) {
		hashval1 = ((hashval1 << 7) + *s) % 13;
		hashval2 = ((hashval2 << 7) + *s) % 11;
	}

	hashkey = hashval1;
	if (hash->HASH_SIZE & (1 << hashkey)) {
		hash->HASH_SIZE = hash->HASH_SIZE & ~(1 << hashkey);
		return hashkey;
	}
	else {
		for (i = 0; hash->HASH_SIZE > 0 && (hash->HASH_SIZE & (1 << hashkey)); i++)
			hashkey = (hashval1 + i * hashval2) % 13;
		hash->HASH_SIZE = hash->HASH_SIZE & ~(1 << hashkey);
		return hashkey;
	}
}

/**
 * @brief add item to hash table.
 *
 * @param hash hash table
 * @param s index of item
 * @param element item to store
 * @retval void
 */
void insertHash(HASHTABLE hash, char *s, void *element) {

	if (hash == NULL)
		error(mod[HA_ERR], __FILE__, __func__, __LINE__, NULL_POINTER);

	if (full_hash(hash))
		hash->table[genHashKey(hash, s)] = element;
	else
		error(mod[HA_ERR], __FILE__, __func__, __LINE__, HASH_FULL);
}

/**
 * @brief get element for given index
 *
 * @param hash hash table
 * @param s element index
 * @retval *element pointer to element
 */
void *getHash(HASHTABLE hash, char *s) {
	void *element;

	if (hash == NULL)
		error(mod[HA_ERR], __FILE__, __func__, __LINE__, NULL_POINTER);

	return ((element = hash->table[genHashKey(hash, s)]) == NULL) ? NULL : element;
}
