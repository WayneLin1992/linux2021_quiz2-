#include "list_sort.h"
#include "list.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct __element {
    int value;
    struct list_head list;
} list_ele_t;

int sort_by_int(void *priv, struct list_head *a, struct list_head *b)
{
    list_ele_t *pa = list_entry(a, list_ele_t, list);
    list_ele_t *pb = list_entry(b, list_ele_t, list);
    return pa->value > pb->value;
}

static list_ele_t *head_new()
{
    list_ele_t *head = malloc(sizeof(list_ele_t));
    if (!head) return NULL;
    head->value = 0;
    INIT_LIST_HEAD(&head->list);
    return head;
}

bool list_insert_head(list_ele_t *head, int s)
{
    if (!head) return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    int new_value = s;
    if (!new_value) {
        free(newh);
        return false;
    }

    newh->value = new_value;
    list_add_tail(&newh->list, &head->list);

    return true;
}

void list_show(struct list_head *list)
{
    struct list_head *node;
    list_for_each(node, list){
        printf("%d", list_entry(node, list_ele_t, list)->value);
        printf("->");
    }
}

int main()
{
    srand(time(NULL));
    FILE *fp = fopen("sort_time_linux.txt", "w");
    if (!fp) {
        perror("failed to open sort_time_linux.txt");
        exit(EXIT_FAILURE);
    }
    fprintf(fp,"count\tlist_sort(linux_merge)\n");
    for(int i=10000; i<100000;i= i+1000){
        list_ele_t *head = head_new();
        int count = i;
        while (count--){
            list_insert_head(head, rand() % 1024);
        }
        struct timespec time_start;
        struct timespec time_end;
        long during = 0;
        //list_show(&head->list);
        clock_gettime(CLOCK_MONOTONIC, &time_start);
        list_sort(NULL,&head->list,sort_by_int);
        clock_gettime(CLOCK_MONOTONIC, &time_end);
        //list_show(&head->list);
        during = time_end.tv_nsec - time_start.tv_nsec;
        //list_free(&head->list);
        fprintf(fp,"%d\t%ld \n",i,during);
    }
    fclose(fp);
    return 0;
}
