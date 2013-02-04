/**
 * @file tokdef.h Header-File for Token-Stream library
 * 
 * Forwards the functions of the Token-Stream library to another file which includes this header file.
 */

/**
 * @struct _tag_list  
 * 
 * @brief Element-Structure which builds Token-Stream.
 * 
 * One element can only contain one token, whereas each element is connected with its last
 * and previous neighbour.
 **/
extern struct _tag_list {
  /**
   * @union Token  
   * 
   * @brief Different Token for storing symbols.
   * 
   * Except for Token: token all token have an ID which helps identifying
   * the type of the keyword, identifer or number.
   **/
  union {
    /**
     * @struct token
     * 
     * Stores single character symbols like: +, -, >, <, etc.
     **/
    struct _token {
      char t; /**< single character */
    }token;
    
    /**
     * @struct number
     * 
     * Stores numbers.
     **/
    struct _number {
      int n; /**< number */ 
      unsigned int ID; /**< identifier */
    }number;
    
    /**
     * @struct word
     * 
     * Stores keywords and identifier.
     **/
    struct _word {
      char w[50]; /**< keyword / identifer */
      unsigned int ID; /**< identifier */
    }word;
  };

  char type; /**< Token-Type */
  struct _tag_list *next; /**< pointer to next element */
  struct _tag_list *previous; /**< pointer to previous element */
}tag_list; /**< dummy element to quit compiler */

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