#ifndef _CILIX_SYSTEM_H_
#define _CILIX_SYSTEM_H_

int cilix_path_epn(char* epn);

// *** public - start ***

void cilix_path_exe_path_name(char* epn);
void cilix_path_exe_path(char* epn, char* path);
void cilix_path_exe_path_name_no_suffix(char* epn, char* pathnamenosuf);
void cilix_path_exe_path_patch_name(char* epn, char* filename, char* pathname);
void cilix_path_exe_path_patch_name_shared_ps(char* epn, char* filename, char* pathname);
void cilix_path_exe_name(char* epn, char* exename);
void cilix_path_exe_name_no_suffix(char* epn, char* exenamenosuf);

void cilix_path_cwd(char* cwd);
void cilix_path_cwd_change(char* cwd);
void cilix_path_home_dir(char* homedir);
void cilix_path_tmp_dir(char* tmpdir);

void cilix_os_user_name(char* username);
void cilix_os_host_name(char* hostname);
void cilix_os_sys_name(char* sysname);
void cilix_os_sys_version(char* sysver);
void cilix_os_sys_version_detail(char* sysverdetail);
void cilix_os_sys_processor(char* sysprocessor);
void cilix_os_mem_total(unsigned long long* mt);
void cilix_os_mem_free(unsigned long long* mf);
void cilix_os_shared_prefix(char* soprefix);
void cilix_os_shared_suffix(char* sosuffix);
void cilix_os_executable_suffix(char* exesuffix);

void cilix_project_build_type(char* buildtype);
void cilix_project_version(char* projectversion);

void cilix_time_hr_nano_tick(unsigned long long* nano_ui64);
void cilix_time_of_day(long long* sec_i64, int* usec_i32);

void cilix_ifaddr_init(void** ifaddr, int* num);
void cilix_ifaddr_fini(void* ifaddr);
void cilix_ifaddr_at_name(void* ifaddr, int idx, char* name);
void cilix_ifaddr_at_internal(void* ifaddr, int idx, int* internal);
void cilix_ifaddr_at_addr_phy(void* ifaddr, int idx, char* addrphy);
void cilix_ifaddr_at_addr_4(void* ifaddr, int idx, char* addr4);
void cilix_ifaddr_at_addr_6(void* ifaddr, int idx, char* addr6);
void cilix_ifaddr_at_mask_4(void* ifaddr, int idx, char* mask4);
void cilix_ifaddr_at_mask_6(void* ifaddr, int idx, char* mask6);

void cilix_cpu_init(void** cpu, int* num);
void cilix_cpu_fini(void* cpu);
void cilix_cpu_at_model(void* cpu, int idx, char* model);
void cilix_cpu_at_speed(void* cpu, int idx, int* speed);
void cilix_cpu_at_times_user(void* cpu, int idx, unsigned long long* tuser);
void cilix_cpu_at_times_nice(void* cpu, int idx, unsigned long long* tnice);
void cilix_cpu_at_times_sys(void* cpu, int idx, unsigned long long* tsys);
void cilix_cpu_at_times_idle(void* cpu, int idx, unsigned long long* tidle);
void cilix_cpu_at_times_irq(void* cpu, int idx, unsigned long long* tirq);

void cilix_env_init(void** env, int* num);
void cilix_env_fini(void* env);
void cilix_env_at(void* env, int idx, char* name, char* value);
void cilix_env_get(char* name, char* value);
void cilix_env_set(char* name, char* value);
void cilix_env_unset(char* name);

// *** public - end ***

#endif // #ifndef _CILIX_SYSTEM_H_
