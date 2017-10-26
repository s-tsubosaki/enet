/** 
 @file  callbacks.h
 @brief ENet callbacks
*/
#ifndef __ENET_CALLBACKS_H__
#define __ENET_CALLBACKS_H__

#include <stdlib.h>

/** @defgroup callbacks ENet internal callbacks
    @{
    @ingroup private
*/
extern void *enet_malloc(size_t);
extern void enet_free(void *);

/** @} */

#endif /* __ENET_CALLBACKS_H__ */
