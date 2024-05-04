#ifndef _ENGINE_HEAP_H_
#define _ENGINE_HEAP_H_

#include <ultratypes.h>

#include "engine/config.h"

extern u8 heap[HEAP_SIZE];

extern int InitHeap(void *start, unsigned int size);

#endif /* _ENGINE_HEAP_H_ */
