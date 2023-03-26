#include "list.h"
#include "debug.h"

FILE * graphiz_file = NULL;
FILE * log_file = NULL;
int dump_cnt = 0;

int open_graphiz_file(void)
{
    graphiz_file = fopen("./tmp/graphiz.dot", "w");

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

#ifdef LOG_MODE
    int open_log_file(void)
    {
        log_file = fopen("./logs/log.html", "w");

        if (log_file == NULL)
        {
            printf("Can't open log file\n");
            return 1;
        }

        fprintf(log_file, "<html>\n");
        return 0;
    }

    int close_log_file(void)
    {
        if (log_file == NULL)
        {
            printf("Log file has NULL pointer, can't close it\n");
            return 1;
        }
        fprintf(log_file, "</html>");
        fclose(log_file);
        return 0;
    }
#else
    int open_log_file(void)  { return 0; }
    int close_log_file(void) { return 0; }
#endif

int graphiz_init(list_s * list)
{
    ASSERT(list);
    ASSERT(graphiz_file);

    fprintf(graphiz_file, "digraph\n{\n    rankdir = LR;\n");
    fprintf(graphiz_file, "    node_info[shape = record, label = \"{{capacity = %d | size = %d | <f0> head = %d | <f1> tail = %d | <f2> free = %d}}\"];\n\n",
            list->capacity, list->size, list->head, list->tail, list->free);

    return 0;
}

int add_node(node * nd, int index, const char * color)
{
    ASSERT(nd);
    ASSERT(graphiz_file);
    ASSERT(color);

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
    ASSERT(color);

    fprintf(graphiz_file, "    node%d->node%d [color = \"%s\"];\n", index1, index2, color);

    return 0;
}

int link_head_tail_free(list_s * list)
{
    ASSERT(list);
    ASSERT(graphiz_file);

    fprintf(graphiz_file, "    node_info:<f0>->node%d [color = \"gray\", style=\"dashed\"];\n", list->head);
    fprintf(graphiz_file, "    node_info:<f1>->node%d [color = \"gray\", style=\"dashed\"];\n", list->tail);
    fprintf(graphiz_file, "    node_info:<f2>->node%d [color = \"gray\", style=\"dashed\"];\n", list->free);

    return 0;
}

char * create_graphiz_cmd(void)
{
    dump_cnt++;
    const char * begin_cmd = "dot ./tmp/graphiz.dot -Tpng -o ./logs/images/list_dump";
    const char * end_cmd = ".png";
    char * cmd = (char *) calloc(100, sizeof(char));

    strcat(cmd, begin_cmd);
    snprintf(cmd + strlen(begin_cmd), 20, "%d", dump_cnt);
    strcat(cmd, end_cmd);

    return cmd;
}

int list_dump_(list_s * list, const char * func, const char * file, int line)
{
    ASSERT(list);

    char * graphiz_cmd = create_graphiz_cmd();

    list_dump_info(list, func, file, line);

    if (list->status)
        return 1;

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
    system(graphiz_cmd);

    fprintf(log_file, "<img src=\"./images/list_dump%d.png\" width=\"75%c\">\n", dump_cnt, '%');

    free(graphiz_cmd);

    return 0;
}

int list_dump_info(list_s * list, const char * func, const char * file, int line)
{
    ASSERT(list);

    list_verify(list);
    fprintf(log_file, "<pre>\n%s at %s(%d):\n", func, file, line);
    if (!list->status)
    {
        fprintf(log_file, "List %p (<span style=\"color: green\">OK</span>) \"%s\" at %s at %s(%d):\n",
                list, list->info.name, list->info.func, list->info.file, list->info.line);

        fprintf(log_file, "{\n    head     = %d\n    tail     = %d\n    free     = %d\
            \n    size     = %d\n    capacity = %d\n",
            list->head, list->tail, list->free, list->size, list->capacity);

        fprintf(log_file, "    data [%p]\n}\n", list->data);
        fprintf(log_file, "</pre>\n");
    }
    else
    {
        fprintf(log_file, "Queue %p (<span style=\"color: red\">ERROR</span>) \"%s\" at %s at %s(%d):\n",
                list, list->info.name, list->info.func, list->info.file, list->info.line);
        error_number_translate(list);

        fprintf(log_file, "{\n    head     = %d\n    tail     = %d\n    free     = %d\
            \n    size     = %d\n    capacity = %d\n",
            list->head, list->tail, list->free, list->size, list->capacity);

        fprintf(log_file, "    data [%p]", list->data);
        fprintf(log_file, "\n}\n</pre>\n");

        return 1;
    }
    return 0;
}

int list_verify(list_s * list)
{
    ASSERT(list);

    if (list->data == NULL)
        list->status |= DATA_PTR_NULL;

    if (list->size < 0)
        list->status |= SIZE_ERROR;
    if (list->size > list->capacity)
        list->status |= SIZE_CAP_ERROR;

    if (list->capacity <= 0)
        list->status |= CAPACITY_ERROR;

    if (list->free < 0)
        list->status |= FREE_ERROR;
    if (list->free >= list->capacity)
        list->status |= FREE_CAP_ERROR;

    if (list->head < 0)
        list->status |= HEAD_ERROR;
    if (list->head > list->capacity)
        list->status |= HEAD_CAP_ERROR;

     if (list->tail < 0)
        list->status |= TAIL_ERROR;
    if (list->tail > list->capacity)
        list->status |= TAIL_CAP_ERROR;

    if (list->data != NULL)
    {
        if (list->data[0].value != 0 || list->data[0].next != 0 || list->data[0].prev != 0)
            list->status |= NULL_ELEMENT_ERROR;
        if (list->data[list->head].prev != 0)
            list->status |= HEAD_NODE_ERROR;
        if (list->data[list->tail].next != 0)
            list->status |= TAIL_NODE_ERROR;
    }

    return 0;
}


void error_number_translate(list_s * list)
{
    ASSERT(list);

    int i = 0;

    while (i < ERRORS_COUNT)
    {
        switch (list->status & (1 << i))
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
            case BAD_POS_INSERT:
                fprintf(log_file, "Bad position to insert element\n");
                break;
            case BAD_POS_POP:
                fprintf(log_file, "Bad position to pop element\n");
                break;
            default:
                fprintf(log_file, "Unknown error\n");
                break;
        }
        i++;
    }
}
