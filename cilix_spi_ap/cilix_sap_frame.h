#ifndef _CILIX_SAP_FRAME_H_
#define _CILIX_SAP_FRAME_H_

#include "cilix_task.h"
#include "cilix_queue.h"

#include "cilix_sp_cps.h"

typedef long SAP_FRAME_Q_TYPE;

#define	SAP_FRAME_Q_TYPE_SAP_REQ        (1)
#define	SAP_FRAME_Q_TYPE_CLOSE_SESSION  (2)
#define	SAP_FRAME_Q_TYPE_CLEAN_SESSION  (3)
#define	SAP_FRAME_Q_TYPE_STOP_ALL       (4)

typedef struct sap_frame_q_s {
    SAP_FRAME_Q_TYPE type;
    void* q;
} sap_frame_q_t;

typedef struct sap_frame_s {
    sp_cps_t sp_cps;
    CILIX_QUEUE snqueue;
    cilix_task_t* task;
} sap_frame_t;

// *** public - start ***
sap_frame_t* sap_frame_malloc_start(void);
void sap_frame_stop_free(sap_frame_t* frame);
void sap_frame_putq_with_type(sap_frame_t* frame, SAP_FRAME_Q_TYPE type, void* q, int wait, void* w);
// *** public - end ***

void sap_frame_q_get_cb(cilix_task_t* task, void* udata, void* w, cilix_task_q_done_cb qdcb, void* sem);
void sap_frame_q_clean_cb(cilix_task_t* task, void* udata);

#endif // #ifndef _CILIX_SAP_FRAME_H_
