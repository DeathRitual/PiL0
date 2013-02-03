/**
 * @file tokdef.h Header-File for Token-Stream library
 * 
 * Forwards the functions of the Token-Stream library to another file which includes this header file.
 */

/**
 * @var typedef struct _tag_list *list
 * @brief short form to create pointer on Tolen-Stream
 */
typedef struct _tag_list *list;

extern void l_init(list *);
extern int l_IsEmpty(list);
extern void l_append(list *, char*, int *);
extern void l_remove(list *);
extern list l_top(list);
extern list l_last(list);