#include "list.h"
#include "debug.h"

int list_ctor_(list_s * list, var_info info)
{
    assert(list != NULL);
    assert(info.file);
    assert(info.func);
    assert(info.name);

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    list->info = info;

    return 0;
}

int list_dtor(list_s * list)
{
    assert(list != NULL);

    node * nd = list->head;

    while (nd->next != NULL)
    {
        node * next = nd->next;
        free(nd);
        nd = next;
    }

    list->head = NULL;
    list->tail = NULL;

    list->size = 0;
    list->status = 0;

    fprintf(log_file, "<pre>\nList %p \"%s\" at %s at %s(%d): DESTRUCTED\n</pre>\n",
                list, list->info.name, list->info.func, list->info.file, list->info.line);

    list->info.file = NULL;
    list->info.func = NULL;
    list->info.name = NULL;
    list->info.line = 0;

    return 0;
}

node * list_insert_after(list_s * list, elem value, node * nd)
{
    assert(list !=  NULL);

    if (list->status)
    {
        list_dump(list);
        return NULL;
    }

    if (list->size == 0)
    {
        list->head = (node *) calloc(1, sizeof(node));
        list->head->value = value;
        list->head->next = NULL;
        list->head->prev = NULL;
    }

    else if (nd == list->tail)
    {
        nd->next = (node *) calloc(1, sizeof(node));
        nd->next->value = value;
        nd->next->prev = list->tail;
    }

    else
    {
        node * new_next = nd->next;

        nd->next = (node *) calloc(1, sizeof(node));
        nd->next->prev = nd;
        nd->next->next = new_next;
        new_next->prev = nd;
    }

    list->size++;

    list_dump(list);

    return NULL;
}

int list_insert_tail(list_s * list, elem value)
{
    assert(list != NULL);

    return list_insert_after(list, value, list->tail);
}

int list_insert_before(list_s * list, elem value, int pos)
{
    assert(list !=  NULL);

    if (!list->free)
    {
        list->status |= FULL_DATA_ERROR;
    }

    if (pos > list->capacity - 1 || pos < 1)
    {
        list->status |= BAD_POS_INSERT;
    }

    if (list->status)
    {
        list_dump(list);
        return 1;
    }

    if (list->size == 0)
    {
        list->head = 1;
        list->data[1].value = value;
        list->free = list->data[1].next;
        list->data[1].next = 0;
        list->data[1].prev = 0;
        list->tail = 1;
    }

    else if (pos == list->head)
    {
        int next_free = list->data[list->free].next;

        list->data[list->free].value = value;
        list->data[list->head].prev = list->free;
        list->data[list->free].next = list->head;
        list->data[list->free].prev = 0;
        list->head = list->free;
        list->free = next_free;
    }

    else if (list->data[pos].prev != FREE)                         // !!!оишибка тут!!!
    {
        int next_free = list->data[list->free].next;

        list->data[list->free].value = value;
        list->data[list->data[pos].prev].next = list->free;
        list->data[list->free].prev = list->data[pos].prev;
        list->data[pos].prev = list->free;
        list->data[list->free].next = pos;
        list->free = next_free;
    }

    else
    {
        list->status |= BAD_POS_INSERT;
        list_dump(list);
        return 1;
    }
    list->size++;

    list_dump(list);

    return 0;
}

int list_insert_head(list_s * list, elem value)
{
    assert(list != NULL);

    return list_insert_before(list, value, list->head);
}

int list_pop(list_s * list, int pos)
{
    assert(list != NULL);

    if (list->status)
    {
        list_dump(list);
        return 1;
    }

    if (pos == list->head)
    {
        list->data[pos].value = 0;
        list->head = list->data[pos].next;
        list->data[list->head].prev = 0;
        list->data[pos].next = list->free;
        list->data[pos].prev = FREE;
        list->free = pos;
    }

    else if (pos == list->tail)
    {
        list->data[pos].value = 0;
        list->tail = list->data[pos].prev;
        list->data[list->tail].next = 0;
        list->data[pos].next = list->free;
        list->data[pos].prev = FREE;
        list->free = pos;
    }

    else if (pos > 0 && pos < list->capacity && list->data[pos].prev != FREE)
    {
        list->data[pos].value = 0;
        list->data[list->data[pos].prev].next = list->data[pos].next;
        list->data[list->data[pos].next].prev = list->data[pos].prev;
        list->data[pos].next = list->free;
        list->data[pos].prev = FREE;
        list->free = pos;
    }

    else
    {
        list->status |= BAD_POS_POP;
        list_dump(list);
        return 1;
    }

    list->size--;

    list_dump(list);

    return 0;
}

int list_clear(list_s * list)
{
    assert(list != NULL);

    if (list->status)
    {
        list_dump(list);
        return 1;
    }

    for (int i = 1; i < list->capacity; i++)
    {
        list->data[i].value = 0;
        list->data[i].next = i + 1;
        list->data[i].prev = FREE;
    }
    list->data[list->capacity - 1].next = 0;
    list->free = 1;
    list->head = 0;
    list->tail = 0;
    list->size = 0;

    list_dump(list);

    return 0;
}
