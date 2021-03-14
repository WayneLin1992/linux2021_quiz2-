#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "qlist.h"

int main()
{
    srand(time(NULL));
    FILE *fp = fopen("sort_timeqn.txt", "w");
    if (!fp) {
        perror("failed to open sort_timeqn.txt");
        exit(EXIT_FAILURE);
    }
    fprintf(fp,"count\tlist_sort(nonqsort)\n");
    for(int i=10000; i<100000;i= i+1000){
        node_t *list = NULL;
        int count = i;
        while (count--){
            list = list_make_node_t(list, random() % 1024);
        }
        struct timespec time_start;
        struct timespec time_end;
        long during = 0;
        clock_gettime(CLOCK_MONOTONIC, &time_start);
        quicksort_nonrecursive(&list,i);
        clock_gettime(CLOCK_MONOTONIC, &time_end);
        during = time_end.tv_nsec - time_start.tv_nsec;
        list_free(&list);
        fprintf(fp,"%d\t%ld \n",i,during);
    }
    fclose(fp);
    return 0;
}
