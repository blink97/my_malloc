#include <sys/mman.h>
#include <stdio.h>
#include <stdint.h>

#include "malloc.h"

#define SIZE_PAGE 4096

static struct super *super = NULL;

static void *cast_void(struct super *super)
{
  return super;
}

static void *cast_next(struct meta_data *meta, size_t size)
{
  void *tmp1 = meta + 1;
  char *tmp2 = tmp1;
  tmp2 += size;
  return tmp2;
}

static void split_data(size_t size, struct meta_data *meta)
{
  if (meta->size < size + 16 + sizeof(struct meta_data))//check for *2
    return;
  struct meta_data *new_meta = cast_next(meta, size);
  new_meta->type = 0;
  new_meta->size = meta->size - size - sizeof(struct meta_data);
  new_meta->next = meta->next;
  meta->next = new_meta;

  meta->size = size;
}

void *malloc(size_t size)
{
  if (size == 0)
    return NULL;
  size_t align = sizeof(size_t) - (size % sizeof(size_t));
  if (align < sizeof(size_t))
    size = size + align;
  struct super *curr = super;
  while (curr && curr->free_size >= size)
  {
    struct meta_data *meta = cast_void(curr + 1);
    while(meta)
    {
      if (meta->size >= size && meta->type == 0)
      {
        split_data(size, meta);
        curr->free_size -= size;
        meta->type = 1;
        return meta + 1;
      }
      meta = meta->next;
    }
    curr = curr->next;
  }
  size_t multi = 1;
  if (size + sizeof(struct super) + 2 * sizeof(struct meta_data) >= SIZE_PAGE)
  {
    multi += (size + sizeof(struct super) + 2 * sizeof(struct meta_data))
            / SIZE_PAGE;
    curr = mmap(NULL, multi * SIZE_PAGE, PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  }
  else
    curr = mmap(NULL, SIZE_PAGE, PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (curr == MAP_FAILED)
    return NULL;
  curr->next = super;
  curr->free_size = multi * SIZE_PAGE;
  super = curr;
  struct meta_data *meta = cast_void(curr + 1);
  meta->type = 1;
  meta->size = multi * SIZE_PAGE - sizeof(struct super)
              - sizeof(struct meta_data);
  meta->next = NULL;

  split_data(size, meta);
  curr->free_size -= size + sizeof(struct meta_data);
  return meta + 1;
}
