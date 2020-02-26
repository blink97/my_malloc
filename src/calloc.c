#include <string.h>

#include "malloc.h"

void *calloc(size_t number, size_t size)
{
  if (number == 0 || size == 0)
    return NULL;
  size *= number;
  size_t align = sizeof(size_t) - (size % sizeof(size_t));
  if (align < sizeof(size_t))
    size += align;
  void *res = malloc(size);
  res = memset(res, 0, size);
  return res;
}
