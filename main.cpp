#include "list.h"
#include "debug.h"

int main(void)
{
    list_s list = {};

    list_ctor(&list);

    open_graphiz_file();


    list_insert_before(&list, 3, 1);
    list_insert_tail(&list, 6);
    list_insert_tail(&list, 7);
    list_insert_tail(&list, 9);
    list_insert_tail(&list, 3);


    list_dump(&list);

    dump_list(&list);

    close_graphiz_file();

    return 0;
}
