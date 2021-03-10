#ifndef DDLIKEDLIST_H_INCLUDED
#define DDLIKEDLIST_H_INCLUDED

#include <stddef.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * container_of() - Calculate address of object that contains address ptr
 * @ptr: pointer to member variable
 * @type: type of the structure containing ptr
 * @member: name of the member variable in struct @type
 *
 * Return: @type pointer of object containing ptr
 */
#ifndef container_of
#define container_of(ptr, type, member)                                        \
  ({                                                                           \
    const typeof(((type *)0)->member) *__mptr = (ptr);                         \
    (type *)((char *)__mptr - offsetof(type, member));                         \
  })

#endif

/**
 * struct list_head - Head and node of a doubly-linked list
 * @prev: pointer to the previous node in the list
 * @next: pointer to the next node in the list
 */
struct list_head {
    struct list_head *prev, *next;
};

/**
 * LIST_HEAD - Declare list head and initialize it
 * @head: name of the new object
 */
#define LIST_HEAD(head) struct list_head head = {&(head), &(head)}

/**
 * INIT_LIST_HEAD() - Initialize empty list head
 * @head: pointer to list head
 */
static inline void INIT_LIST_HEAD(struct list_head *head)
{
    head->next = head; head->prev = head;
}

/**
 * list_add_tail() - Add a list node to the end of the list
 * @node: pointer to the new node
 * @head: pointer to the head of the list
 */
static inline void list_add_tail(struct list_head *node, struct list_head *head)
{
    struct list_head *prev = head->prev;

    prev->next = node;
    node->next = head;
    node->prev = prev;
    head->prev = node;
}

/**
 * list_del() - Remove a list node from the list
 * @node: pointer to the node
 */
static inline void list_del(struct list_head *node)
{
    struct list_head *next = node->next, *prev = node->prev;
    next->prev = prev; prev->next = next;
}

/**
 * list_empty() - Check if list head has no nodes attached
 * @head: pointer to the head of the list
 */
static inline int list_empty(const struct list_head *head)
{
    return (head->next == head);
}

/**
 * list_is_singular() - Check if list head has exactly one node attached
 * @head: pointer to the head of the list
 */
static inline int list_is_singular(const struct list_head *head)
{
    return (!list_empty(head) && head->prev == head->next);
}

/**
 * list_splice_tail() - Add list nodes from a list to end of another list
 * @list: pointer to the head of the list with the node entries
 * @head: pointer to the head of the list
 */
static inline void list_splice_tail(struct list_head *list,
                                    struct list_head *head)
{
    struct list_head *head_last = head->prev;
    struct list_head *list_first = list->next, *list_last = list->prev;

    if (list_empty(list))
        return;

    head->prev = list_last;
    list_last->next = head;

    list_first->prev = head_last;
    head_last->next = list_first;
}

/**
 * list_cut_position() - Move beginning of a list to another list
 * @head_to: pointer to the head of the list which receives nodes
 * @head_from: pointer to the head of the list
 * @node: pointer to the node in which defines the cutting point
 */
static inline void list_cut_position(struct list_head *head_to,
                                     struct list_head *head_from,
                                     struct list_head *node)
{
    struct list_head *head_from_first = head_from->next;

    if (list_empty(head_from))
        return;

    if (head_from == node) {
        INIT_LIST_HEAD(head_to);
        return;
    }

    head_from->next = node->next;
    head_from->next->prev = head_from;

    head_to->prev = node;
    node->next = head_to;
    head_to->next = head_from_first;
    head_to->next->prev = head_to;
}

/**
 * list_entry() - Calculate address of entry that contains list node
 * @node: pointer to list node
 * @type: type of the entry containing the list node
 * @member: name of the list_head member variable in struct @type
 */
#define list_entry(node, type, member) container_of(node, type, member)

/**
 * list_first_entry() - get first entry of the list
 * @head: pointer to the head of the list
 * @type: type of the entry containing the list node
 * @member: name of the list_head member variable in struct @type
 */
#define list_first_entry(head, type, member) \
    list_entry((head)->next, type, member)

/**
 * list_for_each - iterate over list nodes
 * @node: list_head pointer used as iterator
 * @head: pointer to the head of the list
 */
#define list_for_each(node, head) \
    for (node = (head)->next; node != (head); node = node->next)

typedef struct __element {
    int value;
    struct list_head list;
} list_ele_t;

static list_ele_t *get_middle(struct list_head *list)
{
    struct list_head *fast = list->next, *slow;
    list_for_each (slow, list) {
        if (fast->next == list || fast->next->next == list)
            break;
        fast = fast->next->next;
    }
    return list_entry(slow, list_ele_t, list);
}

static void list_merge(struct list_head *lhs,
                       struct list_head *rhs,
                       struct list_head *head)
{
    INIT_LIST_HEAD(head);
    if (list_empty(lhs)) {
        list_splice_tail(lhs, head);
        return;
    }
    if (list_empty(rhs)) {
        list_splice_tail(rhs, head);
        return;
    }

    while (!list_empty(lhs) && !list_empty(rhs)) {
        int lv = list_entry(lhs->next, list_ele_t, list)->value;
        int rv = list_entry(rhs->next, list_ele_t, list)->value;
        struct list_head *tmp = (lv < rv) ? lhs->next : rhs->next;
        list_del(tmp);
        list_add_tail(tmp, head);
    }
    list_splice_tail(list_empty(lhs) ? rhs : lhs, head);
}

void list_merge_sort(list_ele_t *head)
{
    if (list_is_singular(&head->list))
        return;

    list_ele_t left;
    struct list_head sorted;
    INIT_LIST_HEAD(&left.list);
    list_cut_position(&left.list, &head->list, &get_middle(&head->list)->list);
    list_merge_sort(&left);
    list_merge_sort(head);
    list_merge(&left.list, &head->list, &sorted);
    INIT_LIST_HEAD(&head->list);
    list_splice_tail(&sorted, &head->list);
}

static bool validate(list_ele_t *head)
{
    struct list_head *node;
    list_for_each (node, &head->list) {
        if (node->next == &head->list)
            break;
        if ((list_entry(node, list_ele_t, list)->value) >
                   list_entry(node->next, list_ele_t, list)->value)
            return false;
    }
    return true;
}

static list_ele_t *head_new()
{
    list_ele_t *head = malloc(sizeof(list_ele_t));
    if (!head) return NULL;
    head->value = 0;
    INIT_LIST_HEAD(&head->list);
    return head;
}

static void list_free(struct list_head *list)
{
    if (!list) return;

    struct list_head *current = list;
    while (current != list) {
        struct list_head *tmp = current;
        current = tmp->next;
        //free(list_entry(current, list_ele_t, list)->value);
        free(list_entry(tmp, list_ele_t, list));
    }
    free(list_entry(list, list_ele_t, list));
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
#endif // DDLIKEDLIST_H_INCLUDED
