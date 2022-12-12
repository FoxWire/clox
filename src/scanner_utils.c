#include "common.h"

static bool is_whitespace(const char character){
  return (character == ' '
      || character == '\t'
      || character == '\r'
      || character == '\n');
}

static bool is_numeric(const char character){
  return (character >= '0' && character <= '9');
}

static bool is_alpha(const char character){
  return (character >= 'a' && character <= 'z'
      || character >= 'A' && character <= 'Z');
}



