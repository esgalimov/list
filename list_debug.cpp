#include "list.h"
#include "debug.h"

FILE * graphiz_file = NULL;
FILE * log_file = NULL;

int open_graphiz_file(void)
{
    graphiz_file = fopen("graphiz.dot", "w");

    if (graphiz_file == NULL)
    {
        printf("Can't open graphiz file\n");
        return 1;
    }
    return 0;
}

int close_graphiz_file(void)
{
    if (graphiz_file == NULL)
    {
        printf("Graphiz file has NULL pointer, can't close it\n");
        return 1;
    }
    fprintf(graphiz_file, "}");
    fclose(graphiz_file);
    return 0;
}

int open_log_file(void)
{
    log_file = fopen("log.html", "w");

    if (log_file == NULL)
    {
        printf("Can't open log file\n");
        return 1;
    }
    return 0;
}

int close_log_file(void)
{
    if (log_file == NULL)
    {
        printf("Log file has NULL pointer, can't close it\n");
        return 1;
    }
    fprintf(log_file, "}");
    fclose(log_file);
    return 0;
}

int graphiz_init(list_s * list)
{
    assert(list != NULL);
    assert(graphiz_file != NULL);

    fprintf(graphiz_file, "digraph\n{\n    rankdir = LR;\n");
    fprintf(graphiz_file, "    node_info[shape = record, label = \"{{name = %s \\n func = %s \\n file = %s \\n\n\
              line = %d} | {capacity = %d | size = %d | <f0> head = %d | <f1> tail = %d | <f2> free = %d}}\"];\n\n",
            list->info.name, list->info.func, list->info.file, list->info.line, list->capacity,
            list->size, list->head, list->tail, list->free);

    return 0;
}

int add_node(node * nd, int index, const char * color)
{
    assert(nd != NULL);
    assert(graphiz_file != NULL);
    assert(color != NULL);

    if (nd->prev != FREE)
    {
        fprintf(graphiz_file, "    node%d[shape = Mrecord, label = \"{index = %d} | {value = %d} | {next = %d | prev = %d}\",\n\
            style=\"filled\", fillcolor=\"%s\"];\n", index, index, nd->value, nd->next, nd->prev, color);
    }
    else
    {
        fprintf(graphiz_file, "    node%d[shape = Mrecord, label = \"{index = %d} | {value = FREE} | {next = %d | prev = %d}\",\n\
            style=\"filled\", fillcolor=\"%s\"];\n", index, index, nd->next, nd->prev, color);
    }
    return 0;
}

int link_nodes(int index1, int index2, const char * color)
{
    assert(color != NULL);
    fprintf(graphiz_file, "    node%d->node%d [color = \"%s\"];\n", index1, index2, color);

    return 0;
}

int link_head_tail_free(list_s * list)
{
    assert(list != NULL);
    assert(graphiz_file != NULL);

    fprintf(graphiz_file, "    node_info:<f0>->node%d [color = \"gray\", style=\"dashed\"];\n", list->head);
    fprintf(graphiz_file, "    node_info:<f1>->node%d [color = \"gray\", style=\"dashed\"];\n", list->tail);
    fprintf(graphiz_file, "    node_info:<f2>->node%d [color = \"gray\", style=\"dashed\"];\n", list->free);

    return 0;
}

int list_dump(list_s * list)
{
    assert(list != NULL);

    open_graphiz_file();

    graphiz_init(list);

    add_node(&list->data[0], 0, "#98FB98");

    int i = 1;

    for (; i < list->capacity; i++)
    {
        if (i == list->head || i == list->tail)
        {
            add_node(&list->data[i], i, "#FFD0D0");
        }
        else if (list->data[i].prev != FREE)
        {
            add_node(&list->data[i], i, "#87CEEB");
        }
        else
        {
            add_node(&list->data[i], i, "#F0E68C");
        }
    }

    for (i = 1; i < list->capacity; i++)
    {
        if (list->data[i].prev != -1 && list->data[i].prev != 0)
        {
            link_nodes(i, list->data[i].prev, "#DC143C");
        }
        if (list->data[i].prev != -1 && list->data[i].next != 0)
        {
            link_nodes(i, list->data[i].next, "#0000CD");
        }
        else if (list->data[i].prev == -1 && list->data[i].next != 0)
        {
            link_nodes(i, list->data[i].next, "#98FB98");
        }
    }

    link_head_tail_free(list);

    close_graphiz_file();
    system("dot ~/documents/prog/list/graphiz.dot -Tpng -o ~/documents/prog/list/list_dump.png");

    return 0;
}

int queue_verify(list_s * list)
{
    assert(list != NULL);

    int error_number = 0;

    if (list->data == NULL)
        error_number += DATA_PTR_NULL;

    if (list->size < 0)
        error_number += SIZE_ERROR;
    if (list->size > list->capacity)
        error_number += SIZE_CAP_ERROR;

    if (list->capacity <= 0)
        error_number += CAPACITY_ERROR;

    if (list->free <= 0)
        error_number += FREE_ERROR;
    if (list->free >= list->capacity)
        error_number += FREE_CAP_ERROR;

    if (list->data[0].value != 0 || list->data[0].next != 0 || list->data[0].prev != 0)
        error_number += NULL_ELEMENT_ERROR;

    if (list->head < 0)
        error_number += HEAD_ERROR;
    if (list->head > list->capacity)
        error_number += HEAD_CAP_ERROR;
    if (list->data[list->head].prev != 0)
        error_number += HEAD_NODE_ERROR;

    if (list->tail < 0)
        error_number += TAIL_ERROR;
    if (list->tail > list->capacity)
        error_number += TAIL_CAP_ERROR;
    if (list->data[list->tail].next != 0)
        error_number += TAIL_NODE_ERROR;

    return error_number;
}


void error_number_translate(int error_number)
{
    int i = 0;

    while (i < ERRORS_COUNT)
    {
        switch (error_number & (1 << i))
        {
            case 0:
                break;
            case DATA_PTR_NULL:
                fprintf(log_file, "Data have NULL pointer\n");
                break;
            case SIZE_ERROR:
                fprintf(log_file, "Size lower than 0\n");
                break;
            case CAPACITY_ERROR:
                fprintf(log_file, "Capacity lower than 0\n");
                break;
            case SIZE_CAP_ERROR:
                fprintf(log_file, "Size bigger than capacity\n");
                break;
            case FREE_ERROR:
                fprintf(log_file, "Free lower or equal 0\n");
                break;
            case FREE_CAP_ERROR:
                fprintf(log_file, "Free bigger than capacity\n");
                break;
            case NULL_ELEMENT_ERROR:
                fprintf(log_file, "Null element changed\n");
                break;
            case TAIL_ERROR:
                fprintf(log_file, "Tail lower than 0\n");
                break;
            case TAIL_CAP_ERROR:
                fprintf(log_file, "Tail bigger than capacity\n");
                break;
            case TAIL_NODE_ERROR:
                fprintf(log_file, "Next tail element is not 0\n");
                break;
            case HEAD_ERROR:
                fprintf(log_file, "Head lower than 0\n");
                break;
            case HEAD_CAP_ERROR:
                fprintf(log_file, "Head bigger than capacity\n");
                break;
            case HEAD_NODE_ERROR:
                fprintf(log_file, "Prev head element is not 0\n");
                break;
            default:
                fprintf(log_file, "Unknown error\n");
                break;
        }
    }
}
