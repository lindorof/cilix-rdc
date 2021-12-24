#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "uv.h"

#include "cilix_system.h"

#include "Config.h"

#define PATHMAX  (2048+1)
#define PATHSIZE (2048)
#define ENVVSIZE (2048)

#define EPN_CHECK \
    int iret = 0; \
    char buf[PATHMAX] = {0}; \
    if (!epn) { \
        iret = cilix_path_epn(buf); \
    } else { \
        strcpy(buf, epn); \
    } \
    epn = buf; \
    if (iret == 0) \

inline int cilix_path_epn(char* epn) {
    int iret = 0;

    size_t isize = PATHSIZE;
    iret = uv_exepath(epn, &isize);

    return iret;
}

void cilix_path_exe_path_name(char* val) {
    cilix_path_epn(val);
}
void cilix_path_exe_path(char* epn, char* val) {
    EPN_CHECK {
        ((char*)strrchr(epn,CILIX_PATH_SLASH))[1] = 0;
        strcpy(val, epn);
    }
}
void cilix_path_exe_path_name_no_suffix(char* epn, char* val) {
    EPN_CHECK {
        epn[strlen(epn)-strlen(_CILIX_OS_EXECUTABLE_SUFFIX)] = 0;
        strcpy(val, epn);
    }
}
void cilix_path_exe_name(char* epn, char* val) {
    EPN_CHECK {
        strcpy(val, (char*)strrchr(epn,CILIX_PATH_SLASH)+1);
    }
}
void cilix_path_exe_name_no_suffix(char* epn, char* val) {
    EPN_CHECK {
        strcpy(val, (char*)strrchr(epn,CILIX_PATH_SLASH)+1);
        val[strlen(val)-strlen(_CILIX_OS_EXECUTABLE_SUFFIX)] = 0;
    }
}
void cilix_path_exe_path_patch_name(char* epn, char* filename, char* val) {
    EPN_CHECK {
        ((char*)strrchr(epn,CILIX_PATH_SLASH))[1] = 0;
        strcpy(val, epn);
        strcat(val, filename);
    }
}
void cilix_path_exe_path_patch_name_shared_ps(char* epn, char* filename, char* val) {
    EPN_CHECK {
        ((char*)strrchr(epn,CILIX_PATH_SLASH))[1] = 0;
        strcpy(val, epn);
        strcat(val, _CILIX_OS_SHARED_PREFIX);
        strcat(val, filename);
        strcat(val, _CILIX_OS_SHARED_SUFFIX);
    }
}

void cilix_path_cwd(char* cwd) {
    size_t isize = PATHSIZE;
    uv_cwd(cwd, &isize);
}
void cilix_path_cwd_change(char* cwd) {
    uv_chdir(cwd);
}
void cilix_path_home_dir(char* homedir) {
    size_t isize = PATHSIZE;
    uv_os_homedir(homedir, &isize);
}
void cilix_path_tmp_dir(char* tmpdir) {
    size_t isize = PATHSIZE;
    uv_os_tmpdir(tmpdir, &isize);
}

void cilix_os_user_name(char* username) {
    uv_passwd_t pw = {0};
    if (uv_os_get_passwd(&pw) == 0) {
        strcpy(username, pw.username);
        uv_os_free_passwd(&pw);
    }
}
void cilix_os_host_name(char* hostname) {
    size_t isize = PATHSIZE;
    uv_os_gethostname(hostname, &isize);
}
void cilix_os_sys_name(char* ps) {
    strcpy(ps, _CILIX_OS_SYS_NAME);
}
void cilix_os_sys_version(char* ps) {
    strcpy(ps, _CILIX_OS_SYS_VERSION);
}
void cilix_os_sys_processor(char* ps) {
    strcpy(ps, _CILIX_OS_SYS_PROCESSOR);
}
void cilix_os_sys_version_detail(char* ps) {
    uv_utsname_t utsname = {0};
    if (uv_os_uname(&utsname) == 0) {
        strcpy(ps, utsname.version);
    }
}
void cilix_os_mem_total(unsigned long long* mt) {
    *mt = uv_get_total_memory();
}
void cilix_os_mem_free(unsigned long long* mf) {
    *mf = uv_get_free_memory();
}
void cilix_os_shared_prefix(char* ps) {
    strcpy(ps, _CILIX_OS_SHARED_PREFIX);
}
void cilix_os_shared_suffix(char* ps) {
    strcpy(ps, _CILIX_OS_SHARED_SUFFIX);
}
void cilix_os_executable_suffix(char* ps) {
    strcpy(ps, _CILIX_OS_EXECUTABLE_SUFFIX);
}

