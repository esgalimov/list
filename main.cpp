#include "list.h"

int main(void)
{
    list_s list = {};

    list_ctor(&list);

    //list_dump(&list);

    list_insert(&list, 3, 4);
    list_insert(&list, 4, 5);
    list_insert(&list, 7, 5);
    list_insert(&list, 8, list.tail);
    list_insert_head(&list, 9);

    list_dump(&list);

    list_pop(&list, 2);

    list_dump(&list);

    return 0;
}
