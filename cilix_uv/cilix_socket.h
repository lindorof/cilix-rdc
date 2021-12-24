#ifndef _CILIX_SOCKET_H_
#define _CILIX_SOCKET_H_

#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_SOCKET_ADDR_H
#include CILIX_STRING_H

#include "uv.h"
#include "queue.h"

#define CILIX_FIELD2STRUCT(ptr, type, field)                \
    ((type *) ((char *) (ptr) - offsetof(type, field)))     \

#define CILIX_WQ_FLAG_SK_WRITE  (0)
#define CILIX_WQ_FLAG_SK_CLOSE  (-1)

typedef struct cilix_wq_s cilix_wq_t;
typedef struct cilix_sq_s cilix_sq_t;
typedef struct cilix_skcb_s cilix_skcb_t;
typedef struct cilix_socket_s cilix_socket_t;
typedef struct cilix_socket_router_s cilix_socket_router_t;
typedef struct cilix_acceptor_s cilix_acceptor_t;
typedef struct cilix_connector_s cilix_connector_t;

// *** socket-cb - start ***
// for acceptor only
typedef void (*cilix_socket_new_cb)(cilix_socket_router_t* router);
// for connector only
typedef void (*cilix_socket_connect_err_cb)(cilix_socket_router_t* router, int sys_err);
// for all sockets
typedef void (*cilix_socket_read_cb)(cilix_socket_router_t* router, void* base, long len);
typedef void (*cilix_socket_close_cb)(cilix_socket_router_t* router);
// *** socket-cb - end ***

// *** public - start ***
// for connector
cilix_connector_t* cilix_connector_malloc(void* usession, void* pcontext, void* tpp, cilix_skcb_t* pcbs);
void cilix_connector_free(cilix_connector_t* cc);
void cilix_connector_start(cilix_connector_t* cc, char* ip, int port);
void cilix_connector_stop(cilix_connector_t* cc);
void cilix_connector_wait(cilix_connector_t* cc);
void cilix_connector_write(cilix_connector_t* cc, void* base, unsigned long len);
// LEN[8]PROTOCOL[*]^DATA
void cilix_connector_write_system(cilix_connector_t* cc, void* base, unsigned long len, char* protocol);
// for acceptor
typedef void (*cilix_acceptor_ready_cb)(void* rctx);
cilix_acceptor_t* cilix_acceptor_malloc(void* pcontext, void* tpp, cilix_skcb_t* pcbs);
void cilix_acceptor_free(cilix_acceptor_t* ca);
void cilix_acceptor_start(cilix_acceptor_t* ca, char* ip, int port, cilix_acceptor_ready_cb ready_cb, void* rctx);
void cilix_acceptor_stop(cilix_acceptor_t* ca);
void cilix_acceptor_wait(cilix_acceptor_t* ca);
void cilix_acceptor_write(cilix_socket_router_t* router, void* base, unsigned long len);
// LEN[8]PROTOCOL[*]^DATA
void cilix_acceptor_write_system(cilix_socket_router_t* router, void* base, unsigned long len, char* protocol);
// FLAG
void cilix_acceptor_write_flag(cilix_socket_router_t* router, void* base, unsigned long len, int flag);
// *** public - end ***

// *** public - start ***
cilix_wq_t* cilix_wq_malloc(cilix_socket_t* psk, void* base, unsigned long len);
cilix_wq_t* cilix_wq_malloc_system(cilix_socket_t* psk, void* base, unsigned long baselen, char* protocol);
cilix_wq_t* cilix_wq_malloc_flag(cilix_socket_t* psk, void* base, unsigned long len, int flag);
void cilix_wq_free(cilix_wq_t* wq);
// *** public - end ***

struct cilix_skcb_s {
    cilix_socket_new_cb s_n_cb;
    cilix_socket_connect_err_cb s_e_cb;
    cilix_socket_read_cb s_r_cb;
    cilix_socket_close_cb s_c_cb;
};

struct cilix_socket_router_s {
    void* u_session;
    void* owner;
    void* owner_context;
    void* tpp;
    void* sk;
};

struct cilix_wq_s {
    cilix_socket_t* sk;
    int sk_flag;

    uv_write_t wr;
    uv_buf_t buf;

    QUEUE node;
};

typedef void (*cilix_owner_socket_close_cb)(cilix_socket_router_t* router);
struct cilix_socket_s {
    cilix_socket_router_t router;
    cilix_skcb_t* cbs;
    uv_tcp_t tcp;

    cilix_owner_socket_close_cb coscc;
};

struct cilix_connector_s {
    uv_connect_t connect;
    struct sockaddr_in addr;

    int stopf;
    uv_loop_t loop;
    uv_async_t iasync;

    cilix_socket_t* sk;

    QUEUE wqueue;
    uv_mutex_t mtx;

    uv_thread_t t_id;
};

struct cilix_sq_s {
    cilix_socket_t* sk;
    QUEUE node;
};

struct cilix_acceptor_s {
    cilix_acceptor_ready_cb ready_cb;
    void* ready_cb_ctx;

    void* context;
    void* tpp;
    struct sockaddr_in addr;

    int stopf;
    uv_loop_t loop;
    uv_tcp_t tcp;
    uv_async_t iasync;

    QUEUE squeue;
    cilix_skcb_t* cbs;

    QUEUE wqueue;
    uv_mutex_t mtx;

    uv_thread_t t_id;
};

#endif // #ifndef _CILIX_SOCKET_H_
