#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "cilix_sp_ddk.h"

#include "cilix_sap.h"
#include "cilix_sap_req.h"
#include "cilix_sap_seq.h"
#include "cilix_sap_frame.h"

sap_frame_q_t* sap_frame_q_malloc(SAP_FRAME_Q_TYPE pt, void* pq) {
    sap_frame_q_t* frameq = (sap_frame_q_t*)malloc(sizeof(sap_frame_q_t)); {
        frameq->type = pt;
        frameq->q = pq;
    } return frameq;
}

void sap_frame_q_free(sap_frame_q_t* frameq) {
    free(frameq);
}

void sap_frame_putq_with_type(sap_frame_t* frame, SAP_FRAME_Q_TYPE type, void* q, int wait, void* w) {
    sap_frame_q_t* frameq = sap_frame_q_malloc(type, q);
    cilix_task_putq(frame->task, frameq, wait, w);
}

sap_frame_t* sap_frame_malloc_start(void) {
    sap_frame_t* frame = (sap_frame_t*)malloc(sizeof(sap_frame_t)); {
        sp_cps_init(&frame->sp_cps);
        CILIX_QUEUE_INIT(&frame->snqueue);

        frame->task = cilix_task_malloc(frame, 0, 0, sap_frame_q_get_cb, sap_frame_q_clean_cb);
        cilix_task_start(frame->task);
    } return frame;
}

void sap_frame_stop_free(sap_frame_t* frame) {
    if (frame == 0) return;

    sap_frame_putq_with_type(frame, SAP_FRAME_Q_TYPE_STOP_ALL, 0, 0, 0);
    cilix_task_empty_exit(frame->task);
    cilix_task_wait(frame->task);
    cilix_task_free(frame->task);

    CILIX_QUEUE_LOOPOP(sap_seq_t*, seq, &frame->snqueue) {
        CILIX_QUEUE_REMOVE;
        sap_seq_free(seq);
    }}}

    free(frame);
}

void sap_frame_q_get_cb(cilix_task_t* task, void* udata, void* w, cilix_task_q_done_cb qdcb, void* sem) {
    sap_frame_t* frame = (sap_frame_t*)task->u_data;
    sap_frame_q_t* frameq = (sap_frame_q_t*)udata;

    if (frameq->type == SAP_FRAME_Q_TYPE_STOP_ALL) {
        CILIX_QUEUE_FOREACH(sap_seq_t*, seq, &frame->snqueue) {
            sap_seq_stop_wait(seq);
        }}} qdcb(sem);
    }

    if (frameq->type == SAP_FRAME_Q_TYPE_CLOSE_SESSION) {
        CILIX_QUEUE_FOREACH(sap_seq_t*, seq, &frame->snqueue) {
            if (seq->hs == frameq->q) {
                sap_seq_stop_wait(seq);
                break;
            }
        }}} qdcb(sem);
    }

    if (frameq->type == SAP_FRAME_Q_TYPE_CLEAN_SESSION) {
        CILIX_QUEUE_FOREACH(sap_seq_t*, seq, &frame->snqueue) {
            if (seq == frameq->q) {
                CILIX_QUEUE_REMOVE;
                sap_seq_stop_wait(seq);
                sap_seq_free(seq);
                break;
            }
        }}} qdcb(sem);
    }

    if (frameq->type == SAP_FRAME_Q_TYPE_SAP_REQ) {
        sap_req_t* sap_req = (sap_req_t*)frameq->q;
        sap_seq_t* seq = 0;

        if (sap_req->sap_session) {
            seq = sap_seq_malloc(frame, sap_req->sap_session, sap_req->hs);
            CILIX_QUEUE_INSERT(seq, &frame->snqueue);
            sap_seq_start(seq);
        }

        CILIX_QUEUE_FOREACH(sap_seq_t*, pseq, &frame->snqueue) {
            if (pseq->hs == sap_req->hs) {
                seq = pseq;
                break;
            }
        }}} if (w) { *(long*)w = (seq != 0 ? SP_RET_SUCCESS : SP_RET_ERR_HSERVICE_NOT_EXIST); }

        void* packer = 0;
        SPIP_CALL(spip_req_packer_create) {
            packer = pf(SCAP->spip_p, sap_req->req);
        }}

        void* data = 0;
        long  datalen = 0;
        SPIP_CALL(spip_req_packer_data) {
            pf(SCAP->spip_p, packer, &data, &datalen);
            sap_seq_write_packet(seq, data, datalen);
        }}

        SPIP_CALL(spip_req_packer_destroy) {
            pf(SCAP->spip_p, packer);
        }}

        qdcb(sem);
    }

    sap_frame_q_free(frameq);
}

void sap_frame_q_clean_cb(cilix_task_t* task, void* udata) {
    sap_frame_q_free((sap_frame_q_t*)udata);
}
