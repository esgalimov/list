#include "list.h"

int main(void)
{
    list_s list = {};

    list_ctor(&list);

    //list_dump(&list);

    list_push(&list, 3, 4);
    list_push(&list, 4, 5);
    list_push(&list, 7, 5);
    list_push(&list, 8, list.tail);
    list_push(&list, 9, list.head - 1);

    list_dump(&list);


    return 0;
}