void cilix_project_build_type(char* ps) {
    strcpy(ps, _CILIX_PRJ_BUILD_TYPE);
}
void cilix_project_version(char* ps) {
    strcpy(ps, _CILIX_PRJ_VERSION);
}

void cilix_time_hr_nano_tick(unsigned long long* nano_ui64) {
    *nano_ui64 = uv_hrtime();
}
void cilix_time_of_day(long long* sec_i64, int* usec_i32) {
    uv_timeval64_t tv = {0};
    uv_gettimeofday(&tv); {
        *sec_i64 = tv.tv_sec;
        *usec_i32 = tv.tv_usec;
    }
}

typedef struct cilix_ifaddr_s {
    uv_interface_address_t* ias;
    int num;
} cilix_ifaddr_t;
#define IFADDR_CHECK \
    if (pifaddr == 0) return; \
    cilix_ifaddr_t* ifaddr = (cilix_ifaddr_t*)pifaddr; \
    if (idx >= ifaddr->num) return; \

void cilix_ifaddr_init(void** pifaddr, int* pnum) {
    uv_interface_address_t* ias = 0;
    int num = 0;
    if (uv_interface_addresses(&ias, &num) == 0) {
        cilix_ifaddr_t* p = (cilix_ifaddr_t*)malloc(sizeof(cilix_ifaddr_t)); {
            p->ias = ias;
            p->num = num;
        } {
            *pifaddr = p;
            *pnum = num;
        }
    }
}
void cilix_ifaddr_fini(void* pifaddr) {
    if (pifaddr == 0) return;
    cilix_ifaddr_t* ifaddr = (cilix_ifaddr_t*)pifaddr;
    uv_free_interface_addresses(ifaddr->ias, ifaddr->num);
    free(ifaddr);
}
void cilix_ifaddr_at_name(void* pifaddr, int idx, char* name) {
    IFADDR_CHECK;
    strcpy(name, ifaddr->ias[idx].name);
}
void cilix_ifaddr_at_internal(void* pifaddr, int idx, int* internal) {
    IFADDR_CHECK;
    *internal = ifaddr->ias[idx].is_internal;
}
void cilix_ifaddr_at_addr_phy(void* pifaddr, int idx, char* addrphy) {
    IFADDR_CHECK;
    for (int p=0; p<6; p++) {
        sprintf(addrphy+strlen(addrphy), "%s%02X", (p==0?"":":"), (unsigned char)(ifaddr->ias[idx].phys_addr[p]));
    }
}
void cilix_ifaddr_at_addr_4(void* pifaddr, int idx, char* addr4) {
    IFADDR_CHECK;
    uv_ip4_name(&ifaddr->ias[idx].address.address4, addr4, 100);
}
void cilix_ifaddr_at_addr_6(void* pifaddr, int idx, char* addr6) {
    IFADDR_CHECK;
    uv_ip6_name(&ifaddr->ias[idx].address.address6, addr6, 100);
}
void cilix_ifaddr_at_mask_4(void* pifaddr, int idx, char* mask4) {
    IFADDR_CHECK;
    uv_ip4_name(&ifaddr->ias[idx].netmask.netmask4, mask4, 100);
}
void cilix_ifaddr_at_mask_6(void* pifaddr, int idx, char* mask6) {
    IFADDR_CHECK;
    uv_ip6_name(&ifaddr->ias[idx].netmask.netmask6, mask6, 100);
}

