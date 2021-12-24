#ifndef _CILIX_THREAD_H_
#define _CILIX_THREAD_H_

// *** public - start ***
void* cilix_thread_start(void* func, void* arg);
void cilix_thread_wait(void* t);
void cilix_thread_destroy(void* t);
// *** public - end ***

#endif // #ifndef _CILIX_THREAD_H_
