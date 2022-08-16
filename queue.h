/************************************************************************
 *
 * Copyright (C) 2021-2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 * 
 ************************************************************************/

/*********************************************************
 **               DO NOT MODIFY THIS FILE               ** 
 *********************************************************/

#pragma once 

#include <stdbool.h>

/**
 * Create a queue.
 * 
 * Returns NULL if memory allocation fails.
 */
void *queue_create();

/**
 * Destroys QUEUE previously created with queue_create().
 */
void queue_destroy(void *queue);

/**
 * Insert DATA at the tail of QUEUE.
 */
void queue_enqueue(void *queue, void *data);

/**
 * Remove and return the head item of QUEUE.
 * 
 * Returns NULL if queue_size() would return 0.
 */
void *queue_dequeue(void *queue);

/**
 * Return the head item of QUEUE.
 * 
 * Returns NULL if queue_size() would return 0.
 */
void *queue_head(void *queue);

/**
 * Remove DATA from QUEUE.
 * 
 * Returns DATA or NULL if DATA is not in QUEUE.
 */
void *queue_remove(void *queue, void *data);

/**
 * Returns the number of items in QUEUE.
 */
int queue_size(void *queue);

/**
 * Remove all items from QUEUE.
 */
void queue_clear(void *queue);

/**
 * Find the first element in QUEUE containing VALUE via EQUALITOR.
 * 
 * Returns NULL if none found.
 * 
 * Example:
 * 
 *     typedef struct {
 *       int a;
 *       int b;
 *     } inner_t;
 *
 *     typedef struct {
 *       inner_t *inner;
 *       int x;
 *       int y;
 *     } outer_t;
 *
 *     static bool inner_equalitor(void *outer, void *inner) {
 *       return ((outer_t*)outer)->inner == (inner_t*)inner;
 *     }
 * 
 *     int main(int argc char *argv[]) {
 *       outer_t inner = { 1, 2 };
 *       outer_t outer = { &inner, 3, 4 };
 *
 *       void *queue = queue_create();
 *       queue_enqueue(queue, &outer);
 *       outer_t *found = queue_find(queue, inner_equalitor, &inner);
 *     }
 */
void *queue_find(void *queue, bool (*equalitor)(void*,void*), void *value);

/**
 * Sort QUEUE according to COMPARATOR where COMPARATOR returns:
 * 
 *   < 1 if a is less than b
 *     0 if a and b are equal
 *   > 1 if a is greated than b
 * 
 * Example: ( given structs from queue_find() comments )
 * 
 *     static int inner_comparator(void *a, void *b) {
 *       return ((outer_t*)a)->inner->a - ((outer_t*)b)->inner->a;
 *     }
 * 
 *     int main(int argc char *argv[]) {
 *       outer_t inner1 = { 1, 2 };
 *       outer_t inner2 = { 3, 4 };
 *       outer_t outer1 = { &inner1, 5, 6 };
 *       outer_t outer2 = { &inner2, 7, 8 };
 *
 *       void *queue = queue_create();
 *       queue_enqueue(queue, &outer1);
 *       queue_enqueue(queue, &outer2);
 *       queue_sort(queue, inner_comparator);
 *     };
 * 
 */
void queue_sort(void *queue, int (*comparator)(void*,void*));

/**
 * Iterate over the contents of QUEUE starting at the head calling ITERATOR 
 * for each element found.
 * 
 * Example: ( given structs from queue_find() comments )
 * 
 *     static int outer_iterator(void *outer) {
 *       printf("%d\n", ((outer_t*)outer)->inner->a;
 *     }
 * 
 *     int main(int argc char *argv[]) {
 *       outer_t inner1 = { 1, 2 };
 *       outer_t outer1 = { &inner1, 5, 6 };
 *
 *       void *queue = queue_create();
 *       queue_enqueue(queue, &outer1);
 *       queue_iterate(queue, outer_iterator);
 *     };
 * 
 *  Would print '1' on the console.
 */
void queue_iterate(void *queue, void (*iterator)(void*));
