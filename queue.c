#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;

    list_ele_t *tmp = NULL;
    while (q->head) {
        tmp = q->head;
        q->head = tmp->next;
        free(tmp->value);
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh;

    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    newh->value = malloc(strlen(s) + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }

    strncpy(newh->value, s, strlen(s));
    *(newh->value + strlen(s)) = '\0';

    newh->next = q->head;
    q->head = newh;

    if (q->size == 0) {
        q->tail = newh;
    }
    q->size += 1;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }

    list_ele_t *node;
    node = malloc(sizeof(list_ele_t));

    if (!node) {
        return false;
    }
    node->value = malloc(strlen(s) + 1);
    node->next = NULL;

    if (!node->value) {
        free(node);
        return false;
    }

    strncpy(node->value, s, strlen(s));
    *(node->value + strlen(s)) = '\0';

    if (q->size == 0) {
        q->head = node;
    } else {
        q->tail->next = node;
    }
    q->tail = node;
    q->size += 1;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) {
        return false;
    }

    list_ele_t *tmp = q->head;

    size_t tmplen = strlen(tmp->value);
    size_t cpylen = tmplen < bufsize - 1 ? tmplen : bufsize - 1;

    if (sp) {
        strncpy(sp, tmp->value, cpylen);
        *(sp + cpylen) = '\0';
    }

    q->head = tmp->next;
    q->size -= 1;

    free(tmp->value);
    free(tmp);

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head)
        return;

    list_ele_t *pre = q->head;
    list_ele_t *next = q->head->next;
    q->tail = q->head;
    q->tail->next = NULL;

    while (next) {
        q->head = next;
        next = next->next;
        q->head->next = pre;
        pre = q->head;
    }
}

static list_ele_t *merge(list_ele_t *left, list_ele_t *right)
{
    list_ele_t *head = NULL;

    for (list_ele_t **iter = &head; true; iter = &((*iter)->next)) {
        if (!left) {
            *iter = right;
            break;
        }
        if (!right) {
            *iter = left;
            break;
        }
        if (strcmp(left->value, right->value) < 0) {
            *iter = left;
            left = left->next;
        } else {
            *iter = right;
            right = right->next;
        }
    }

    return head;
}
static list_ele_t *mergeSort(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    list_ele_t *slow = head, *fast;

    for (fast = head->next; fast && fast->next; fast = fast->next->next) {
        slow = slow->next;
    }
    list_ele_t *mid = slow->next;
    slow->next = NULL;

    return merge(mergeSort(head), mergeSort(mid));
}
/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head)
        return;
    q->head = mergeSort(q->head);

    for (; q->tail->next; q->tail = q->tail->next)
        ;
}
