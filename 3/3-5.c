#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

enum
{
    STR_VAL_SIZE = 20,
    BASE = 10
};

struct Elem
{
    struct Elem *next;
    char *str;
};

int 
check_str(char *str)
{
    char *eptr;
    errno = 0;
    long lval = strtol(str, &eptr, BASE);
    if (errno || *eptr || eptr == str || (int) lval != lval) {
        return INT_MAX;
    }
    return lval;
}

struct Elem *
check_val_and_create(struct Elem *current) {
    int value = check_str(current->str);
    if (value != INT_MAX) {
        char *add_str = calloc(STR_VAL_SIZE, sizeof(*add_str));
        if (snprintf(add_str, STR_VAL_SIZE, "%d", value + 1) == -1) {
            exit(1);
        }
        struct Elem *add_elem = calloc(1, sizeof(*add_elem));
        add_elem->str = add_str;
        add_elem->next = current;
        return add_elem;
    }
    return NULL;
}

struct Elem *
dup_elem(struct Elem *head) 
{
    if (head == NULL) {
        return head;
    }
    struct Elem *current = head->next;
    struct Elem *prev = head;
    struct Elem *add_elem;
    if ((add_elem = check_val_and_create(head)) != NULL) {
        head = add_elem;
    }
    while (current != NULL) {
        if ((add_elem = check_val_and_create(current)) != NULL) {
            prev->next = add_elem;
        }
        prev = current;
        current = prev->next;
    }
    return head;
}