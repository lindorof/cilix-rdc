#ifndef _CILIX_SAP_H_
#define _CILIX_SAP_H_

#include "cilix_sp_ddk.h"
#include "cilix_sp_exp.h"

#define SPI_AP_API _CILIX_SP_EXP

SPI_AP_API sp_cp_t* sap_startup(void);
SPI_AP_API void sap_cleanup(void);

SPI_AP_API long sap_post_req(void* hs, void* req, sap_session_t* sap_session);
SPI_AP_API void sap_close_ap(void* hs);

#endif // #ifndef _CILIX_SAP_H_