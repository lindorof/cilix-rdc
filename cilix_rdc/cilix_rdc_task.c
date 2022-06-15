#include <stdio.h>
#include <stdlib.h>

#include "cilix_sp_ddk.h"
#include "cilix_rdc_task.h"

#define DLIB_CALL(sym, str)                                         \
    {                                                               \
        ___##sym df = (___##sym)cilix_dlib_sym(rdc->dlib_h, str);   \
        if (df)                                                     \

rdc_task_q_t* rdc_task_q_malloc(int ptype, cilix_socket_router_t* pr, char* func, char* pdata, long plen) {
    rdc_task_q_t* q = (rdc_task_q_t*)malloc(sizeof(rdc_task_q_t));

    q->type = ptype;
    memcpy(&q->router, pr, sizeof(cilix_socket_router_t));

    strcpy(q->func, func);
    q->ret = 0;
    q->in = cilix_json_init(pdata, plen);
    q->out = cilix_json_init(0, 0);

    return q;
}

void rdc_task_q_free(rdc_task_q_t* fq) {
    if (fq != 0) {
        cilix_json_destroy(fq->in);
        cilix_json_destroy(fq->out);
    } free(fq);
}

void rdc_task_init_cb(cilix_task_t* task) {
    rdc_t* rdc = (rdc_t*)task->u_data;

    rdc->dlib_h = cilix_dlib_open_aeps(rdc->dlib_name);
    DLIB_CALL(dlib_init, DLIB_INIT) {
        df(rdc->dlib_phy, (sp_cp_t*)&rdc->dlib_cp);
    }}
}

void rdc_task_fini_cb(cilix_task_t* task) {
    rdc_t* rdc = (rdc_t*)task->u_data;

    DLIB_CALL(dlib_fini, DLIB_FINI) {
        df();
    }}
    cilix_dlib_close(rdc->dlib_h);
}

void rdc_task_q_get_cb(cilix_task_t* task, void* udata, void* w, cilix_task_q_done_cb qdcb, void* sem) {
    rdc_t* rdc = (rdc_t*)task->u_data;
    rdc_task_q_t* q = (rdc_task_q_t*)udata;
    qdcb(sem);

    if (q->type == RDC_TASK_Q_TYPE_REQ) {
        // call : func,in,out,ret
		DLIB_CALL(dlib_invoke, DLIB_INVOKE) {
			q->ret = df(q->func, q->in, q->out);
        } else { q->ret = SP_RET_ERR_DLIB_FUNC_NOT_EXIST; } }

        // print : out
        char* data = cilix_json_print(q->out);
        unsigned long datalen = strlen(data?data:"");

        // itoa : ret
        char sret[100] = "";
        sprintf(sret, "%d", q->ret);

        // acp_write_sys
        cilix_acceptor_write_system(&q->router, data, datalen, sret);

        // print_free
        cilix_json_print_free(data);
    }

    rdc_task_q_free(q);
}

void rdc_task_q_clean_cb(cilix_task_t* task, void* udata) {
    rdc_task_q_free((rdc_task_q_t*)udata);
}
