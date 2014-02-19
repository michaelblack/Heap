#include <stdlib.h>
#include <pthread.h>
#include "heap.h"

struct heap {
  int maxsize; // The size of array
  pthread_mutex_t lock; // To be locked by applications that modify the queue
  int size; // The size of the heap / the position of the last element in array
  void ** array; // The internal representation of the heap
  int (*compare)(void *, void *); // The comparison function to sort the heap
                                  // >0 results means the first argument is greater
                                  // and will be sorted towards the top
};

// Makes a heap from a maximum internal size and a comparison function
/* The comparison takes two elements and returns -1 if the first is less
   than the second, 0 if they're equal, and 1 if it's greater.
   The heap uses the comparison function to sort greater items to the top.
*/
heap * make_heap(int maxsize, int (*compare)(void *, void *)) {
  heap * h = malloc(sizeof(heap));
  h->maxsize = maxsize;
  h->size = 0;
  h->array = malloc((maxsize+1) * sizeof(void *));
  h->array[0] = NULL;
  h->compare = compare;

  return h;
}

// Frees the heap
void free_heap(heap * h) {
  free(h->array);
  free(h);
}

// Inserts an item into the heap
heap * insert(heap * h, void * item) {
  pthread_mutex_lock(&(h->lock));
 
  // Puts item in the last position of the heap 
  h->size++;
  int position = h->size;
  h->array[position] = item;

  // While the item is greater than it's parent, swap them
  while(position > 1 &&
	(h->compare)(h->array[position], h->array[position/2]) > 0) {
    void * tmp = h->array[position];
    h->array[position] = h->array[position/2];
    h->array[position/2] = tmp;

    position /= 2;
  }


  pthread_mutex_unlock(&(h->lock));
  return h;
}

// Deletes the top item from the heap and returns it
void * delete(heap * h) {
  pthread_mutex_lock(&(h->lock));
  
  // Move last item to front
  void * deleted = h->array[1];
  h->array[1] = h->array[h->size];
  h->size--;

  int largest_child;
  int position = 1;

  //Swaps the last item with the first then sorts it down by swapping with the largest child
  while(position*2 <= h->size /* has child? */) {

    if(position*2+1 <= h->size && // has right child and it's bigger than the left?
       (*h->compare)(h->array[position*2], h->array[position*2+1]) <= 0) {
      largest_child = position*2 + 1; // the largest child is the right
    } 
    else {
      largest_child = position*2; // the largest child is on the left
    }
    
    if((*h->compare)(h->array[position], h->array[largest_child]) <=0) { // if the parent's smaller than the larger child
      void * temp = h->array[largest_child];
      h->array[largest_child] = h->array[position];                      // then swap them
      h->array[position] = temp;
    }

    position = largest_child; //repeat from the new position
  }


  pthread_mutex_unlock(&(h->lock));
  return deleted; //return the deleted element
}

// Takes a heap and a function and calls the function on each element of the heap (e.g. for_each(myheap, &print_int))
void for_each(heap * h, void(*foo)(void *)) {
  pthread_mutex_lock(&(h->lock));

  for(int i = 1; i <= h->size; i++)
    foo(h->array[i]);
  
  pthread_mutex_unlock(&(h->lock));
}

// Sorts an array by putting it into a heap and then removing each item in order back into the original array
// Not an in place sort
void heap_sort(void * arr[], int size, int (*compare)(void *, void *)) {
  heap * h = make_heap(size, compare);
  for(int i = 0; i < size; i++)
    insert(h, arr[i]);

  for(int i = 0; i < size; i++)
    arr[i] = delete(h);

  free_heap(h);
}

// Returns the size of a heap
int heap_size(heap * h) {
  return h->size;
}

// Returns the maximum internal size of the heap
int heap_maxsize(heap * h) {
  return h->maxsize;
}

// Resizes the heap to a larger max internal size. If it's smaller than before, the function returns null.
heap * resize_heap(heap * h, int new_maxsize) {
  pthread_mutex_lock(&(h->lock));
  if (new_maxsize < heap_maxsize(h)) {
    return NULL;
  }
  if (new_maxsize == heap_maxsize(h)) {
    return h;
  }

  void ** ptr = realloc(h->arr, (new_maxsize + 1) * sizeof(void *));

  if(!ptr) {
    return NULL;
  }

  h->arr = ptr;
  pthread_mutex_unlock(&(h->lock));
  return h;
}

// Returns the comparison function used in the heap
int (*heap_compare(heap * h)) (void *, void *) {
  return h->compare;
}
