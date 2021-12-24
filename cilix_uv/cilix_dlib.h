#ifndef _CILIX_DLIB_H_
#define _CILIX_DLIB_H_

#include "uv.h"

// *** public - start ***
void* cilix_dlib_open(char* libname);
void* cilix_dlib_open_ae(char* libname);
void* cilix_dlib_open_aeps(char* libname);
void* cilix_dlib_sym(void* dlib, char* symname);
void cilix_dlib_close(void* dlib);
// *** public - end ***

#endif // #ifndef _CILIX_DLIB_H_
