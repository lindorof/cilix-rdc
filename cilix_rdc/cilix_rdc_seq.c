#include <stdio.h>

#include "cilix_rdc.h"
#include "cilix_rdc_task.h"
#include "cilix_rdc_seq.h"

void rdc_seq_socket_new_cb(cilix_socket_router_t* router) {
    rdc_seq_t* seq = (rdc_seq_t*)malloc(sizeof(rdc_seq_t)); {
        memcpy(&seq->router, router, sizeof(cilix_socket_router_t));
        seq->tpp_sys = cilix_tpp_sys_init(seq, rdc_seq_parse_data_sys_packet_cb);
    } router->u_session = seq;
}

void rdc_seq_socket_close_cb(cilix_socket_router_t* router) {
    rdc_seq_t* seq = (rdc_seq_t*)router->u_session;
    if (seq != 0) {
        cilix_tpp_sys_fini(seq->tpp_sys);
        free(seq);
    } router->u_session = 0;
}

void rdc_seq_socket_read_cb(cilix_socket_router_t* router, void* base, long len) {
    rdc_seq_t* seq = (rdc_seq_t*)router->u_session;
    cilix_tpp_sys_parse(seq->tpp_sys, 0, (char*)base, len);
}

int rdc_seq_parse_data_sys_packet_cb(void* pseq, void* pe, char* pkp, long pkpl, char* pks, long pksl, int pki) {
    rdc_seq_t* seq = (rdc_seq_t*)pseq;
    rdc_t* rdc = (rdc_t*)seq->router.owner_context;

    char func[256] = "";
    memcpy(func, pkp, pkpl);

    rdc_task_q_t* q = rdc_task_q_malloc(RDC_TASK_Q_TYPE_REQ, &seq->router, func, pks, pksl);
    cilix_task_putq(rdc->task, q, 0, 0);

    return 0;
}
