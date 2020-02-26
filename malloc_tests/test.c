#include <stdlib.h>
#include <stdio.h>

int main(void)
{
  int *ptr = NULL;
  for(int i = 0; i < 4156; i++)
  {
    ptr = calloc(i, sizeof(int));
    for (int j = 0; j < i; j++)
    {
      if (ptr[j] != 0)
        printf("%d", ptr[j]);
    }
    if (i % 15 == 0)
      free(ptr);
  }
  return 0;
}
