#include "list.h"

int main(void)
{
    list_s list = {};

    list_ctor(&list);

    //list_dump(&list);

//     list_insert_after(&list, 3, 1);
//     list_insert_after(&list, 4, 1);
//     list_insert_after(&list, 7, 2);
//     list_insert_after(&list, 8, list.tail);
//     list_insert_after(&list, 5, 1);
//
//     list_dump(&list);
//     list_pop(&list, list.head);
//     list_dump(&list);
//     list_pop(&list, list.tail);
//     list_pop(&list, 2);
//     list_pop(&list, 3);
//     list_dump(&list);
//     list_pop(&list, 5);
//     list_dump(&list);
//     list_insert_after(&list, 3, 1);
//     list_dump(&list);

        list_insert_before(&list, 3, 1);
        list_dump(&list);
        list_insert_before(&list, 5, 1);
        list_dump(&list);
        list_insert_before(&list, 2, 1);
        list_dump(&list);
        list_insert_before(&list, 6, list.head);
        list_dump(&list);
        list_insert_before(&list, 8, list.head);

        list_dump(&list);

    //list_pop(&list, 2);

    return 0;
}
