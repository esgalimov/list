#ifndef MY_LIST
#define MY_LIST

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__
#define list_ctor(list) list_ctor_((list), var_info {#list, LOCATION})


//! @brief Element's type in List
typedef int elem;

//! @brief Start size of list
const int MIN_SIZE = 12;

//! @brief Free in prev array
const int FREE = -1;

//! @struct var_info
//! @brief Information about list to write to log
//!
//! @var name - name of list
//! @var func - function where list created
//! @var file - file where list created
//! @var line - line where list created
typedef struct
{
    const char * name;
    const char * func;
    const char * file;
    int          line;
} var_info;

//! @brief Struct of list's node
//!
//! @var value - value of node
//! @var next - index of next element
//! @var prev - index 0f previous element
typedef struct
{
    elem value;
    int  next;
    int  prev;
} node_t;

//! @brief Struct of list
//!
//! @var data - ptr to list's array
//! @var head - index of first element
//! @var tail - index of last element
//! @var free - index of first free element in list's array
//! @var info - struct with info about variable to use it in logs
typedef struct
{
    node_t *  data;
    int     head;
    int     tail;
    int     free;
    int     size;
    int capacity;
    int   status;

    var_info info;

} list_t;

//! @brief Constructor for list struct
//!
//! @param [out] qu - ptr to list
//! @param [in] info - info about list
//!
//! @return 0 - if created OK, 1 - else
int list_ctor_(list_t * list, var_info info);


//! @brief Destructor for list
//!
//! @param [in] qu - ptr to list
//! @return 0 if OK
int list_dtor(list_t * list);

//! @brief Insert value into list after given
//!
//! @param [in] list - ptr to list
//! @param [in] value - value to push
//! @param [in] pos - index of element after which we push
//!
//! @return 0 - Ok, 1 - error
int list_insert_after(list_t * list, elem value, int pos);

//! @brief Insert value into list befor given
//!
//! @param [in] list - ptr to list
//! @param [in] value - value to push
//! @param [in] pos - index of element before which we push
//!
//! @return 0 - Ok, 1 - error
int list_insert_before(list_t * list, elem value, int pos);

//! @brief Insert value into list after tail
//!
//! @param [in] list - ptr to list
//! @param [in] value - value to push
//!
//! @return 0 - Ok, 1 - error
int list_insert_tail(list_t * list, elem value);

//! @brief Insert value into list before head
//!
//! @param [in] list - ptr to list
//! @param [in] value - value to push
//!
//! @return 0 - Ok, 1 - error
int list_insert_head(list_t * list, elem value);

//! @brief Remove element after given index
//!
//! @param [in] list - ptr to list's struct
//! @param pos - index after which element will be removed
//!
//! @return 0 if succes, else - 1
int list_pop(list_t * list, int pos);

//! @brief Func to clear list
//!
//! @param [in] list - ptr to list's struct
//! @return 0 - if OK
int list_clear(list_t * list);

//! @brief Return element after given
//!
//! @param [in] list - ptr to list
//! @param [in] pos - index of element to get next
//! @return 0 - if bad pos or tail, else - phys index of next
int list_get_next(list_t * list, int pos);

//! @brief Return element before given
//!
//! @param [in] list - ptr to list
//! @param [in] pos - index of element to get prev
//! @return 0 - if bad pos or head, else - phys index of prev
int list_get_prev(list_t * list, int pos);

//! @brief Find element with given value
//!
//! @param [in] list - ptr to list
//! @param [in] value - value to find
//! @return 0 - if not found, else - phys index of element
int list_find_elem(list_t * list, elem value);

//! @brief Return list head
int list_get_head(list_t * list);

//! @brief Return list tail
int list_get_tail(list_t * list);

//! @brief Get physical index by logical
//!
//! @param [in] list - ptr to list
//! @param [in] log_i - logical index
//! @return 0 - if bad logical index, else - logical index
int get_element_by_logical_index_but_it_is_too_long_so_save_phycal_indexes(list_t * list, int log_i);

//! @brief 1 - if empty, 0 - else
//! @param [in] list - ptr to list
int list_is_empty(list_t * list);

//! @brief Make list linearize (logical_index = phys_index - 1)
//! @param [in] list - ptr to list
//! @return 1 - if mistake, 0 - Ok
int list_linearize(list_t * list);

//! @brief Func to resize list's data
//! @param [in] list - ptr to list
//! @param [in] new_capacity - new capacity for list
//! @return 1 - error, 0 - Ok
int list_resize(list_t * list, int new_capacity);


//--------------------TESTS-----------------------
//! @brief Some tests of list work
void test1(void);
void test2(void);
void test3(void);


#endif
