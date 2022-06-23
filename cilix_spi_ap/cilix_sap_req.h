#ifndef _CILIX_SAP_REQ_H_
#define _CILIX_SAP_REQ_H_

#include "cilix_sp_ddk.h"

typedef struct sap_req_s {
    void* hs;
    void* req;
    sap_session_t* sap_session;
} sap_req_t;

#endif // #ifndef _CILIX_SAP_REQ_H_