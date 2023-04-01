#include "list.h"
#include "debug.h"

void test1(void)
{
    list_t list = {};

    list_ctor(&list);

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
}

void test2(void)
{
    list_t list = {};
    list_ctor(&list);

    list_insert_after(&list, 34, 5);
    list_insert_after(&list, 45, 7);
    list_pop(&list, 14);
    list.data = NULL;
    list.tail = -1;
    list_dump(&list);

    list_dtor(&list);
}

void test3(void)
{
    list_t list = {};
    list_ctor(&list);

    list_insert_after(&list, 56, 6);
    list_insert_after(&list, 14, 1);
    list_insert_after(&list, 23, 1);
    list_insert_before(&list, 45, 1);
    list_insert_before(&list, 89, 3);

    list_insert_tail(&list, 78);
    list_insert_head(&list, 91);

    printf("element with value = 89 have index = %d\n", list_find_elem(&list, 89));
    printf("element with logical index = 4 have phys index = %d\n",
           get_element_by_logical_index_but_it_is_too_long_so_save_phycal_indexes(&list, 4));

    list_linearize(&list);

    list_dtor(&list);
}
