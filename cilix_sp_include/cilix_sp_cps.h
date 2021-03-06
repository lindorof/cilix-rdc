#ifndef _CILIX_SP_CPS_H_
#define _CILIX_SP_CPS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cilix_sp_ddk.h"
#include "cilix_sp_exp.h"

#define SP_CPS_API _CILIX_SP_EXP

typedef struct sp_cps_s {
    SP_CP_FIELDS

    sp_sapp_cp_t _sapp__t;
    sp_spip_cp_t _spip__t;
    sp_dlib_cp_t _dlib__t;

    sp_cp_timeout_t timeout__t;
    sp_cp_dlib_t dlib__t;
    sp_cp_alloc_t alloc__t;
    sp_cp_path_t path__t;
    sp_cp_os_t os__t;
    sp_cp_project_t project__t;
    sp_cp_time_t time__t;
    sp_cp_ifaddr_t ifaddr__t;
    sp_cp_cpu_t cpu__t;
    sp_cp_env_t env__t;
    sp_cp_mutex_t mutex__t;
    sp_cp_thread_t thread__t;
    sp_cp_toml_t toml__t;
    sp_cp_json_t json__t;
    sp_cp_base64_t base64__t;
} sp_cps_t;

void sp_cps_init(sp_cps_t* cps);

SP_CPS_API sp_cp_t* sp_cp_create(void);
SP_CPS_API void sp_cp_destroy(sp_cp_t* sp_cp);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _CILIX_SP_CPS_H_
