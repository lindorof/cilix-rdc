#ifndef _CILIX_SP_DDK_H_
#define _CILIX_SP_DDK_H_

// lindorof.2021.12.08

typedef struct sp_cp_s sp_cp_t;
typedef struct sp_sapp_cp_s sp_sapp_cp_t;
typedef struct sp_spip_cp_s sp_spip_cp_t;
typedef struct sp_dlib_cp_s sp_dlib_cp_t;

#define SP_RET_SUCCESS                  (0)
#define SP_RET_ERR_INTERNAL_ERR         (1)
#define SP_RET_ERR_HSERVICE_NOT_EXIST   (2)
#define SP_RET_ERR_DLIB_FUNC_NOT_EXIST  (3)

/*
--------------------------------------------------------------------------------
interfaces of spi_ap 
eg : typedef ___func, and then "func" is symbol name in lib
--------------------------------------------------------------------------------
*/

typedef struct sap_session_s sap_session_t;

typedef void (*sap_connect_err_cb)(sap_session_t* sap_session, int sys_err);
typedef void (*sap_disconnect_cb)(sap_session_t* sap_session);
typedef void (*sap_response_cb)(sap_session_t* sap_session, void* rsp);
typedef void (*sap_session_clean_cb)(sap_session_t* sap_session);

struct sap_session_s {
    void* user_data;
    char ip[100];
    int port;
    char spip[1025];

    sap_connect_err_cb connect_err_cb;
    sap_disconnect_cb disconnect_cb;
    sap_response_cb response_cb;
    sap_session_clean_cb session_clean_cb;
};

typedef sp_cp_t* (*___sap_startup)(void);
typedef void (*___sap_cleanup)(void);

// return : SP_RET_XXX
// req : mem of req is managed by caller
// sap_session : means OPEN if not null , cleaned by sap_session_clean_cb
typedef long (*___sap_post_req)(void* hs, void* req, sap_session_t* sap_session);
typedef void (*___sap_close_ap)(void* hs);

/*
--------------------------------------------------------------------------------
interfaces of spi_package 
eg : typedef ___func, and then "func" is symbol name in lib
--------------------------------------------------------------------------------
*/

typedef void* (*___spip_pkg_create)(sp_cp_t* sp_cp);
typedef void  (*___spip_pkg_destroy)(void* spipkg);

typedef void* (*___spip_req_packer_create)(void* spipkg, void* req);
typedef void  (*___spip_req_packer_data)(void* spipkg, void* packer, void** data, long* datalen);
typedef void  (*___spip_req_packer_destroy)(void* spipkg, void* packer);

typedef void* (*___spip_rsp_packet)(void* spipkg, void* packet, long packetlen);
typedef void  (*___spip_rsp_free)(void* spipkg, void* rsp);

/*
--------------------------------------------------------------------------------
interfaces of driver lib 
--------------------------------------------------------------------------------
*/

#define DLIB_INIT "Init"
typedef void (*___dlib_init)(char* phy, sp_cp_t* sp_cp);

#define DLIB_FUNC "xxx"
typedef int (*___dlib_func)(void* json_in, void* json_out);

#define DLIB_FINI "Fini"
typedef void (*___dlib_fini)(void);

#define DLIB_INVOKE "Invoke"
typedef int(*___dlib_invoke)(char* func, void* json_in, void** json_out);

/*
--------------------------------------------------------------------------------
interfaces of sp_cp
--------------------------------------------------------------------------------
*/

typedef sp_cp_t* (*___sp_cp_create)(void);
typedef void (*___sp_cp_destroy)(sp_cp_t* sp_cp);

/*
--------------------------------------------------------------------------------
cps
--------------------------------------------------------------------------------
*/

typedef void* (*sp_cp_timeout_init)(int timeout);
typedef void (*sp_cp_timeout_destroy)(void* tmt);
typedef int (*sp_cp_timeout_flag)(void* tmt);
typedef void (*sp_cp_timeout_sleep)(int timeout);

typedef struct sp_cp_timeout_s {
    sp_cp_timeout_init init;
    sp_cp_timeout_destroy destroy;
    sp_cp_timeout_flag flag; // 0 - not timeout yet
    sp_cp_timeout_sleep sleep;
} sp_cp_timeout_t;

