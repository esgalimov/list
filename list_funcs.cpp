#include "list.h"
#include "debug.h"

int list_ctor_(list_s * list, var_info info)
{
    ASSERT(list);

    list->capacity = MIN_SIZE;
    list->data = (node *) calloc((size_t) list->capacity, sizeof(node));
    list->head = 0;
    list->tail = 0;
    list->size = 0;
    list->free = 1;
    list->status = 0;

    list->info = info;

    if (list->data == NULL)
    {
        printf("Ctor error");
        return 1;
    }

    for (int i = 1; i < list->capacity; i++)
    {
        list->data[i].next = i + 1;
        list->data[i].prev = -1;
    }

    list->data[list->capacity - 1].next = 0;

    return 0;
}

int list_dtor(list_s * list)
{
    ASSERT(list);

    free(list->data);
    list->data = NULL;
    list->head = 0;
    list->tail = 0;
    list->free = 0;
    list->capacity = 0;
    list->size = 0;
    list->status = 0;

    #ifdef LOG_MODE
        fprintf(log_file, "<pre>\nList %p \"%s\" at %s at %s(%d): DESTRUCTED\n</pre>\n",
                    list, list->info.name, list->info.func, list->info.file, list->info.line);
    #endif

    list->info.file = NULL;
    list->info.func = NULL;
    list->info.name = NULL;
    list->info.line = 0;

    return 0;
}

int list_insert_after(list_s * list, elem value, int pos)
{
    ASSERT(list);

    if (!list->free)
    {
        list_resize(list, list->capacity * 2);
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

    else if (pos == list->tail)
    {
        list->data[list->free].value = value;
        list->data[list->free].prev = list->tail;
        list->data[list->tail].next = list->free;
        list->tail = list->free;
        list->free = list->data[list->free].next;
        list->data[list->tail].next = 0;
    }

    else if (list->data[pos].prev != FREE)
    {
        int next_free = list->data[list->free].next;

        list->data[list->free].value = value;
        list->data[list->data[pos].next].prev = list->free;
        list->data[list->free].next = list->data[pos].next;
        list->data[pos].next = list->free;
        list->data[list->free].prev = pos;
        list->free = next_free;
    }

    else
    {
        list->status += BAD_POS_INSERT;
        list_dump(list);
        return 1;
    }

    list->size++;

    list_dump(list);

    return 0;
}

int list_insert_tail(list_s * list, elem value)
{
    ASSERT(list);

    return list_insert_after(list, value, list->tail);
}

int list_insert_before(list_s * list, elem value, int pos)
{
    ASSERT(list);

    if (!list->free)
    {
        list_resize(list, list->capacity * 2);
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

    else if (list->data[pos].prev != FREE)
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
    ASSERT(list);

    return list_insert_before(list, value, list->head);
}

int list_pop(list_s * list, int pos)
{
    ASSERT(list);

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

    if (list->size < list->capacity / 4 && list->size > 0)
    {
        list_resize(list, list->capacity / 2);
    }

    list_dump(list);

    return 0;
}

int list_clear(list_s * list)
{
    ASSERT(list);

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

int list_get_next(list_s * list, int pos)
{
    ASSERT(list);

    if (pos < 0 || pos > list->capacity)
        return 0;

    return list->data[pos].next;
}

int list_get_prev(list_s * list, int pos)
{
    ASSERT(list);

    if (pos < 0 || pos > list->capacity)
        return 0;

    return list->data[pos].prev;
}

int list_find_elem(list_s * list, elem value)
{
    ASSERT(list);

    int current = list->head;

    while (list->data[current].next != 0)
    {
        if (list->data[current].value == value)
        {
            return current;
            break;
        }
    }

    return 0;
}

int list_get_head(list_s * list)
{
    ASSERT(list);

    return list->head;
}

int list_get_tail(list_s * list)
{
    ASSERT(list);

    return list->tail;
}

int get_element_by_logical_index_but_it_is_too_long_so_save_phycal_indexes(list_s * list, int log_i)
{
    ASSERT(list);

    if (log_i < 0 || log_i >= list->size)
    {
        return 0;
    }

    int current = list->head;

    for (int i = 0; i < log_i; i++)
    {
        current = list->data[current].next;
    }

    return current;
}

int list_is_empty(list_s * list)
{
    ASSERT(list);

    return !list->size;
}

int list_linearize(list_s * list)
{
    ASSERT(list);

    if (list->status)
    {
        list_dump(list);
        return 1;
    }

    node * new_data = (node *) calloc((size_t) list->capacity, sizeof(node));

    int current = list->head;
    int i = 1;

    while (list->data[current].next != 0)
    {
        new_data[i].value = list->data[current].value;
        new_data[i].prev = i - 1;
        new_data[i].next = i + 1;

        current = list->data[current].next;
        i++;
    }
    new_data[i].value = list->data[current].value;

    list->head = 1;
    list->tail = i;
    list->free = (list->free == 0) ? 0 : i + 1;

    new_data[i].prev = i - 1;

    for (i = i + 1; i < list->capacity; i++)
    {
        new_data[i].next = i + 1;
        new_data[i].prev = FREE;
        new_data[i].value = FREE;
    }

    new_data[list->capacity - 1].next = 0;

    free(list->data);

    list->data = new_data;

    list_dump(list);

    return 0;
}

int list_resize(list_s * list, int new_capacity)
{
    ASSERT(list);

    if (list->status)
    {
        list_dump(list);
        return 1;
    }

    if (new_capacity < list->capacity)
    {
        if (new_capacity < MIN_SIZE)
        {
            new_capacity = MIN_SIZE;
        }
        list_linearize(list);
        list->data = (node *) realloc(list->data, (size_t) new_capacity * sizeof(node));
        list->capacity = new_capacity;
        list->data[new_capacity - 1].next = 0;
    }
    else
    {
        list->data = (node *) realloc(list->data, (size_t) new_capacity * sizeof(node));

        for (int i = list->capacity; i < new_capacity; i++)
        {
            list->data[i].value = FREE;
            list->data[i].prev = FREE;
            list->data[i].next = i + 1;
        }
        list->data[new_capacity - 1].next = 0;


        list->free = list->capacity;
        list->capacity = new_capacity;
    }

    list_dump(list);

    return 0;
}
