#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "cilix_sap_seq.h"

sap_session_cap_t* sap_session_cap_malloc(sap_session_t* usn) {
    sap_session_cap_t* scap = (sap_session_cap_t*)malloc(sizeof(sap_session_cap_t)); {
        scap->sap_session = usn;
        sp_cps_init(&scap->spip_c);

        scap->spip_h = cilix_dlib_open(usn->spip);
        __SPIP_CALL(spip_pkg_create) {
            scap->spip_p = pf((sp_cp_t*)&scap->spip_c);
        }}
    } return scap;
}

void sap_session_cap_free(sap_session_cap_t* scap) {
    if (scap != 0) {
        __SPIP_CALL(spip_pkg_destroy) {
            pf(scap->spip_p);
        }}
        cilix_dlib_close(scap->spip_h);

        __SAPS_CALL(session_clean_cb) {
            sf(scap->sap_session);
        }}
    } free(scap);
}

sap_seq_t* sap_seq_malloc(sap_frame_t* pf, sap_session_t* pu, void* hs) {
    sap_seq_t* seq = (sap_seq_t*)malloc(sizeof(sap_seq_t)); {
        seq->sap_frame = pf;
        seq->u_session = sap_session_cap_malloc(pu);
        seq->close_invoked = 0;
        seq->hs = hs;

        memset(&seq->skcbs, 0, sizeof(cilix_skcb_t)); {
            seq->skcbs.s_n_cb = 0;
            seq->skcbs.s_e_cb = sap_seq_socket_connect_err_cb;
            seq->skcbs.s_r_cb = sap_seq_socket_read_cb;
            seq->skcbs.s_c_cb = sap_seq_socket_close_cb;
        }
        seq->client = cilix_connector_malloc(seq, pf, 0, &seq->skcbs);
        seq->tpp_sys = cilix_tpp_sys_init(seq, sap_seq_parse_data_sys_packet_cb);
    } return seq;
}

void sap_seq_free(sap_seq_t* seq) {
    if (seq != 0) {
        sap_session_cap_free(seq->u_session);
        cilix_connector_free(seq->client);
        cilix_tpp_sys_fini(seq->tpp_sys);
    } free(seq);
}

void sap_seq_start(sap_seq_t* seq) {
    cilix_connector_start(seq->client, SCAP->sap_session->ip, SCAP->sap_session->port);
}

void sap_seq_stop_wait(sap_seq_t* seq) {
    seq->close_invoked = 1;
    cilix_connector_stop(seq->client);
    cilix_connector_wait(seq->client);
}

void sap_seq_write_packet(sap_seq_t* seq, void* data, long datalen) {
    cilix_connector_write_system(seq->client, data, datalen, 0);
}

void sap_seq_socket_connect_err_cb(cilix_socket_router_t* router, int sys_err) {
    sap_seq_t* seq = (sap_seq_t*)router->u_session;

    seq->close_invoked = 1;
    SAPS_CALL(connect_err_cb) {
        sf(SCAP->sap_session, sys_err);
    }}
}

void sap_seq_socket_close_cb(cilix_socket_router_t* router) {
    sap_seq_t* seq = (sap_seq_t*)router->u_session;

    if (!seq->close_invoked) {
        SAPS_CALL(disconnect_cb) {
            sf(SCAP->sap_session);
        }}
    }
    sap_frame_putq_with_type(seq->sap_frame, SAP_FRAME_Q_TYPE_CLEAN_SESSION, seq, 0, 0);
}

void sap_seq_socket_read_cb(cilix_socket_router_t* router, void* base, long len) {
    sap_seq_t* seq = (sap_seq_t*)router->u_session;

    cilix_tpp_sys_parse(seq->tpp_sys, 0, (char*)base, len);
}

int sap_seq_parse_data_sys_packet_cb(void* pseq, void* pe, char* pkp, long pkpl, char* pks, long pksl, int pki) {
    sap_seq_t* seq = (sap_seq_t*)pseq;

    void* sap_rsp = 0;
    SPIP_CALL(spip_rsp_packet) {
        sap_rsp = pf(SCAP->spip_p, pks, pksl);
    }}
    SAPS_CALL(response_cb) {
        sf(SCAP->sap_session, sap_rsp);
    }}
    SPIP_CALL(spip_rsp_free) {
        pf(SCAP->spip_p, sap_rsp);
    }}

    return 0;
}
