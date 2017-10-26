/** 
 @file callbacks.c
 @brief ENet callback functions
*/
#define ENET_BUILDING_LIB 1
#include "enet/enet.h"

void *
enet_malloc(size_t size)
{
  void *memory = malloc(size);

  if (memory == NULL)
    abort();

  return memory;
}

void enet_free(void *memory)
{
  free(memory);
}
