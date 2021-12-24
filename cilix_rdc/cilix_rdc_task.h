#ifndef _CILIX_RDC_TASK_H_
#define _CILIX_RDC_TASK_H_

#include "cilix_task.h"
#include "cilix_socket.h"
#include "cilix_rdc.h"

#define	RDC_TASK_Q_TYPE_REQ     (1)

typedef struct rdc_task_q_s {
    int type;
    cilix_socket_router_t router;

    char func[256];
    int ret;
    void* in;
    void* out;
} rdc_task_q_t;

rdc_task_q_t* rdc_task_q_malloc(int ptype, cilix_socket_router_t* pr, char* func, char* pdata, long plen);
void rdc_task_q_free(rdc_task_q_t* fq);

void rdc_task_init_cb(cilix_task_t* task);
void rdc_task_fini_cb(cilix_task_t* task);
void rdc_task_q_get_cb(cilix_task_t* task, void* udata, void* w, cilix_task_q_done_cb qdcb, void* sem);
void rdc_task_q_clean_cb(cilix_task_t* task, void* udata);

#endif // #ifndef _CILIX_RDC_TASK_H_
