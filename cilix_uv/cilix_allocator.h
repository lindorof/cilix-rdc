#ifndef _CILIX_ALLOCATOR_H_
#define _CILIX_ALLOCATOR_H_

#include "queue.h"

// *** public - start ***
void* cilix_alloc_buf(unsigned long len);
void* cilix_alloc_more(unsigned long len, void* po);
void cilix_alloc_fini(void* po);
// *** public - end ***

typedef struct cilix_alloc_s {
    QUEUE* queue;
    QUEUE node;
} cilix_alloc_t;

#endif // #ifndef _CILIX_ALLOCATOR_H_
