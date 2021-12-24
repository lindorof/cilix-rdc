#include <stdio.h>
#include <stdlib.h>

#include "cilix_sp_cps.h"

void sp_cps_init(sp_cps_t* cps) {

    memset(cps, 0, sizeof(sp_cps_t)); {
        cps->_sapp = &cps->_sapp__t;
        cps->_spip = &cps->_spip__t;
        cps->_dlib = &cps->_dlib__t;
    }

    cps->timeout = &cps->timeout__t; {
        cps->timeout->init = cilix_timeout_init;
        cps->timeout->destroy = cilix_timeout_destroy;
        cps->timeout->flag = cilix_timeout_flag;
        cps->timeout->sleep = cilix_timeout_sleep;
    }

    cps->dlib = &cps->dlib__t; {
        cps->dlib->open = cilix_dlib_open;
        cps->dlib->sym = cilix_dlib_sym;
        cps->dlib->close = cilix_dlib_close;
        cps->dlib->open_ae = cilix_dlib_open_ae;
        cps->dlib->open_aeps = cilix_dlib_open_aeps;
    }

    cps->alloc = &cps->alloc__t; {
        cps->alloc->buf = cilix_alloc_buf;
        cps->alloc->more = cilix_alloc_more;
        cps->alloc->fini = cilix_alloc_fini;
    }

    cps->path = &cps->path__t; {
        cps->path->exe_path_name = cilix_path_exe_path_name;
        cps->path->exe_path = cilix_path_exe_path;
        cps->path->exe_path_name_no_suffix = cilix_path_exe_path_name_no_suffix;
        cps->path->exe_path_patch_name = cilix_path_exe_path_patch_name;
        cps->path->exe_path_patch_name_shared_ps = cilix_path_exe_path_patch_name_shared_ps;
        cps->path->exe_name = cilix_path_exe_name;
        cps->path->exe_name_no_suffix = cilix_path_exe_name_no_suffix;

        cps->path->cwd = cilix_path_cwd;
        cps->path->cwd_change = cilix_path_cwd_change;
        cps->path->home_dir = cilix_path_home_dir;
        cps->path->tmp_dir = cilix_path_tmp_dir;
    }

    cps->os = &cps->os__t; {
        cps->os->user_name = cilix_os_user_name;
        cps->os->host_name = cilix_os_host_name;
        cps->os->sys_name = cilix_os_sys_name;
        cps->os->sys_version = cilix_os_sys_version;
        cps->os->sys_version_detail = cilix_os_sys_version_detail;
        cps->os->sys_processor = cilix_os_sys_processor;
        cps->os->mem_total = cilix_os_mem_total;
        cps->os->mem_free = cilix_os_mem_free;
        cps->os->shared_prefix = cilix_os_shared_prefix;
        cps->os->shared_suffix = cilix_os_shared_suffix;
        cps->os->executable_suffix = cilix_os_executable_suffix;
    }

    cps->project = &cps->project__t; {
        cps->project->build_type = cilix_project_build_type;
        cps->project->version = cilix_project_version;
    }

    cps->time = &cps->time__t; {
        cps->time->hr_nano_tick = cilix_time_hr_nano_tick;
        cps->time->of_day = cilix_time_of_day;
    }

    cps->ifaddr = &cps->ifaddr__t; {
        cps->ifaddr->init = cilix_ifaddr_init;
        cps->ifaddr->fini = cilix_ifaddr_fini;
        cps->ifaddr->at_name = cilix_ifaddr_at_name;
        cps->ifaddr->at_internal = cilix_ifaddr_at_internal;
        cps->ifaddr->at_addr_phy = cilix_ifaddr_at_addr_phy;
        cps->ifaddr->at_addr_4 = cilix_ifaddr_at_addr_4;
        cps->ifaddr->at_addr_6 = cilix_ifaddr_at_addr_6;
        cps->ifaddr->at_mask_4 = cilix_ifaddr_at_mask_4;
        cps->ifaddr->at_mask_6 = cilix_ifaddr_at_mask_6;
    }

    cps->cpu = &cps->cpu__t; {
        cps->cpu->init = cilix_cpu_init;
        cps->cpu->fini = cilix_cpu_fini;
        cps->cpu->at_model = cilix_cpu_at_model;
        cps->cpu->at_speed = cilix_cpu_at_speed;
        cps->cpu->at_times_user = cilix_cpu_at_times_user;
        cps->cpu->at_times_nice = cilix_cpu_at_times_nice;
        cps->cpu->at_times_sys = cilix_cpu_at_times_sys;
        cps->cpu->at_times_idle = cilix_cpu_at_times_idle;
        cps->cpu->at_times_irq = cilix_cpu_at_times_irq;
    }

    cps->env = &cps->env__t; {
        cps->env->init = cilix_env_init;
        cps->env->fini = cilix_env_fini;
        cps->env->at = cilix_env_at;
        cps->env->get = cilix_env_get;
        cps->env->set = cilix_env_set;
        cps->env->unset = cilix_env_unset;
    }

    cps->mutex = &cps->mutex__t; {
        cps->mutex->init = cilix_mutex_init;
        cps->mutex->destroy = cilix_mutex_destroy;
        cps->mutex->lock = cilix_mutex_lock;
        cps->mutex->trylock = cilix_mutex_trylock;
        cps->mutex->unlock = cilix_mutex_unlock;
    }

    cps->thread = &cps->thread__t; {
        cps->thread->start = cilix_thread_start;
        cps->thread->wait = cilix_thread_wait;
        cps->thread->destroy = cilix_thread_destroy;
    }

    cps->toml = &cps->toml__t; {
        cps->toml->init = cilix_toml_init;
        cps->toml->destroy = cilix_toml_destroy;
        cps->toml->get_node_numb = cilix_toml_get_node_numb;
        cps->toml->get_keyv_numb = cilix_toml_get_keyv_numb;
        cps->toml->get_node_name_at = cilix_toml_get_node_name_at;
        cps->toml->get_keyv_name_at = cilix_toml_get_keyv_name_at;
        cps->toml->get_node_o = cilix_toml_get_node_o;
        cps->toml->get_keyv_s = cilix_toml_get_keyv_s;
        cps->toml->get_keyv_f = cilix_toml_get_keyv_f;
        cps->toml->get_keyv_i = cilix_toml_get_keyv_i;
        cps->toml->get_keyv_r = cilix_toml_get_keyv_r;
        cps->toml->r_elem_numb = cilix_toml_r_elem_numb;
        cps->toml->r_elem_s = cilix_toml_r_elem_s;
        cps->toml->r_elem_f = cilix_toml_r_elem_f;
        cps->toml->r_elem_i = cilix_toml_r_elem_i;
    }

    cps->json = &cps->json__t; {
        cps->json->init = cilix_json_init;
        cps->json->destroy = cilix_json_destroy;
        cps->json->get_obj = cilix_json_get_obj;
        cps->json->get_str = cilix_json_get_str;
        cps->json->get_int = cilix_json_get_int;
        cps->json->get_double = cilix_json_get_double;
        cps->json->get_arr = cilix_json_get_arr;
        cps->json->get_arr_size = cilix_json_get_arr_size;
        cps->json->get_arr_obj = cilix_json_get_arr_obj;
        cps->json->get_arr_str = cilix_json_get_arr_str;
        cps->json->get_arr_int = cilix_json_get_arr_int;
        cps->json->get_arr_double = cilix_json_get_arr_double;
        cps->json->add_obj = cilix_json_add_obj;
        cps->json->add_str = cilix_json_add_str;
        cps->json->add_int = cilix_json_add_int;
        cps->json->add_double = cilix_json_add_double;
        cps->json->add_arr = cilix_json_add_arr;
        cps->json->add_arr_obj = cilix_json_add_arr_obj;
        cps->json->add_arr_str = cilix_json_add_arr_str;
        cps->json->add_arr_int = cilix_json_add_arr_int;
        cps->json->add_arr_double = cilix_json_add_arr_double;
        cps->json->print = cilix_json_print;
        cps->json->print_free = cilix_json_print_free;
    }

}

sp_cp_t* sp_cp_create(void) {
    sp_cps_t* sp_cps = (sp_cps_t*)cilix_alloc_buf(sizeof(sp_cps_t)); {
        sp_cps_init(sp_cps);
    } return (sp_cp_t*)sp_cps;
}

void sp_cp_destroy(sp_cp_t* sp_cp) {
    cilix_alloc_fini(sp_cp);
}
