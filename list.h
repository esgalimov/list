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

const int MIN_SIZE = 8;

//! @brief Free in prev array

const int FREE = -1;

//! @struct var_info
//! @brief Information about queue to write to log
//!
//! @var name - name of queue
//! @var func - function where queue created
//! @var file - file where queue created
//! @var line - line where queue created

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
} node;

//! @brief Struct of list
//!
//! @var data - ptr to list's array
//! @var head - index of first element
//! @var tail - index of last element
//! @var free - index of first free element in list's array
//! @var info - struct with info about variable to use it in logs

typedef struct
{
    node *  data;
    int     head;
    int     tail;
    int     free;
    int     size;
    int capacity;
    int   status;

    var_info info;

} list_s;

//! @brief Constructor for list struct
//!
//! @param [out] qu - ptr to list
//! @param [in] info - info about list
//!
//! @return 0 - if created OK, 1 - else

int list_ctor_(list_s * list, var_info info);


//! @brief Destructor for list
//!
//! @param [in] qu - ptr to list
//! @return 0 if OK

int list_dtor(list_s * list);

//! @brief Insert value into list after given
//!
//! @param [in] list - ptr to list
//! @param [in] value - value to push
//! @param [in] pos - index of element after which we push
//!
//! @return 0 - Ok, 1 - error

int list_insert_after(list_s * list, elem value, int pos);

//! @brief Insert value into list befor given
//!
//! @param [in] list - ptr to list
//! @param [in] value - value to push
//! @param [in] pos - index of element before which we push
//!
//! @return 0 - Ok, 1 - error

int list_insert_before(list_s * list, elem value, int pos);

//! @brief Insert value into list after tail
//!
//! @param [in] list - ptr to list
//! @param [in] value - value to push
//!
//! @return 0 - Ok, 1 - error

int list_insert_tail(list_s * list, elem value);

//! @brief Insert value into list before head
//!
//! @param [in] list - ptr to list
//! @param [in] value - value to push
//!
//! @return 0 - Ok, 1 - error

int list_insert_head(list_s * list, elem value);

//! @brief Remove element after given index
//!
//! @param [in] list - ptr to list's struct
//! @param pos - index after which element will be removed
//!
//! @return 0 if succes, else - 1

int list_pop(list_s * list, int pos);

//! @brief Func to clear list
//!
//! @param [in] list - ptr to list's struct
//! @return 0 - if OK

int list_clear(list_s * list);



#endif
