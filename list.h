#ifndef MY_LIST
#define MY_LIST

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__
#define list_ctor(list) list_ctor_((list), var_info {#list, LOCATION})

//#define list_dump(list, error_number) queue_dump_((list), (error_number), __PRETTY_FUNCTION__, __FILE__, __LINE__)

//! @brief Element's type in List

typedef int elem;

//! @brief Start size of list

const int SIZE = 16;

//! @brief Log file

extern FILE * log_file;

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
    int  prev; // пока не трогаю
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
    node * data;
    int    head;
    int    tail;
    int    free;

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

//! @brief Push value into list after given
//!
//! @param [in] list - ptr to list
//! @param [in] value - value to push
//! @param [in] pos - index of element after which we push
//!
//! @return 0 - Ok, 1 - error

int list_push(list_s * list, elem value, int pos);

//! @brief Find first free element with cycle
//!
//! @param [in] list - ptr to list's struct
//!
//! @return index of first free element, or 0 if there not free

int find_free(list_s * list);

//! @brief Func for print list's data
//!
//! @param [in] list - ptr to list's struct

void list_dump(list_s * list);



#endif
