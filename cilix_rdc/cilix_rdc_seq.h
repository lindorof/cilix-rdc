#ifndef _CILIX_RDC_SEQ_H_
#define _CILIX_RDC_SEQ_H_

#include "cilix_socket.h"
#include "cilix_tpp_sys.h"

typedef struct rdc_seq_s {
    cilix_socket_router_t router;
    cilix_tpp_sys_t* tpp_sys;
} rdc_seq_t;

// *** session-cb - start ***
void rdc_seq_socket_new_cb(cilix_socket_router_t* router);
void rdc_seq_socket_read_cb(cilix_socket_router_t* router, void* base, long len);
void rdc_seq_socket_close_cb(cilix_socket_router_t* router);
// *** session-cb - end ***

int rdc_seq_parse_data_sys_packet_cb(void* seq, void* pe, char* pkp, long pkpl, char* pks, long pksl, int pki);

#endif // #ifndef _CILIX_RDC_SEQ_H_
