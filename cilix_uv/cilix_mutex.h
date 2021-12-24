#ifndef _CILIX_MUTEX_H_
#define _CILIX_MUTEX_H_

// *** public - start ***
void* cilix_mutex_init(void);
void cilix_mutex_destroy(void* mtx);
void cilix_mutex_lock(void* mtx);
int cilix_mutex_trylock(void* mtx);
void cilix_mutex_unlock(void* mtx);
// *** public - end ***

#endif // #ifndef _CILIX_MUTEX_H_
