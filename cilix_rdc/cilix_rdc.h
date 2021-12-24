#ifndef _CILIX_RDC_H_
#define _CILIX_RDC_H_

#include "cilix_task.h"
#include "cilix_socket.h"
#include "cilix_system.h"

#include "cilix_sp_ddk.h"
#include "cilix_sp_cps.h"

typedef struct rdc_s {
    void*    dlib_h;
    sp_cps_t dlib_cp;
    char     dlib_phy[256];
    char     dlib_name[1025];

    cilix_task_t* task;

    cilix_skcb_t skcbs;
    cilix_acceptor_t* acceptor;
} rdc_t;

#endif // #ifndef _CILIX_RDC_H_