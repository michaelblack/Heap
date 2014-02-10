#ifndef HEAP_H
#define HEAP_H


typedef struct heap heap;

heap * make_heap(int maxsize, int (*compare)(void *, void *));

heap * insert(heap * h, void * item);
void * delete(heap * h);
    
void for_each(heap * h);
void free_heap(heap * h);

int heap_size(heap * h);
int heap_maxsize(heap * h);
int (*heap_compare(heap * h)) (void *, void *);

heap * resize_heap(heap * h, int new_maxsize);

void heap_sort(void * arr[], int size, int (*compare)(void *, void *));



#endif
