#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "cilix_sp_ddk.h"
#include "cilix_sp_cps.h"

#include "cilix_sap.h"
#include "cilix_sap_req.h"
#include "cilix_sap_frame.h"

sap_frame_t* gsf = 0;

sp_cp_t* sap_startup() {
    if (!gsf) {
        gsf = sap_frame_malloc_start();
    } return (sp_cp_t*)&gsf->sp_cps;
}

void sap_cleanup(void) {
    if (gsf) {
        sap_frame_stop_free(gsf);
    } gsf = 0;
}

long sap_post_req(void* hs, void* req, sap_session_t* sap_session) {
    sap_req_t sap_req = {0}; {
        sap_req.hs = hs;
        sap_req.req = req;
        sap_req.sap_session = sap_session;
    }

    long sp_ret = SP_RET_SUCCESS;
    sap_frame_putq_with_type(gsf, SAP_FRAME_Q_TYPE_SAP_REQ, &sap_req, 1, &sp_ret);
    return sp_ret;
}

void sap_close_ap(void* hs) {
    sap_frame_putq_with_type(gsf, SAP_FRAME_Q_TYPE_CLOSE_SESSION, hs, 0, 0);
}
