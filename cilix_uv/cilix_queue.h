#ifndef _CILIX_QUEUE_H_
#define _CILIX_QUEUE_H_

#include "queue.h"

typedef struct cilix_queue_s {
    void* data;
    QUEUE node;
} cilix_queue_t;

typedef QUEUE CILIX_QUEUE;

#define CILIX_QUEUE_INIT(que) QUEUE_INIT(que);

#define CILIX_QUEUE_FOREACH(t, d, que) { \
    QUEUE* q; \
    QUEUE_FOREACH(q, que) { \
        cilix_queue_t* fq = QUEUE_DATA(q, cilix_queue_t, node); \
        t d = (t)(long)fq->data; \

#define CILIX_QUEUE_LOOPOP(t, d, que) { \
    while (!QUEUE_EMPTY(que)) { \
        QUEUE* q = QUEUE_HEAD(que); \
        cilix_queue_t* fq = QUEUE_DATA(q, cilix_queue_t, node); \
        t d = (t)(long)fq->data; \

#define CILIX_QUEUE_REMOVE { \
    QUEUE_REMOVE(q); \
    CILIX_QUEUE_DESTROY(fq); } \

// public
void CILIX_QUEUE_INSERT(void* data, CILIX_QUEUE* que);
// private
void CILIX_QUEUE_DESTROY(cilix_queue_t* fq);

#endif // #ifndef _CILIX_QUEUE_H_
