#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "dlist.h"

int main()
{
    srand(time(NULL));
    FILE *fp = fopen("sort_time.txt", "w");
    if (!fp) {
        perror("failed to open sort_time.txt");
        exit(EXIT_FAILURE);
    }
    fprintf(fp,"count\tlist_sort(merge)\n");
    for(int i=10000; i<100000;i= i+1000){
        list_ele_t *head = head_new();
        int count = i;
        while (count--){
            list_insert_head(head, rand() % 1024);
        }
        struct timespec time_start;
        struct timespec time_end;
        long during = 0;
        clock_gettime(CLOCK_MONOTONIC, &time_start);
        list_merge_sort(head);
        clock_gettime(CLOCK_MONOTONIC, &time_end);
        during = abs(time_end.tv_nsec - time_start.tv_nsec);
        list_free(&head->list);
        fprintf(fp,"%d\t%ld \n",i,during);
    }
    fclose(fp);
    return 0;
}
