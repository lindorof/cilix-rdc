#ifndef _CILIX_SAP_SEQ_H_
#define _CILIX_SAP_SEQ_H_

#include "cilix_task.h"
#include "cilix_dlib.h"
#include "cilix_socket.h"
#include "cilix_tpp_sys.h"

#include "cilix_sp_ddk.h"
#include "cilix_sp_cps.h"

#include "cilix_sap_frame.h"

typedef struct sap_session_cap_s {
    sap_session_t* sap_session;
    sp_cps_t spip_c;

    void* spip_h;
    void* spip_p;
} sap_session_cap_t;

typedef struct sap_seq_s {
    sap_frame_t* sap_frame;
    sap_session_cap_t* u_session;
    int close_invoked;
    void* hs;

    cilix_skcb_t skcbs;
    cilix_connector_t* client;
    cilix_tpp_sys_t* tpp_sys;
} sap_seq_t;

#define SCAP (seq->u_session)

#define __SPIP_CALL(sym)                                            \
    {                                                               \
        ___##sym pf = (___##sym)cilix_dlib_sym(scap->spip_h, #sym); \
        if (pf)                                                     \

#define SPIP_CALL(sym)                                              \
    if (seq) {                                                      \
        ___##sym pf = (___##sym)cilix_dlib_sym(SCAP->spip_h, #sym); \
        if (pf)                                                     \

#define __SAPS_CALL(sym)                                            \
    {                                                               \
        sap_##sym sf = 0;                                           \
        if (scap->sap_session && (sf = scap->sap_session->sym))     \

#define SAPS_CALL(sym)                                              \
    if (seq) {                                                      \
        sap_##sym sf = 0;                                           \
        if (SCAP->sap_session && (sf = SCAP->sap_session->sym))     \

// *** public - start ***
sap_seq_t* sap_seq_malloc(sap_frame_t* pf, sap_session_t* pu, void* hs);
void sap_seq_start(sap_seq_t* seq);
void sap_seq_stop_wait(sap_seq_t* seq);
void sap_seq_free(sap_seq_t* seq);
void sap_seq_write_packet(sap_seq_t* seq, void* data, long datalen);
// *** public - end ***

// *** session-cb - start ***
void sap_seq_socket_connect_err_cb(cilix_socket_router_t* router, int sys_err);
void sap_seq_socket_read_cb(cilix_socket_router_t* router, void* base, long len);
void sap_seq_socket_close_cb(cilix_socket_router_t* router);
// *** session-cb - end ***

int sap_seq_parse_data_sys_packet_cb(void* seq, void* pe, char* pkp, long pkpl, char* pks, long pksl, int pki);

#endif // #ifndef _CILIX_SAP_SEQ_H_
