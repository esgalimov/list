#ifndef DUMP_GRAPHIZ_FUNCS
#define DUMP_GRAPHIZ_FUNCS

#include "list.h"

#define LOG_MODE

#ifdef LOG_MODE
    #define ASSERT(condition)                                                               \
        if (!(condition))                                                                   \
        {                                                                                   \
            fprintf(log_file, "\nError in \"%s\" in %d line in function %s in file %s\n",   \
                    #condition, __LINE__, __PRETTY_FUNCTION__, __FILE__);                   \
            abort();                                                                        \
        }
    #define list_dump(list) list_dump_((list), __PRETTY_FUNCTION__, __FILE__, __LINE__)
#else
    #define ASSERT(condition)
    #define list_dump(list)
#endif

//! @brief Element's type in List

typedef int elem;

//! @brief Log file

extern FILE * log_file;

//! @brief Graphiz file

extern FILE * graphiz_file;

//! @brief Number for count dump pictures

extern int dump_cnt;

//! @brief Errors what can be with list
//! consists of 2ˆn numbers to use bit mask

enum ERRORS
{
    DATA_PTR_NULL      = 1 << 0,
    SIZE_ERROR         = 1 << 1,
    CAPACITY_ERROR     = 1 << 2,
    SIZE_CAP_ERROR     = 1 << 3,
    FREE_ERROR         = 1 << 4,
    FREE_CAP_ERROR     = 1 << 5,
    NULL_ELEMENT_ERROR = 1 << 6,
    TAIL_ERROR         = 1 << 7,
    TAIL_CAP_ERROR     = 1 << 8,
    TAIL_NODE_ERROR    = 1 << 9,
    HEAD_ERROR         = 1 << 10,
    HEAD_CAP_ERROR     = 1 << 11,
    HEAD_NODE_ERROR    = 1 << 12,
    BAD_POS_INSERT     = 1 << 13,
    BAD_POS_POP        = 1 << 14,
};

const int ERRORS_COUNT = 15;

//! @brief Func to open graphiz_file

int open_graphiz_file(void);

//! @brief Func to close graphiz_file
//! write "}"before closing to finish making graphiz file

int close_graphiz_file(void);

//! @brief Func to open log_file and write some html tags to start logging

int open_log_file(void);

//! @brief Func to close log_file
//! write "</html>" before closing to finish making log file

int close_log_file(void);

//!@brief Graphiz file init
//!
//! @param [in] list - ptr to list's struct

int graphiz_init(list_t * list);

//! @brief Add element into .dot file to use graphiz
//!
//! @param [in] nd - ptr to node
//! @param [in] index - index of node in data array
//! @param [in] color - color of node's filling

int add_node(node_t * nd, int index, const char * color);

//! @brief Link nodes
//!
//! @param [in] index1 - first node (from)
//! @param [in] index2 - second node (where)
//! @param [in] color - color of arrow

int link_nodes(int index1, int index2, const char * color);

//! @brief List dump
//!
//! Make .dot file using funcs above and use graphiz to make png picture of dump
//! @param [in] list - ptr to list
//! @param [in] func - ptr to func name
//! @param [in] file - ptr to file name
//! @param [in] line - line where was dump
//! @return 0

int list_dump_(list_t * list, const char * func, const char * file, int line);

//! @brief Check list and write information about it in logs
//!
//! @param [in] list - ptr to list
//! @param [in] func - ptr to func name
//! @param [in] file - ptr to file name
//! @param [in] line - line where was dump
//! @return 0

int list_dump_info(list_t * list, const char * func, const char * file, int line);

//! @brief Add arrows from node_info to head, tail and free
//!
//! @param list [in] - ptr to list

int link_head_tail_free(list_t * list);

//! @brief Func create string with graphiz console command (make png from .dot file)
//! it use global variable 'dump_number' to create name of next picture
//! cmd string buffer allocated with calloc (don't forget to free)
//!
//! @return ptr to string

char * create_graphiz_cmd(void);

//! @brief Func to check list
//! Summarize codes of mistakes to make number where each bit is concrete mistake
//!
//! @param [in] list - ptr to list
//!
//! @return error number

int list_verify(list_t * list);

//! @brief Write to log errors with using error number
//!
//! @param [in] error_number - error number what return list verify

void error_number_translate(list_t * list);

#endif