typedef void* (*sp_cp_dlib_open)(char* libname);
typedef void* (*sp_cp_dlib_sym)(void* dlib, char* symname);
typedef void (*sp_cp_dlib_close)(void* dlib);
typedef void* (*sp_cp_dlib_open_auto_exepath)(char* libname);
typedef void* (*sp_cp_dlib_open_auto_exepath_prefix_suffix)(char* libname);

typedef struct sp_cp_dlib_s {
    sp_cp_dlib_open open;
    sp_cp_dlib_sym sym;
    sp_cp_dlib_close close;
    sp_cp_dlib_open_auto_exepath open_ae;
    sp_cp_dlib_open_auto_exepath_prefix_suffix open_aeps;
} sp_cp_dlib_t;

typedef void* (*sp_cp_alloc_buf)(unsigned long len);
typedef void* (*sp_cp_alloc_more)(unsigned long len, void* po);
typedef void (*sp_cp_alloc_fini)(void* po);

typedef struct sp_cp_alloc_s {
    sp_cp_alloc_buf buf;
    sp_cp_alloc_more more;
    sp_cp_alloc_fini fini;
} sp_cp_alloc_t;

typedef void (*sp_cp_path_exe_path_name)(char* epn);
typedef void (*sp_cp_path_exe_path)(char* epn, char* path);
typedef void (*sp_cp_path_exe_path_name_no_suffix)(char* epn, char* pathnamenosuf);
typedef void (*sp_cp_path_exe_path_patch_name)(char* epn, char* filename, char* pathname);
typedef void (*sp_cp_path_exe_path_patch_name_shared_ps)(char* epn, char* filename, char* pathname);
typedef void (*sp_cp_path_exe_name)(char* epn, char* exename);
typedef void (*sp_cp_path_exe_name_no_suffix)(char* epn, char* exenamenosuf);

typedef void (*sp_cp_path_cwd)(char* cwd);
typedef void (*sp_cp_path_cwd_change)(char* cwd);
typedef void (*sp_cp_path_home_dir)(char* homedir);
typedef void (*sp_cp_path_tmp_dir)(char* tmpdir);

typedef struct sp_cp_path_s {
    sp_cp_path_exe_path_name exe_path_name;
    sp_cp_path_exe_path exe_path;
    sp_cp_path_exe_path_name_no_suffix exe_path_name_no_suffix;
    sp_cp_path_exe_path_patch_name exe_path_patch_name;
    sp_cp_path_exe_path_patch_name_shared_ps exe_path_patch_name_shared_ps;
    sp_cp_path_exe_name exe_name;
    sp_cp_path_exe_name_no_suffix exe_name_no_suffix;

    sp_cp_path_cwd cwd;
    sp_cp_path_cwd_change cwd_change;
    sp_cp_path_home_dir home_dir;
    sp_cp_path_tmp_dir tmp_dir;
} sp_cp_path_t;

typedef void (*sp_cp_os_user_name)(char* username);
typedef void (*sp_cp_os_host_name)(char* hostname);
typedef void (*sp_cp_os_sys_name)(char* sysname);
typedef void (*sp_cp_os_sys_version)(char* sysver);
typedef void (*sp_cp_os_sys_version_detail)(char* sysverdetail);
typedef void (*sp_cp_os_sys_processor)(char* sysprocessor);
typedef void (*sp_cp_os_mem_total)(unsigned long long* mt);
typedef void (*sp_cp_os_mem_free)(unsigned long long* mf);
typedef void (*sp_cp_os_shared_prefix)(char* soprefix);
typedef void (*sp_cp_os_shared_suffix)(char* sosuffix);
typedef void (*sp_cp_os_executable_suffix)(char* exesuffix);

typedef struct sp_cp_os_s {
    sp_cp_os_user_name user_name;
    sp_cp_os_host_name host_name;
    sp_cp_os_sys_name sys_name;
    sp_cp_os_sys_version sys_version;
    sp_cp_os_sys_version_detail sys_version_detail;
    sp_cp_os_sys_processor sys_processor;
    sp_cp_os_mem_total mem_total;
    sp_cp_os_mem_free mem_free;
    sp_cp_os_shared_prefix shared_prefix;
    sp_cp_os_shared_suffix shared_suffix;
    sp_cp_os_executable_suffix executable_suffix;
} sp_cp_os_t;

