#ifndef MALLOC_H
# define MALLOC_H

# include <stddef.h>

/*enum type_blk
{
  ALLOCATED,
  FREE
};*/

struct super
{
  struct super *next;
  size_t free_size;
};

struct meta_data
{
  char type;
  size_t size;
  struct meta_data *next;
};

void *malloc(size_t size);

#endif /* !MALLOC_H */
