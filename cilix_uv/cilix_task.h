#ifndef _CILIX_TASK_H_
#define _CILIX_TASK_H_

#include <stdio.h>
#include <stdlib.h>

#include "uv.h"
#include "queue.h"

#define CILIX_TASK_GETQ_RET_OK          (0)
#define CILIX_TASK_GETQ_RET_STOP        (1)
#define CILIX_TASK_GETQ_RET_EMPTY_EXIT  (2)

typedef struct cilix_task_s cilix_task_t;
typedef struct cilix_task_q_s cilix_task_q_t;
typedef void (*cilix_task_init_cb)(cilix_task_t* task);
typedef void (*cilix_task_fini_cb)(cilix_task_t* task);
typedef void (*cilix_task_q_done_cb)(void* sem);
typedef void (*cilix_task_q_get_cb)(cilix_task_t* task, void* udata, void* w, cilix_task_q_done_cb qdcb, void* sem);
typedef void (*cilix_task_q_clean_cb)(cilix_task_t* task, void* udata);

// *** public - start ***
cilix_task_t* cilix_task_malloc(
    void* udata, 
    cilix_task_init_cb uicb, cilix_task_fini_cb ufcb,
    cilix_task_q_get_cb uqgcb, cilix_task_q_clean_cb uqccb);
void cilix_task_free(cilix_task_t* task);
void cilix_task_start(cilix_task_t* task);
void cilix_task_stop(cilix_task_t* task);
void cilix_task_empty_exit(cilix_task_t* task);
void cilix_task_wait(cilix_task_t* task);
void cilix_task_putq(cilix_task_t* task, void* udata, int wait, void* w);
// *** public - end ***

struct cilix_task_q_s {
    void* u_data;

    void* w;
    void* sem;

    QUEUE node;
};

struct cilix_task_s {
    void* u_data;
    cilix_task_init_cb u_init_cb;
    cilix_task_fini_cb u_fini_cb;
    cilix_task_q_get_cb u_q_get_cb;
    cilix_task_q_clean_cb u_q_clean_cb;

    uv_mutex_t mtx;
    uv_cond_t cond;

    int run;
    int empty_exit;
    uv_thread_t t_id;

    QUEUE qqueue;
};

#endif // #ifndef _CILIX_TASK_H_
