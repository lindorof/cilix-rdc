#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "cilix_queue.h"

void CILIX_QUEUE_INSERT(void* data, CILIX_QUEUE* que) {
    cilix_queue_t* cq = (cilix_queue_t*)malloc(sizeof(cilix_queue_t)); {
        cq->data = data;
        QUEUE_INIT(&cq->node);
    } QUEUE_INSERT_TAIL(que, &cq->node);
}

void CILIX_QUEUE_DESTROY(cilix_queue_t* fq) {
    if (fq == 0) return;
    free(fq);
}