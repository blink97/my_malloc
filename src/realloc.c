#include <string.h>

#include "malloc.h"
#include "free.h"

static void *castblk(void *ptr)
{
  char *tmp1 = ptr;
  void *tmp2 = tmp1 - sizeof(struct meta_data);
  return tmp2;
}

void *realloc(void *ptr, size_t size)
{
  if (!ptr)
    return malloc(size);
  if (size == 0)
  {
    free(ptr);
    return NULL;
  }
  void *res = malloc(size);
  struct meta_data *data = castblk(ptr);
  if (data->size < size)
    size = data->size;
  res = memcpy(res, ptr, size);
  free(ptr);
  return res;
}
