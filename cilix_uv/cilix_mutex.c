#include <stdio.h>
#include <stdlib.h>

#include "uv.h"

#include "cilix_mutex.h"

typedef struct cilix_mutex_s {
    uv_mutex_t mtx;
} cilix_mutex_t;

void* cilix_mutex_init(void) {
    cilix_mutex_t* cmt = (cilix_mutex_t*)malloc(sizeof(cilix_mutex_t)); {
        uv_mutex_init(&cmt->mtx);
    }
    return cmt;
}

void cilix_mutex_destroy(void* pmt) {
    if (pmt == 0) return;

    cilix_mutex_t* cmt = (cilix_mutex_t*)pmt; {
        uv_mutex_destroy(&cmt->mtx);
    }
    free(cmt);
}

void cilix_mutex_lock(void* pmt) {
    if (pmt == 0) return;

    cilix_mutex_t* cmt = (cilix_mutex_t*)pmt; {
        uv_mutex_lock(&cmt->mtx);
    }
}

int cilix_mutex_trylock(void* pmt) {
    if (pmt == 0) return 0;

    cilix_mutex_t* cmt = (cilix_mutex_t*)pmt; {
        return uv_mutex_trylock(&cmt->mtx);
    }
}

void cilix_mutex_unlock(void* pmt) {
    if (pmt == 0) return;

    cilix_mutex_t* cmt = (cilix_mutex_t*)pmt; {
        uv_mutex_unlock(&cmt->mtx);
    }
}
