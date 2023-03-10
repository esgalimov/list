#include "list.h"
#include "debug.h"

FILE * graphiz_file = NULL;

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
    assert(graphiz_file != NULL);

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
        if (list->data[i].prev != -1 && i != 0 && list->data[i].prev != 0)
        {
            link_nodes(i, list->data[i].prev, "#DC143C");
            link_nodes(list->data[i].prev, i, "#0000CD");
        }
        else if (list->data[i].prev == -1 && i != 0 && list->data[i].next != 0)
        {
            link_nodes(i, list->data[i].next, "#98FB98");
        }
    }

    link_head_tail_free(list);

    system("dot ~/documents/prog/list/graphiz.dot -Tpng -o ~/documents/prog/list/list_dump.png");

    return 0;
}