typedef void (*sp_cp_project_build_type)(char* buildtype);
typedef void (*sp_cp_project_version)(char* projectversion);

typedef struct sp_cp_project_s {
    sp_cp_project_build_type build_type;
    sp_cp_project_version version;
} sp_cp_project_t;

typedef void (*sp_cp_time_hr_nano_tick)(unsigned long long* nano_ui64);
typedef void (*sp_cp_time_of_day)(long long* sec_i64, int* usec_i32);

typedef struct sp_cp_time_s {
    sp_cp_time_hr_nano_tick hr_nano_tick;
    sp_cp_time_of_day of_day;
} sp_cp_time_t;

typedef void (*sp_cp_ifaddr_init)(void** ifaddr, int* num);
typedef void (*sp_cp_ifaddr_fini)(void* ifaddr);
typedef void (*sp_cp_ifaddr_at_name)(void* ifaddr, int idx, char* name);
typedef void (*sp_cp_ifaddr_at_internal)(void* ifaddr, int idx, int* internal);
typedef void (*sp_cp_ifaddr_at_addr_phy)(void* ifaddr, int idx, char* addrphy);
typedef void (*sp_cp_ifaddr_at_addr_4)(void* ifaddr, int idx, char* addr4);
typedef void (*sp_cp_ifaddr_at_addr_6)(void* ifaddr, int idx, char* addr6);
typedef void (*sp_cp_ifaddr_at_mask_4)(void* ifaddr, int idx, char* mask4);
typedef void (*sp_cp_ifaddr_at_mask_6)(void* ifaddr, int idx, char* mask6);

typedef struct sp_cp_ifaddr_s {
    sp_cp_ifaddr_init init;
    sp_cp_ifaddr_fini fini;
    sp_cp_ifaddr_at_name at_name;
    sp_cp_ifaddr_at_internal at_internal;
    sp_cp_ifaddr_at_addr_phy at_addr_phy;
    sp_cp_ifaddr_at_addr_4 at_addr_4;
    sp_cp_ifaddr_at_addr_6 at_addr_6;
    sp_cp_ifaddr_at_mask_4 at_mask_4;
    sp_cp_ifaddr_at_mask_6 at_mask_6;
} sp_cp_ifaddr_t;

typedef void (*sp_cp_cpu_init)(void** cpu, int* num);
typedef void (*sp_cp_cpu_fini)(void* cpu);
typedef void (*sp_cp_cpu_at_model)(void* cpu, int idx, char* model);
typedef void (*sp_cp_cpu_at_speed)(void* cpu, int idx, int* speed);
typedef void (*sp_cp_cpu_at_times_user)(void* cpu, int idx, unsigned long long* tuser);
typedef void (*sp_cp_cpu_at_times_nice)(void* cpu, int idx, unsigned long long* tnice);
typedef void (*sp_cp_cpu_at_times_sys)(void* cpu, int idx, unsigned long long* tsys);
typedef void (*sp_cp_cpu_at_times_idle)(void* cpu, int idx, unsigned long long* tidle);
typedef void (*sp_cp_cpu_at_times_irq)(void* cpu, int idx, unsigned long long* tirq);

typedef struct sp_cp_cpu_s {
    sp_cp_cpu_init init;
    sp_cp_cpu_fini fini;
    sp_cp_cpu_at_model at_model;
    sp_cp_cpu_at_speed at_speed;
    sp_cp_cpu_at_times_user at_times_user;
    sp_cp_cpu_at_times_nice at_times_nice;
    sp_cp_cpu_at_times_sys at_times_sys;
    sp_cp_cpu_at_times_idle at_times_idle;
    sp_cp_cpu_at_times_irq at_times_irq;
} sp_cp_cpu_t;

