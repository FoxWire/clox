#include <stdio.h>

#include "memory.h"
#include "value.h"

void ValueArray_init(ValueArray *array){
  array->capacity = 0;
  array->count = 0;
  array->values = NULL;
}

void ValueArray_write_value(ValueArray *array, Value value){
  if (array->capacity < array->count + 1){
    int old_capacity = array->capacity;
    array->capacity = GROW_CAPACITY(old_capacity);
    array->values = GROW_ARRAY(Value,array->values, old_capacity, array->capacity);
  }

  array->values[array->count] = value;
  array->count++;
}

void ValueArray_free(ValueArray *array){
  FREE_ARRAY(Value, array->values, array->capacity);
  ValueArray_init(array);
}

void ValueArray_print_value(Value value){
  printf("%g", value);
}
