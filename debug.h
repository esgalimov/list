#ifndef DUMP_GRAPHIZ_FUNCS
#define DUMP_GRAPHIZ_FUNCS

#include "list.h"

//! @brief Element's type in List

typedef int elem;

//! @brief Log file

extern FILE * log_file;

//! @brief Errors what can be with list
//! consists of 2ˆn numbers to use bit mask

enum ERRORS
{
    DATA_PTR_NULL = 1,
    SIZE_ERROR = 2,
    CAPACITY_ERROR = 4,
    SIZE_CAP_ERROR = 8,
    FREE_ERROR = 16,
    FREE_CAP_ERROR = 32,
    NULL_ELEMENT_ERROR = 64,
    TAIL_ERROR = 128,
    TAIL_CAP_ERROR = 256,
    TAIL_NODE_ERROR = 512,
    HEAD_ERROR = 1024,
    HEAD_CAP_ERROR = 2048,
    HEAD_NODE_ERROR = 4096,
};

const int ERRORS_COUNT = 13;

//! @brief Log file

extern FILE * graphiz_file;

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

int graphiz_init(list_s * list);

//! @brief Add element into .dot file to use graphiz
//!
//! @param [in] nd - ptr to node
//! @param [in] index - index of node in data array
//! @param [in] color - color of node's filling

int add_node(node * nd, int index, const char * color);

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

int list_dump(list_s * list);

//! @brief Add arrows from node_info to head, tail and free
//!
//! @param list [in] - ptr to list

int link_head_tail_free(list_s * list);

//! @brief Func to check list
//! Summarize codes of mistakes to make number where each bit is concrete mistake
//!
//! @param [in] list - ptr to queue
//!
//! @return error number

int list_verify(list_s * list);

//! @brief Write to log errors with using error number
//!
//! @param [in] error_number - error number what return list verify

void error_number_translate(int error_number);

#endif
