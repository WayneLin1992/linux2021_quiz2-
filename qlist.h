#ifndef LINKEDLIST_QUICKSORT_H_INCLUDED
#define LINKEDLIST_QUICKSORT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct __node {
    int value;
    struct __node *next;
} node_t;

static inline void list_add_node_t(node_t **list, node_t *node_t) {
    node_t->next = *list;
    *list = node_t;
}

static inline void list_concat(node_t **left, node_t *right) {
    while (*left)
        left = &((*left)->next);
    *left = right;
}

void quicksort(node_t **list)
{
    if (!*list)
        return;

    node_t *pivot = *list;
    int value = pivot->value;
    node_t *p = pivot->next;
    pivot->next = NULL;

    node_t *left = NULL, *right = NULL;
    while (p) {
        node_t *n = p;
        p = p->next;
        list_add_node_t(n->value > value ? &right : &left, n);
    }

    quicksort(&left);
    quicksort(&right);

    node_t *result = NULL;
    list_concat(&result, left);
    list_concat(&result, pivot);
    list_concat(&result, right);
    *list = result;
}
bool non_quickSort(int *arr, int elements) {

      #define  MAX_LEVELS  1000

      int  piv, beg[MAX_LEVELS], end[MAX_LEVELS], i=0, L, R ;

      beg[0]=0; end[0]=elements;
      while (i>=0) {
        L=beg[i]; R=end[i]-1;
        if (L<R) {
          piv=arr[L]; if (i==MAX_LEVELS-1) return false;
          while (L<R) {
            while (arr[R]>=piv && L<R) R--; if (L<R) arr[L++]=arr[R];
            while (arr[L]<=piv && L<R) L++; if (L<R) arr[R--]=arr[L]; }
          arr[L]=piv; beg[i+1]=L+1; end[i+1]=end[i]; end[i++]=L; }
        else {
          i--; }}
      return true; }

void quicksort_nonrecursive(node_t**list, int count1){
    #define MAX_AY 100000
    int arr[MAX_AY];
    node_t *ptr = *list;

    int j=0;
    while(ptr){
        arr[j++] = ptr->value;
        ptr = ptr->next;
    }
    non_quickSort(arr,count1);
    node_t *ptr1 = *list;
    int i = 0;
    while(ptr1){
        ptr1->value = arr[i++];
        ptr1 = ptr1->next;
    }
}

static bool list_is_ordered(node_t *list) {
    bool first = true;
    int value;
    while (list) {
        if (first) {
            value = list->value;
            first = false;
        } else {
            if (list->value < value)
                return false;
            value = list->value;
        }
        list = list->next;
    }
    return true;
}

static void list_display(node_t *list) {
    printf("%s IN ORDER : ", list_is_ordered(list) ? "   " : "NOT");
    while (list) {
        printf("%d ", list->value);
        list = list->next;
    }
    printf("\n");
}

static void list_free(node_t **list){
    node_t *ptr = *list;
    while(*list){
        ptr = (*list)->next;
        free(*list);
        *list = ptr;
    }
}

static inline node_t* list_make_node_t(node_t *list, int num){
    node_t *p = malloc(sizeof(node_t));
    if(p==NULL){
        return list;
    }else{
        p->value = num;
        p->next = NULL;
        list_add_node_t(&list,p);
    }
    return list;
}

#endif // LINKEDLIST_QUICKSORT_H_INCLUDED
