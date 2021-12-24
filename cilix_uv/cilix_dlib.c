#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "cilix_system.h"
#include "cilix_dlib.h"

#include "Config.h"

void* cilix_dlib_open(char* libname) {
    uv_lib_t* dlib = (uv_lib_t*)malloc(sizeof(uv_lib_t));
    memset(dlib, 0, sizeof(uv_lib_t));

    if (uv_dlopen(libname, dlib) != 0) {
        cilix_dlib_close(dlib);
        return 0;
    } return dlib;
}

void* cilix_dlib_open_ae(char* libname) {
    char libpath[1025] = ""; {
        cilix_path_exe_path_patch_name(0, libname, libpath);
    } return cilix_dlib_open(libpath);
}

void* cilix_dlib_open_aeps(char* libname) {
    char libpath[1025] = ""; {
        cilix_path_exe_path_patch_name_shared_ps(0, libname, libpath);
    } return cilix_dlib_open(libpath);
}

void* cilix_dlib_sym(void* dlib, char* symname) {
    if (dlib == 0) return 0;

    void* ptr = 0;
    if (uv_dlsym((uv_lib_t*)dlib, symname, &ptr) != 0) {
        return 0;
    } return ptr;
}

void cilix_dlib_close(void* dlib) {
    if (dlib == 0) return;

    uv_dlclose((uv_lib_t*)dlib);
    free(dlib);
}