typedef void (*sp_cp_env_init)(void** env, int* num);
typedef void (*sp_cp_env_fini)(void* env);
typedef void (*sp_cp_env_at)(void* env, int idx, char* name, char* value);
typedef void (*sp_cp_env_get)(char* name, char* value);
typedef void (*sp_cp_env_set)(char* name, char* value);
typedef void (*sp_cp_env_unset)(char* name);

typedef struct sp_cp_env_s {
    sp_cp_env_init init;
    sp_cp_env_fini fini;
    sp_cp_env_at at;
    sp_cp_env_get get;
    sp_cp_env_set set;
    sp_cp_env_unset unset;
} sp_cp_env_t;

typedef void* (*sp_cp_mutex_init)(void);
typedef void (*sp_cp_mutex_destroy)(void* mtx);
typedef void (*sp_cp_mutex_lock)(void* mtx);
typedef int (*sp_cp_mutex_trylock)(void* mtx);
typedef void (*sp_cp_mutex_unlock)(void* mtx);

typedef struct sp_cp_mutex_s {
    sp_cp_mutex_init init;
    sp_cp_mutex_destroy destroy;
    sp_cp_mutex_lock lock;
    sp_cp_mutex_trylock trylock; // 0 - success
    sp_cp_mutex_unlock unlock;
} sp_cp_mutex_t;

typedef void* (*sp_cp_thread_start)(void* func, void* arg);
typedef void (*sp_cp_thread_wait)(void* t);
typedef void (*sp_cp_thread_destroy)(void* t);

typedef struct sp_cp_thread_s {
    sp_cp_thread_start start; // void (*func)(void* arg)
    sp_cp_thread_wait wait;
    sp_cp_thread_destroy destroy;
} sp_cp_thread_t;

typedef void* (*sp_cp_toml_init)(char* str, int is_file);
typedef void (*sp_cp_toml_destroy)(void* ct);
typedef int (*sp_cp_toml_get_node_numb)(void* ct, void* node, char* path);
typedef int (*sp_cp_toml_get_keyv_numb)(void* ct, void* node, char* path);
typedef const char* (*sp_cp_toml_get_node_name_at)(void* ct, void* node, char* path, int idx);
typedef const char* (*sp_cp_toml_get_keyv_name_at)(void* ct, void* node, char* path, int idx);
typedef void* (*sp_cp_toml_get_node_o)(void* ct, void* node, char* path);
typedef void (*sp_cp_toml_get_keyv_s)(void* ct, void* node, char* path, char* key, char* val, char* df);
typedef double (*sp_cp_toml_get_keyv_f)(void* ct, void* node, char* path, char* key, double df);
typedef long long (*sp_cp_toml_get_keyv_i)(void* ct, void* node, char* path, char* key, long long df);
typedef void* (*sp_cp_toml_get_keyv_r)(void* ct, void* node, char* path, char* key);
typedef int (*sp_cp_toml_r_elem_numb)(void* r);
typedef void (*sp_cp_toml_r_elem_s)(void* r, int idx, char* val);
typedef double (*sp_cp_toml_r_elem_f)(void* r, int idx);
typedef long long (*sp_cp_toml_r_elem_i)(void* r, int idx);

typedef struct sp_cp_toml_s {
    sp_cp_toml_init init;
    sp_cp_toml_destroy destroy;
    sp_cp_toml_get_node_numb get_node_numb;
    sp_cp_toml_get_keyv_numb get_keyv_numb;
    sp_cp_toml_get_node_name_at get_node_name_at;
    sp_cp_toml_get_keyv_name_at get_keyv_name_at;
    sp_cp_toml_get_node_o get_node_o;
    sp_cp_toml_get_keyv_s get_keyv_s;
    sp_cp_toml_get_keyv_f get_keyv_f;
    sp_cp_toml_get_keyv_i get_keyv_i;
    sp_cp_toml_get_keyv_r get_keyv_r;
    sp_cp_toml_r_elem_numb r_elem_numb;
    sp_cp_toml_r_elem_s r_elem_s;
    sp_cp_toml_r_elem_f r_elem_f;
    sp_cp_toml_r_elem_i r_elem_i;
} sp_cp_toml_t;

