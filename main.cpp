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
    printf("phys index %d\n", get_element_by_logical_index_but_it_is_too_long_so_save_phycal_indexes(&list, 2));
    list_linearize(&list);

    printf("head %d\n", list_get_head(&list));
    printf("tail %d\n", list_get_tail(&list));
    printf("next after 2 %d\n", list_get_next(&list, 2));
    printf("prev before 2 %d\n", list_get_prev(&list, 2));
    printf("isempty %d\n", list_is_empty(&list));


    list_clear(&list);

    //list.data[4].next = 2;
    list_dump(&list);

    list_dtor(&list);
    close_log_file();

    return 0;
}
