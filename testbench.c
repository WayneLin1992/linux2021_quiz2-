#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "dlist.h"

int main()
{
    double tim1[10];
    printf("A_size, merge, Insertion, Selection\n");
    FILE *fp = fopen("sort100000.txt", "r");
    if (!fp) {
        perror("failed to open sort100000.txt");
        exit(EXIT_FAILURE);
    }
    int buf = 0;
    fscanf (fp, "%d", &buf);
    list_ele_t *head = head_new();
    while (!feof (fp)){
        list_insert_head(head, buf);
        fscanf (fp, "%d", &buf);
    }
    fclose(fp);
    struct timespec time_start;
    struct timespec time_end;
    long int during;
    clock_gettime(CLOCK_MONOTONIC, &time_start);
    list_merge_sort(head);
    clock_gettime(CLOCK_MONOTONIC, &time_end);
    //list_show(&head->list);

    //double tim1 = ((double)(end - start));//*100000000;
    during = time_end.tv_nsec - time_start.tv_nsec;

    list_free(&head->list);
    printf("%li\n", during);
    return 0;
}
