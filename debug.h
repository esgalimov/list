#ifndef DUMP_GRAPHIZ_FUNCS
#define DUMP_GRAPHIZ_FUNCS

#include "list.h"

//! @brief Element's type in List

typedef int elem;

//! @brief Log file

extern FILE * graphiz_file;

//! @brief Func to open graphiz_file

int open_graphiz_file(void);

//! @brief Func to close graphiz_file
//! write "}"before closing to finish making graphiz file

int close_graphiz_file(void);

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

int dump_list(list_s * list);

//! @brief Add arrows from node_info to head, tail and free
//!
//! @param list [in] - ptr to list

int link_head_tail_free(list_s * list);

#endif
