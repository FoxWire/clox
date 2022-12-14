#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef double Value;

typedef struct {
  int capacity;
  int count;
  Value *values;
} ValueArray;

void ValueArray_init(ValueArray *array);
void ValueArray_write_value(ValueArray *array, Value value);
void ValueArray_free(ValueArray *array);
void ValueArray_print_value(Value value);

#endif
