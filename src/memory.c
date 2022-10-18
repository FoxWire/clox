#include <stdlib.h>

#include "memory.h"

void* reallocate(void *pointer, size_t old_size, size_t new_size){
  if (new_size == 0){
    free(pointer);
    return NULL;
  }

void *allocated = realloc(pointer, new_size);
  if (allocated == NULL){
     exit(1);
  }
  return allocated;
}

