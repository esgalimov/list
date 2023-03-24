#include "list.h"
#include "debug.h"

int main(void)
{
    list_s list = {};

    list_ctor(&list);

    open_log_file();

    list_insert_before(&list, 12, 1);
    list_insert_before(&list, 89, 1);
    list_insert_before(&list, 34, 2);
    list_insert_after(&list, 67, 1);

    for (int i = 0; i < 10; i++)
        list_insert_tail(&list, 20 + i);

    for (int i = 0; i < 9; i++)
        list_pop(&list, list_get_head(&list));

    list_clear(&list);
    list_dump(&list);

    list_dtor(&list);
    close_log_file();

    return 0;
}
