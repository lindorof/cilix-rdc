#ifndef _CILIX_TIMEOUT_H_
#define _CILIX_TIMEOUT_H_

#include <stdio.h>
#include <stdlib.h>

#include "uv.h"

typedef struct cilix_timeout_s cilix_timeout_t;

// *** public - start ***
void* cilix_timeout_init(int timeout);
void cilix_timeout_destroy(void* tmt);
int cilix_timeout_flag(void* tmt);
void cilix_timeout_sleep(int timeout);
// *** public - end ***

void cilix_timeout_thr(void* arg);
void cilix_timeout_async_cb(uv_async_t* handle);
void cilix_timeout_timer_cb(uv_timer_t* handle);

struct cilix_timeout_s {
    uint64_t timeout;

    uv_loop_t loop;
    uv_async_t iasync;
    uv_timer_t timer;
    uv_thread_t t_id;

    int flag;
    int loop_t_stop;
};

#endif // #ifndef _CILIX_TIMEOUT_H_
