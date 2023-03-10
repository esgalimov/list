#include "list.h"
#include "debug.h"

int list_ctor_(list_s * list, var_info info)
{
    assert(list != NULL);
    assert(info.file);
    assert(info.func);
    assert(info.name);

    list->capacity = MIN_SIZE;
    list->data = (node *) calloc((size_t) list->capacity, sizeof(node));
    list->head = 0;
    list->tail = 0;
    list->size = 0;
    list->free = 1;

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
    assert(list != NULL);

    free(list->data);
    list->data = NULL;
    list->head = 0;
    list->tail = 0;
    list->free = 0;
    list->capacity = 0;
    list->size = 0;

    list->info.file = NULL;
    list->info.func = NULL;
    list->info.name = NULL;
    list->info.line = 0;

    return 0;
}

int list_insert_after(list_s * list, elem value, int pos)
{
    assert(list !=  NULL);

    if (!list->free)
    {
        printf("Data is full\n"); // it will be realloced in next versions
        return 1;
    }

    if (pos > list->capacity - 1 || pos < 1)
    {
        printf("Error push after: bad pos = %d\n", pos);
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
        printf("Error push after: bad pos = %d\n", pos);
        return 1;
    }

    list->size++;
    return 0;
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
        printf("Data is full\n"); // it will be realloced in next versions
        return 1;
    }

    if (pos > list->capacity - 1 || pos < 1)
    {
        printf("Error push after: bad pos = %d\n", pos);
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
        printf("Error push after: bad pos = %d\n", pos);
        return 1;
    }
    list->size++;
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

    if (pos == list->head)
    {
        list->data[pos].value = 0;
        list->head = list->data[pos].next;
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
        printf("Error pop: bad pos = %d\n", pos);
        return 1;
    }

    list->size--;
    return 0;
}

int list_clear(list_s * list)
{
    assert(list != NULL);

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

    return 0;
}

void list_dump(list_s * list)
{
    assert(list != NULL);

    for (int i = 0; i < list->capacity; i++) printf("%4d", i);
    printf("\n");

    for (int i = 0; i < list->capacity; i++)
    {
        printf("%4d", list->data[i].value);
    }

    printf("\n");

    for (int i = 0; i < list->capacity; i++)
    {
        printf("%4d", list->data[i].next);
    }

        printf("\n");

    for (int i = 0; i < list->capacity; i++)
    {
        printf("%4d", list->data[i].prev);
    }

    printf("\n head = %d\n tail = %d \n free = %d \n size = %d\n", list->head, list->tail, list->free, list->size);
}
