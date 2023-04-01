#include "list.h"
#include "debug.h"

int main(void)
{
    open_log_file();

    test1();
    test2();
    test3();

    close_log_file();
    return 0;
}
