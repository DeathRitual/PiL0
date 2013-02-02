/* Header adt.h - abstract data types like stack (FIFO) */

typedef struct _tag_stack *stack;

extern void s_init(stack *);
extern int s_IsEmpty(stack);
extern void push(stack *, int *);
extern void pop(stack *);
extern int s_top(stack);
extern void s_free(stack *);


typedef struct _tag_list *list;

extern void l_init(list *);
extern int l_IsEmpty(list);
extern void l_append(list *, int *);
extern void l_remove(list *);
extern int l_top(list);
extern int l_last(list);

