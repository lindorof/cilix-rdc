#ifndef _CILIX_RDC_IVK_H_
#define _CILIX_RDC_IVK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cilix_sp_ddk.h"
#include "cilix_sp_cps.h"
#include "cilix_sp_exp.h"

#define RDC_IVK_API _CILIX_SP_EXP

RDC_IVK_API void* rdc_init(char* phy, char* drv);
RDC_IVK_API void rdc_fini(void* rdc);
RDC_IVK_API char* rdc_invoke(void* rdc, char* fun, char* in, int inlen, int* ret);
RDC_IVK_API void rdc_invoke_free(void* rdc, char* out);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _CILIX_RDC_IVK_H_