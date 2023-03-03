#include "list.h"

int list_ctor_(list_s * list, var_info info)
{
    assert(list != NULL);
    assert(info.file);
    assert(info.func);
    assert(info.name);

    list->data = (node *) calloc(SIZE, sizeof(node));
    list->head = 0;
    list->tail = 0;
    list->free = 0;

    list->info = info;

    if (list->data == NULL)
    {
        printf("Ctor error");
        return 1;
    }

    for (size_t i = 1; i < SIZE; i++)
    {
        list->data[i].next = -1;
    }

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

    list->info.file = NULL;
    list->info.func = NULL;
    list->info.name = NULL;
    list->info.line = 0;

    return 0;
}

int list_insert(list_s * list, elem value, int pos)
{
    assert(list !=  NULL);

    int free = find_free(list);

    if (!free)
    {
        printf("Data is full\n"); // it will be realloced in next versions
        return 1;
    }

    if (list->head == 0) // empty
    {
        list->head = pos + 1;
        list->data[pos + 1].value = value;
        list->data[pos + 1].next = 0;
        list->tail = pos + 1;
    }

    else if (pos == list->tail)
    {
        list->data[free].value = value;
        list->data[free].next = 0;
        list->data[list->tail].next = free;
        list->tail = free;
    }

    else if (list->data[pos].next != -1)
    {
        list->data[free].value = value;
        list->data[free].next = list->data[pos].next;
        list->data[pos].next = free;
    }

    else
    {
        printf("Error: bad pos\n");
        return 1;
    }

    return 0;
}

int list_insert_head(list_s * list, elem value)
{
    if (list->head == 0)
    {
        list_insert(list, value, 1);
    }

    int free = find_free(list);

    if (!free)
    {
        printf("Data is full\n"); // it will be realloced in next versions
        return 1;
    }

    list->data[free].value = value;
    list->data[free].next = list->head;
    list->head = free;

    return 0;
}

int list_pop(list_s * list, int pos)
{
    assert(list != NULL);

    if (list->data[pos].next != -1 && list->data[pos].next != 0)
    {
        list->data[list->data[pos].next].value = 0;
        list->data[list->data[pos].next].next = -1;
        list->data[pos].next = list->data[list->data[pos].next].next;

        if (list->data[pos].next == 0)
        {
            list->tail = pos;
        }
    }

    else
    {
        printf("Error: bad pos");
        return 1;
    }
    return 0;
}

int find_free(list_s * list)
{
    assert(list != NULL);

    int i = 1;

    for (; i < SIZE; i++)
    {
        if (list->data[i].next == -1)
        {
            return i;
        }
    }

    return 0;
}

void list_dump(list_s * list)
{
    assert(list != NULL);

    for (int i = 0; i < SIZE; i++) printf("%4d", i);
    printf("\n");

    for (int i = 0; i < SIZE; i++)
    {
        printf("%4d", list->data[i].value);
    }

    printf("\n");

    for (int i = 0; i < SIZE; i++)
    {
        printf("%4d", list->data[i].next);
    }

    printf("\n head = %d\n tail = %d \n\n", list->head, list->tail);
}
