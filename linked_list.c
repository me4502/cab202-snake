#include "linked_list.h"

void push_to_back(LinkedListEntry * list, void * value) {
    LinkedListEntry * back = list;
    while (back->next != NULL) {
        back = back->next;
    }

    back->next = malloc(sizeof(LinkedListEntry));
    back->next->value = value;
    back->next->next = NULL;
}

void push(LinkedListEntry ** list, void * value) {
    LinkedListEntry * new_entry;
    new_entry = malloc(sizeof(LinkedListEntry));
    new_entry->value = value;
    new_entry->next = *list;
    *list = new_entry;
}

void * pop_from_back(LinkedListEntry * list) {
    if (list->next == NULL) {
        void * value = list->value;
        free(list);
        return value;
    }

    LinkedListEntry * back = list;
    while (back->next->next != NULL) {
        back = back->next;
    }

    void * value = back->next->value;
    free(back->next);
    back->next = NULL;
    return value;
}

void * pop(LinkedListEntry ** list) {
    if (*list == NULL) {
        return NULL; //Well this list is empty...
    }

    LinkedListEntry * old_head = *list;
    void * value = old_head->value;
    LinkedListEntry * new_head = old_head->next;
    free(old_head);
    *list = new_head;

    return value;
}

int length(LinkedListEntry * list) {
    if (list == NULL) {
        return 0;
    }

    int length = 0;
    LinkedListEntry * current = list;
    while (current->next != NULL) {
        current = current->next;
        length ++;
    }

    return length;
}