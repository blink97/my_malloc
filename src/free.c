#include "malloc.h"
#include "free.h"

static void *cast_blk(void *ptr)
{
  char *tmp1 = ptr;
  void *tmp2 = tmp1 - sizeof(struct meta_data);
  return tmp2;
}

void free(void *ptr)
{
  if (!ptr)
    return;
  struct meta_data *meta = cast_blk(ptr);
  meta->type = 0;
  if (meta->next)
  {
    if (meta->next->type == 0)
    {
      meta->size += meta->next->size + sizeof(struct meta_data);
      meta->next = meta->next->next;
    }
  }
}
