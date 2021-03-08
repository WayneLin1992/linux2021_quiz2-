#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "DDlinkedlist.h"

int main()
{
    int count = 20;
    list_ele_t *head = head_new();
    srand(time(NULL));
    while (count--)
        list_insert_head(head, rand() % 1024);

    list_merge_sort(head);
    list_show(&head->list);
    assert(validate(head));
    list_free(&head->list);

}
