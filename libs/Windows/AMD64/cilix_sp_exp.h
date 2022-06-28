#ifndef _CILIX_SP_EXP_H_
#define _CILIX_SP_EXP_H_

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#ifdef _WIN32
#ifdef _CILIX_ON_BUILDING
#define _CILIX_SP_EXP EXTERNC __declspec(dllexport)
#else
#define _CILIX_SP_EXP EXTERNC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define _CILIX_SP_EXP EXTERNC __attribute__((visibility("default")))
#else
#define _CILIX_SP_EXP EXTERNC
#endif

#endif // #ifndef _CILIX_SP_EXP_H_
