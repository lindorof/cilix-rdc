#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "cilix_allocator.h"

#define CA_QUEUE_MEM(pq)                                                        \
    pq = (QUEUE*)malloc(sizeof(QUEUE));                                         \
    QUEUE_INIT(pq);                                                             \

#define CA_QUEUE_FREE(pq)                                                       \
    free(pq);                                                                   \

#define CA_INIT_MEM(mem, cat, len)                                              \
    void* mem = malloc(sizeof(cilix_alloc_t) + len);                            \
    memset(mem, 0, sizeof(cilix_alloc_t) + len);                                \
    cilix_alloc_t* cat = (cilix_alloc_t*)mem;                                   \
    QUEUE_INIT(&cat->node);                                                     \

#define CA_RET_MEM(mem)                                                         \
    return ((char*)mem + sizeof(cilix_alloc_t))                                 \

#define CA_PO_MQUEUE(po, mq)                                                    \
    QUEUE* mq = ((cilix_alloc_t*)((char*)po - sizeof(cilix_alloc_t)))->queue;   \

#define CA_FINI_NODE_MEM(head)                                                  \
    free(QUEUE_DATA(head, cilix_alloc_t, node))                                 \

void* cilix_alloc_buf(unsigned long len) {
    if (len <= 0) return 0;

    CA_INIT_MEM(mem, cat, len);
    CA_QUEUE_MEM(cat->queue);
    QUEUE_INSERT_TAIL(cat->queue, &cat->node);

    CA_RET_MEM(mem);
}

void* cilix_alloc_more(unsigned long len, void* po) {
    if (len <= 0) return 0;
	if (po == 0) return 0;

    CA_PO_MQUEUE(po, pq);
    if (pq == 0) return 0;

    CA_INIT_MEM(mem, cat, len);
    QUEUE_INSERT_TAIL(pq, &cat->node);

    CA_RET_MEM(mem);
}

void cilix_alloc_fini(void* po) {
    if (po == 0) return;

    CA_PO_MQUEUE(po, pq);
    if (pq == 0) return;

    while (!QUEUE_EMPTY(pq)) {
        QUEUE* head = QUEUE_HEAD(pq);
        QUEUE_REMOVE(head);
        CA_FINI_NODE_MEM(head);
    }

    CA_QUEUE_FREE(pq);
}