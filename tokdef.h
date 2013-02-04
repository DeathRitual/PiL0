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
extern struct _tag_list{
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
      unsigned int ID; /**< number identifier */
    }number;
    
    /**
     * @struct word
     * 
     * Stores keywords and identifier.
     **/
    struct _word {
      char w[20]; /**< keyword / identifer */
      unsigned int ID; /**< keyword / identifier identifier */
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

typedef struct{
  char w[10]; /**< keyword */
  int ID; /**< keyword identifier */
  /**
  * @struct keyword  
  * 
  * @brief Keyword structure for storing initial keywords
  * 
  * Stores string and ID of the language keywords like DO, WHILE, IF, etc.
  **/
} keyword;

extern void l_init(list *);
extern int l_IsEmpty(list);
extern void l_append(list *, char*, int *);
extern void l_remove(list *);
extern list l_top(list);
extern list l_last(list);

extern keyword* ini_BEGIN();
extern keyword* ini_CALL();
extern keyword* ini_CONST();
extern keyword* ini_DO();
extern keyword* ini_END();
extern keyword* ini_IF();
extern keyword* ini_ODD();
extern keyword* ini_PRINT();
extern keyword* ini_PROCEDURE();
extern keyword* ini_READ();
extern keyword* ini_THEN();
extern keyword* ini_VAR();
extern keyword* ini_WHILE();
extern keyword* ini_PASS();
extern keyword* ini_EQ();
extern keyword* ini_GE();
extern keyword* ini_LE();
extern keyword* ini_NE();  