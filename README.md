Heap
====

A thread-safe implementation of generic heaps in C.

Technically the implementation is a max-heap, but the constructor takes a comparison function that lets you sort them in anyway you see fit.

heap.h
------
| Signature | Description |
| --------- | ----------- |
| `heap * make_heap(int maxsize, int (*compare)(void *, void*))` | Creates a new heap with a maximum size of maxsize and a function to compare the elements. The heap is a max heap, so for two elements, a and b, if compare(a,b) > 0 then a will be deleted from the heap before b. |
| `int heap_maxsize(heap * h)` | Returns the maximum number of elements the heap can hold. If it is equal to the actual size of the heap, then it will have to be resized before it can hold more elements. | 
| `int heap_size(heap * h)` | Returns the number of elements the heap currently holds. heap_size(h) ≤ heap_maxsize(h). |
| `heap * resize_heap(heap * h, int new_maxsize)` | Assigns a new maximum size to the heap so that new elements can be added. If the new size is less than the old size or if there is not enough memory to resize it, the function will return NULL. If the function is succesful, it will return h. |
| `heap * insert(heap * h, void * item)` | Inserts a new element into the heap in accordance with the compare function and returns h. |
| `void * delete(heap * h)` | Deletes the top element of the heap and returns it. |
| `void for_each(heap * h, void (*f)(void *))` | Calls f on each element of h. |
| `void heap_sort(void * arr[], int size, int (*compare)(void *, void *))` | Sorts an array by creating a heap from the elements and then deleting items from it into the array again. |
| `void free_heap(heap * h)` | Deallocates the heap. Not thread safe. |
