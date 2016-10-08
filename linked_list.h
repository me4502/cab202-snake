#pragma once
#include <stdlib.h>

/**
 * An entry in a LinkedList structure.
 */
typedef struct entry {
    void * value;
    struct entry * next;
} LinkedListEntry;

/**
 * Pushes a value to the end of the LinkedList structure.
 *
 * @param list The head of the list
 * @param value The value to push
 */
void push_to_back(LinkedListEntry * list, void * value);

/**
 * Pushes the value to the front of the LinkedList structure.
 *
 * @param list The list reference
 * @param value The value to push
 */
void push(LinkedListEntry ** list, void * value);

/**
 * Pops the value at the back of the LinkedList structure.
 *
 * @param list The head of the list
 * @return The popped value
 */
void * pop_from_back(LinkedListEntry * list);

/**
 * Pops the value at the front of the LinkedList structure.
 *
 * @param list The list reference
 * @return The popped value
 */
void * pop(LinkedListEntry ** list);

/**
 * Gets the length of the LinkedList structure.
 *
 * @param list The head of the list
 * @return The length
 */
int length(LinkedListEntry * list);