typedef struct cilix_cpu_s {
    uv_cpu_info_t* ci;
    int num;
} cilix_cpu_t;
#define CPU_CHECK \
    if (pcpu == 0) return; \
    cilix_cpu_t* cpu = (cilix_cpu_t*)pcpu; \
    if (idx >= cpu->num) return; \

void cilix_cpu_init(void** pcpu, int* pnum) {
    uv_cpu_info_t* ci = 0;
    int num = 0;
    if (uv_cpu_info(&ci, &num) == 0) {
        cilix_cpu_t* p = (cilix_cpu_t*)malloc(sizeof(cilix_cpu_t)); {
            p->ci = ci;
            p->num = num;
        } {
            *pcpu = p;
            *pnum = num;
        }
    }
}
void cilix_cpu_fini(void* pcpu) {
    if (pcpu == 0) return;
    cilix_cpu_t* cpu = (cilix_cpu_t*)pcpu;
    uv_free_cpu_info(cpu->ci, cpu->num);
    free(cpu);
}
void cilix_cpu_at_model(void* pcpu, int idx, char* model) {
    CPU_CHECK;
    strcpy(model, cpu->ci[idx].model);
}
void cilix_cpu_at_speed(void* pcpu, int idx, int* speed) {
    CPU_CHECK;
    *speed = cpu->ci[idx].speed;
}
void cilix_cpu_at_times_user(void* pcpu, int idx, unsigned long long* tuser) {
    CPU_CHECK;
    *tuser = cpu->ci[idx].cpu_times.user;
}
void cilix_cpu_at_times_nice(void* pcpu, int idx, unsigned long long* tnice) {
    CPU_CHECK;
    *tnice = cpu->ci[idx].cpu_times.nice;
}
void cilix_cpu_at_times_sys(void* pcpu, int idx, unsigned long long* tsys) {
    CPU_CHECK;
    *tsys = cpu->ci[idx].cpu_times.sys;
}
void cilix_cpu_at_times_idle(void* pcpu, int idx, unsigned long long* tidle) {
    CPU_CHECK;
    *tidle = cpu->ci[idx].cpu_times.idle;
}
void cilix_cpu_at_times_irq(void* pcpu, int idx, unsigned long long* tirq) {
    CPU_CHECK;
    *tirq = cpu->ci[idx].cpu_times.irq;
}

typedef struct cilix_env_s {
    uv_env_item_t* ei;
    int num;
} cilix_env_t;

void cilix_env_init(void** penv, int* pnum) {
    uv_env_item_t* ei = 0;
    int num = 0;
    if (uv_os_environ(&ei, &num) == 0) {
        cilix_env_t* p = (cilix_env_t*)malloc(sizeof(cilix_env_t)); {
            p->ei = ei;
            p->num = num;
        } {
            *penv = p;
            *pnum = num;
        }
    }
}
void cilix_env_fini(void* penv) {
    if (penv == 0) return;
    cilix_env_t* env = (cilix_env_t*)penv;
    uv_os_free_environ(env->ei, env->num);
    free(env);
}
void cilix_env_at(void* penv, int idx, char* name, char* value) {
    if (penv == 0) return;
    cilix_env_t* env = (cilix_env_t*)penv;
    if (idx >= env->num) return;
    strcpy(name, env->ei[idx].name);
    strcpy(value, env->ei[idx].value);
}
void cilix_env_get(char* name, char* value) {
    size_t isize = ENVVSIZE;
    uv_os_getenv(name, value, &isize);
}
void cilix_env_set(char* name, char* value) {
    uv_os_setenv(name, value);
}
void cilix_env_unset(char* name) {
    uv_os_unsetenv(name);
}
