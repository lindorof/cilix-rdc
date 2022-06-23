#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "cilix_rdc_ivk.h"

#include "cilix_dlib.h"
#include "cilix_timeout.h"
#include "cilix_allocator.h"
#include "cilix_system.h"
#include "cilix_mutex.h"
#include "cilix_thread.h"
#include "cilix_toml.h"
#include "cilix_json.h"
#include "cilix_base64.h"

#define DLIB_CALL(sym, str)                                     \
    {                                                           \
        ___##sym df = (___##sym)cilix_dlib_sym(rdc->h, str);    \
        if (df)                                                 \

typedef struct rdc_s {
    sp_cps_t cp;
    char     phy[256];
    char     drv[1025];
    void*    h;
} rdc_t;

void* rdc_init(char* phy, char* drv) {
    rdc_t* rdc = (rdc_t*)malloc(sizeof(rdc_t));

    sp_cps_init(&rdc->cp);
    strcpy(rdc->phy, phy);
    strcpy(rdc->drv, drv);
    rdc->h = cilix_dlib_open_aeps(drv);

    DLIB_CALL(dlib_init, DLIB_INIT) {
        df(phy, (sp_cp_t*)&rdc->cp);
    }}

    return rdc;
}

void rdc_fini(void* prdc) {
    rdc_t* rdc = (rdc_t*)prdc;
    if (rdc == 0) return;

    DLIB_CALL(dlib_fini, DLIB_FINI) {
        df();
    }}

    cilix_dlib_close(rdc->h);
    free(rdc);
}

int rdc_invoke(void* prdc, char* fun, char* in, int inlen, void*** out) {
    rdc_t* rdc = (rdc_t*)prdc;
    int ret = 0;

    void* jin = cilix_json_init(in, inlen);
    void* jout = cilix_json_init(0, 0);

    DLIB_CALL(dlib_invoke, DLIB_INVOKE) {
        ret = df(fun, jin, jout);
    } else { ret = SP_RET_ERR_DLIB_FUNC_NOT_EXIST; } }

    void** ro = (void**)malloc(sizeof(void*) * 3);
    ro[0] = cilix_json_print(jout);
    ro[1] = jout;
    ro[2] = jin;

    *out = ro;
    return ret;
}

void rdc_invoke_free(void* rdc, void** out) {
    if (out == 0) return;

    cilix_json_print_free(out[0]);
    cilix_json_destroy(out[1]);
    cilix_json_destroy(out[2]);
    free(out);
}
