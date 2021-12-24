#include <stdio.h>
#include <stdlib.h>

#include "uv.h"

#include "cilix_thread.h"

typedef struct cilix_thread_s {
    uv_thread_t tid;
} cilix_thread_t;

void* cilix_thread_start(void* func, void* arg) {
    cilix_thread_t* t = (cilix_thread_t*)malloc(sizeof(cilix_thread_t)); {
        uv_thread_create(&t->tid, (uv_thread_cb)func, arg);
    }
    return t;
}

void cilix_thread_wait(void* pt) {
    if (pt == 0) return;

    cilix_thread_t* t = (cilix_thread_t*)pt; {
        uv_thread_join(&t->tid);
    }
}

void cilix_thread_destroy(void* pt) {
    if (pt == 0) return;

    cilix_thread_t* t = (cilix_thread_t*)pt; {
    }
    free(t);
}