// str : nil to create json root
// len : <=0 to use strlen(str)
typedef void* (*sp_cp_json_init)(char* str, long len);
typedef void (*sp_cp_json_destroy)(void* obj);
typedef void*  (*sp_cp_json_get_obj)(void* obj, char* name);
typedef char*  (*sp_cp_json_get_str)(void* obj, char* name);
typedef int    (*sp_cp_json_get_int)(void* obj, char* name);
typedef double (*sp_cp_json_get_double)(void* obj, char* name);
typedef void*  (*sp_cp_json_get_arr)(void* obj, char* name);
typedef int    (*sp_cp_json_get_arr_size)(void* obj);
typedef void*  (*sp_cp_json_get_arr_obj)(void* obj, int idx);
typedef char*  (*sp_cp_json_get_arr_str)(void* obj, int idx);
typedef int    (*sp_cp_json_get_arr_int)(void* obj, int idx);
typedef double (*sp_cp_json_get_arr_double)(void* obj, int idx);
typedef void* (*sp_cp_json_add_obj)(void* obj, char* name);
typedef void* (*sp_cp_json_add_str)(void* obj, char* name, char* val);
typedef void* (*sp_cp_json_add_int)(void* obj, char* name, int val);
typedef void* (*sp_cp_json_add_double)(void* obj, char* name, double val);
typedef void* (*sp_cp_json_add_arr)(void* obj, char* name);
typedef void* (*sp_cp_json_add_arr_obj)(void* obj, char* name);
typedef void* (*sp_cp_json_add_arr_str)(void* obj, char* name, char* val);
typedef void* (*sp_cp_json_add_arr_int)(void* obj, char* name, int val);
typedef void* (*sp_cp_json_add_arr_double)(void* obj, char* name, double val);
typedef char* (*sp_cp_json_print)(void* obj);
typedef void  (*sp_cp_json_print_free)(char* text);

typedef struct sp_cp_json_s {
    sp_cp_json_init init;
    sp_cp_json_destroy destroy;
    sp_cp_json_get_obj get_obj;
    sp_cp_json_get_str get_str;
    sp_cp_json_get_int get_int;
    sp_cp_json_get_double get_double;
    sp_cp_json_get_arr get_arr;
    sp_cp_json_get_arr_size get_arr_size;
    sp_cp_json_get_arr_obj get_arr_obj;
    sp_cp_json_get_arr_str get_arr_str;
    sp_cp_json_get_arr_int get_arr_int;
    sp_cp_json_get_arr_double get_arr_double;
    sp_cp_json_add_obj add_obj;
    sp_cp_json_add_str add_str;
    sp_cp_json_add_int add_int;
    sp_cp_json_add_double add_double;
    sp_cp_json_add_arr add_arr;
    sp_cp_json_add_arr_obj add_arr_obj;
    sp_cp_json_add_arr_str add_arr_str;
    sp_cp_json_add_arr_int add_arr_int;
    sp_cp_json_add_arr_double add_arr_double;
    sp_cp_json_print print;
    sp_cp_json_print_free print_free;
} sp_cp_json_t;

struct sp_sapp_cp_s {
	void* def;
};

struct sp_spip_cp_s {
	void* def;
};

struct sp_dlib_cp_s {
	void* def;
};

#define SP_CP_FIELDS                \
    void* cp_context;               \
                                    \
    sp_sapp_cp_t* _sapp;            \
    sp_spip_cp_t* _spip;            \
    sp_dlib_cp_t* _dlib;            \
                                    \
    sp_cp_timeout_t* timeout;       \
    sp_cp_dlib_t* dlib;             \
    sp_cp_alloc_t* alloc;           \
    sp_cp_path_t* path;             \
    sp_cp_os_t* os;                 \
    sp_cp_project_t* project;       \
    sp_cp_time_t* time;             \
    sp_cp_ifaddr_t* ifaddr;         \
    sp_cp_cpu_t* cpu;               \
    sp_cp_env_t* env;               \
    sp_cp_mutex_t* mutex;           \
    sp_cp_thread_t* thread;         \
    sp_cp_toml_t* toml;             \
    sp_cp_json_t* json;             \

struct sp_cp_s {
    SP_CP_FIELDS
};

#endif // #ifndef _CILIX_SP_DDK_H_