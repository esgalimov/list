#include "list.h"
#include "debug.h"

int main(void)
{
    list_s list = {};

    list_ctor(&list);

    open_log_file();


    list_insert_before(&list, 3, 1);
    list_insert_tail(&list, 6);
    list_insert_tail(&list, 7);
    list_insert_tail(&list, 9);
    list_insert_tail(&list, 3);
    list_pop(&list, 3);

    //list.data[4].next = 2;
    list_dump(&list);

    close_log_file();

    return 0;
}
