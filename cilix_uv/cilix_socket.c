#include <stdio.h>
#include <stdlib.h>

#include "cilix_socket.h"
#include "cilix_tpp_sys.h"

void cilix_connector_async_cb(uv_async_t* handle);
void cilix_acceptor_async_cb(uv_async_t* handle);

cilix_wq_t* cilix_wq_malloc(cilix_socket_t* psk, void* base, unsigned long len) {
    cilix_wq_t* wq = (cilix_wq_t*)malloc(sizeof(cilix_wq_t));

    wq->sk = psk;
    wq->sk_flag = CILIX_WQ_FLAG_SK_WRITE;

    wq->buf.base = (char*)malloc(len+1);
    wq->buf.len = len; {
        memcpy(wq->buf.base, base, len);
    }

    QUEUE_INIT(&wq->node);

    return wq;
}

cilix_wq_t* cilix_wq_malloc_system(cilix_socket_t* psk, void* base, unsigned long baselen, char* protocol) {
    cilix_wq_t* wq = (cilix_wq_t*)malloc(sizeof(cilix_wq_t));

    wq->sk = psk;
    wq->sk_flag = CILIX_WQ_FLAG_SK_WRITE;

    unsigned long len = CILIX_TPP_SYS_ASSEM_LEN(protocol, baselen);
    wq->buf.base = (char*)malloc(len+1);
    wq->buf.len = len; {
        cilix_tpp_sys_assem(wq->buf.base, base, baselen, protocol);
    }

    QUEUE_INIT(&wq->node);

    return wq;
}

cilix_wq_t* cilix_wq_malloc_flag(cilix_socket_t* psk, void* base, unsigned long len, int flag) {
    cilix_wq_t* wq = (cilix_wq_t*)malloc(sizeof(cilix_wq_t));

    wq->sk = psk;
    wq->sk_flag = flag;

    wq->buf.base = (char*)malloc(len+1);
    wq->buf.len = len; {
        memcpy(wq->buf.base, base, len);
    }

    QUEUE_INIT(&wq->node);

    return wq;
}

void cilix_wq_free(cilix_wq_t* wq) {
    if (wq == 0) return;

    free(wq->buf.base);
    free(wq);
}

cilix_socket_t* cilix_socket_malloc(
        void* usession, void* powner, void* pownercontext, void* ptpp,
        uv_loop_t* ploop, cilix_skcb_t* pcbs,
        cilix_owner_socket_close_cb pcoscc) {
    cilix_socket_t* cs = (cilix_socket_t*)malloc(sizeof(cilix_socket_t));

    cs->router.u_session = usession;
    cs->router.owner = powner;
    cs->router.owner_context = pownercontext;
    cs->router.tpp = ptpp;
    cs->router.sk = cs;

    cs->cbs = pcbs;

    uv_tcp_init(ploop, &cs->tcp);
    cs->tcp.data = cs;

    cs->coscc = pcoscc;

    return cs;
}

void cilix_socket_free(cilix_socket_t* cs) {
    if (cs == 0) return;

    free(cs);
}

void cilix_socket_close_complete(uv_handle_t* handle) {
    cilix_socket_t* cs = (cilix_socket_t*)handle->data;

    if (cs != 0 && cs->cbs != 0 && cs->cbs->s_c_cb != 0) {
        cs->cbs->s_c_cb(&cs->router);
    }

    if (cs->coscc != 0) {
        cs->coscc(&cs->router);
    }
}

void cilix_socket_write_complete(uv_write_t* pwr, int status) {
    cilix_wq_t* wq = CILIX_FIELD2STRUCT(pwr, cilix_wq_t, wr);
    cilix_wq_free(wq);
}

void cilix_socket_read_malloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

void cilix_socket_read_complete(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    if (nread > 0) {
        cilix_socket_t* cs = (cilix_socket_t*)stream->data;
        if (cs != 0 && cs->cbs != 0 && cs->cbs->s_r_cb != 0) {
            cs->cbs->s_r_cb(&cs->router, buf->base, nread);
        }
    }
    if (nread < 0) {
        uv_close((uv_handle_t*)stream, cilix_socket_close_complete);
    }
    if (nread >= 0) {
        uv_read_start((uv_stream_t*)stream, cilix_socket_read_malloc, cilix_socket_read_complete);
    }

    free(buf->base);
}

void cilix_socket_connect_complete(uv_connect_t* connect, int status) {
    if (status) {
        cilix_socket_t* cs = (cilix_socket_t*)connect->handle->data;
        if (cs != 0 && cs->cbs != 0 && cs->cbs->s_e_cb != 0) {
            cs->cbs->s_e_cb(&cs->router, status);
        }
        uv_close((uv_handle_t*)connect->handle, cilix_socket_close_complete);
    } else {
        uv_read_start((uv_stream_t*)connect->handle, cilix_socket_read_malloc, cilix_socket_read_complete);
    }
}

cilix_connector_t* cilix_connector_malloc(void* usession, void* pcontext, void* tpp, cilix_skcb_t* pcbs) {
    cilix_connector_t* cc = (cilix_connector_t*)malloc(sizeof(cilix_connector_t));

    cc->stopf = 0;
    uv_loop_init(&cc->loop);

    uv_async_init(&cc->loop, &cc->iasync, cilix_connector_async_cb);
    cc->iasync.data = cc;

    cc->sk = cilix_socket_malloc(usession, cc, pcontext, tpp, &cc->loop, pcbs, 0);

    QUEUE_INIT(&cc->wqueue);
    uv_mutex_init(&cc->mtx);

    return cc;
}

void cilix_connector_free(cilix_connector_t* cc) {
    if (cc == 0) return;

    while (!QUEUE_EMPTY(&cc->wqueue)) {
        QUEUE* head = QUEUE_HEAD(&cc->wqueue);
        QUEUE_REMOVE(head);

        cilix_wq_t* wq = QUEUE_DATA(head, cilix_wq_t, node);
        cilix_wq_free(wq);
    }

    uv_mutex_destroy(&cc->mtx);
    cilix_socket_free(cc->sk);
    free(cc);
}

void cilix_connector_async_cb(uv_async_t* handle) {
    cilix_connector_t* cc = (cilix_connector_t*)handle->data;

    if (cc->stopf == 1) {
        uv_close((uv_handle_t*)&cc->iasync, 0);
        uv_close((uv_handle_t*)&cc->sk->tcp, cilix_socket_close_complete);

        cc->stopf = 0;
        return;
    }

    uv_mutex_lock(&cc->mtx);

    while (!QUEUE_EMPTY(&cc->wqueue)) {
        QUEUE* head = QUEUE_HEAD(&cc->wqueue);
        QUEUE_REMOVE(head);

        cilix_wq_t* wq = QUEUE_DATA(head, cilix_wq_t, node);
        if (wq->sk == cc->sk) {
            uv_write(&wq->wr, (uv_stream_t*)&cc->sk->tcp, &wq->buf, 1, cilix_socket_write_complete);
        } else {
            cilix_wq_free(wq);
        }
    }

    uv_mutex_unlock(&cc->mtx);
}

void cilix_connector_thr(void* arg) {
    cilix_connector_t* cc = (cilix_connector_t*)arg;

    uv_tcp_connect(&cc->connect, &cc->sk->tcp, (const struct sockaddr *)&cc->addr, cilix_socket_connect_complete);

    uv_run(&cc->loop, UV_RUN_DEFAULT);
    uv_loop_close(&cc->loop);
}

void cilix_connector_start(cilix_connector_t* cc, char* ip, int port) {
    uv_ip4_addr(ip, port, &cc->addr);
    uv_thread_create(&cc->t_id, cilix_connector_thr, cc);
}

void cilix_connector_stop(cilix_connector_t* cc) {
    cc->stopf = 1;
    uv_async_send(&cc->iasync);
}

void cilix_connector_wait(cilix_connector_t* cc) {
    uv_thread_join(&cc->t_id);
}

void cilix_connector_write(cilix_connector_t* cc, void* base, unsigned long len) {
    cilix_wq_t* wq = cilix_wq_malloc(cc->sk, base, len);

    uv_mutex_lock(&cc->mtx);
    QUEUE_INSERT_TAIL(&cc->wqueue, &wq->node);
    uv_mutex_unlock(&cc->mtx);

    uv_async_send(&cc->iasync);
}

void cilix_connector_write_system(cilix_connector_t* cc, void* base, unsigned long len, char* protocol) {
    cilix_wq_t* wq = cilix_wq_malloc_system(cc->sk, base, len, protocol);

    uv_mutex_lock(&cc->mtx);
    QUEUE_INSERT_TAIL(&cc->wqueue, &wq->node);
    uv_mutex_unlock(&cc->mtx);

    uv_async_send(&cc->iasync);
}

cilix_sq_t* cilix_sq_malloc(cilix_socket_t* psk) {
    cilix_sq_t* sq = (cilix_sq_t*)malloc(sizeof(cilix_sq_t));

    sq->sk = psk;
    QUEUE_INIT(&sq->node);

    return sq;
}

void cilix_sq_free(cilix_sq_t* sq) {
    if (sq == 0) return;

    cilix_socket_free(sq->sk);
    free(sq);
}

cilix_acceptor_t* cilix_acceptor_malloc(void* pcontext, void* ptpp, cilix_skcb_t* pcbs) {
    cilix_acceptor_t* ca = (cilix_acceptor_t*)malloc(sizeof(cilix_acceptor_t)); {
        ca->ready_cb = 0;
        ca->ready_cb_ctx = 0;
    }

    ca->context = pcontext;
    ca->tpp = ptpp;

    ca->stopf = 0;
    uv_loop_init(&ca->loop);

    uv_tcp_init(&ca->loop, &ca->tcp);
    ca->tcp.data = ca;

    uv_async_init(&ca->loop, &ca->iasync, cilix_acceptor_async_cb);
    ca->iasync.data = ca;

    QUEUE_INIT(&ca->squeue);
    ca->cbs = pcbs;

    QUEUE_INIT(&ca->wqueue);
    uv_mutex_init(&ca->mtx);

    return ca;
}

void cilix_acceptor_free(cilix_acceptor_t* ca) {
    if (ca == 0) return;

    while (!QUEUE_EMPTY(&ca->squeue)) {
        QUEUE* head = QUEUE_HEAD(&ca->squeue);
        QUEUE_REMOVE(head);

        cilix_sq_t* sq = QUEUE_DATA(head, cilix_sq_t, node);
        cilix_sq_free(sq);
    }

    while (!QUEUE_EMPTY(&ca->wqueue)) {
        QUEUE* head = QUEUE_HEAD(&ca->wqueue);
        QUEUE_REMOVE(head);

        cilix_wq_t* wq = QUEUE_DATA(head, cilix_wq_t, node);
        cilix_wq_free(wq);
    }

    uv_mutex_destroy(&ca->mtx);
    free(ca);
}

void cilix_acceptor_socket_close_cb(cilix_socket_router_t* router) {
    cilix_acceptor_t* ca = (cilix_acceptor_t*)router->owner;
    cilix_socket_t* cs = (cilix_socket_t*)router->sk;

    QUEUE *q;
    QUEUE_FOREACH(q, &ca->squeue) {
        cilix_sq_t* sq = QUEUE_DATA(q, cilix_sq_t, node);
        if (sq->sk == cs) {
            QUEUE_REMOVE(q);
            cilix_sq_free(sq);
            break;
        }
    }
}

void cilix_acceptor_in(uv_stream_t* stream, int status) {
    if (status) return;
    cilix_acceptor_t* ca = (cilix_acceptor_t*)stream->data;

    cilix_socket_t* cs = cilix_socket_malloc(0, ca, ca->context, ca->tpp, &ca->loop, ca->cbs, cilix_acceptor_socket_close_cb);
    {
        cilix_sq_t *sq = cilix_sq_malloc(cs);
        QUEUE_INSERT_TAIL(&ca->squeue, &sq->node);
    }
    if (cs != 0 && cs->cbs != 0 && cs->cbs->s_n_cb != 0) {
        cs->cbs->s_n_cb(&cs->router);
    }

    int r = uv_accept(stream, (uv_stream_t*)&cs->tcp);
    if (r) {
        uv_close((uv_handle_t*)&cs->tcp, cilix_socket_close_complete);
    } else {
        uv_read_start((uv_stream_t*)&cs->tcp, cilix_socket_read_malloc, cilix_socket_read_complete);
    }
}

void cilix_acceptor_async_cb(uv_async_t* handle) {
    cilix_acceptor_t* ca = (cilix_acceptor_t*)handle->data;

    if (ca->stopf == 1) {
        uv_close((uv_handle_t*)&ca->iasync, 0);
        uv_close((uv_handle_t*)&ca->tcp, 0);

        QUEUE *q;
        QUEUE_FOREACH(q, &ca->squeue) {
            cilix_sq_t* sq = QUEUE_DATA(q, cilix_sq_t, node);
            uv_close((uv_handle_t*)&sq->sk->tcp, cilix_socket_close_complete);
        }

        ca->stopf = 0;
        return;
    }

    uv_mutex_lock(&ca->mtx);

    while (!QUEUE_EMPTY(&ca->wqueue)) {
        QUEUE* head = QUEUE_HEAD(&ca->wqueue);
        QUEUE_REMOVE(head);

        int wo = 0;
        cilix_wq_t* wq = QUEUE_DATA(head, cilix_wq_t, node);

        QUEUE* q;
        QUEUE_FOREACH(q, &ca->squeue) {
            cilix_sq_t* sq = QUEUE_DATA(q, cilix_sq_t, node);
            if (wq->sk == sq->sk) {
                if (wq->sk_flag == CILIX_WQ_FLAG_SK_CLOSE) {
                    uv_close((uv_handle_t*)&sq->sk->tcp, cilix_socket_close_complete);
                } else {
                    wo = 1;
                    uv_write(&wq->wr, (uv_stream_t*)&sq->sk->tcp, &wq->buf, 1, cilix_socket_write_complete);
                }
                break;
            }
        }

        if (wo == 0) {
            cilix_wq_free(wq);
        }
    }

    uv_mutex_unlock(&ca->mtx);
}

void cilix_acceptor_thr(void* arg) {
    cilix_acceptor_t* ca = (cilix_acceptor_t*)arg;

    uv_tcp_bind(&ca->tcp, (const struct sockaddr*)&ca->addr, 0);
    uv_listen((uv_stream_t*)&ca->tcp, 128, cilix_acceptor_in);

    if (ca->ready_cb) {
        ca->ready_cb(ca->ready_cb_ctx);
    }

    uv_run(&ca->loop, UV_RUN_DEFAULT);
    uv_loop_close(&ca->loop);
}

void cilix_acceptor_start(cilix_acceptor_t* ca, char* ip, int port, cilix_acceptor_ready_cb ready_cb, void* rctx) {
    ca->ready_cb = ready_cb;
    ca->ready_cb_ctx = rctx;

    uv_ip4_addr(ip, port, &ca->addr);
    uv_thread_create(&ca->t_id, cilix_acceptor_thr, ca);
}

void cilix_acceptor_stop(cilix_acceptor_t* ca) {
    ca->stopf = 1;
    uv_async_send(&ca->iasync);
}

void cilix_acceptor_wait(cilix_acceptor_t* ca) {
    uv_thread_join(&ca->t_id);
}

void cilix_acceptor_write(cilix_socket_router_t* router, void* base, unsigned long len) {
    cilix_acceptor_t* ca = (cilix_acceptor_t*)router->owner;
    cilix_socket_t* cs = (cilix_socket_t*)router->sk;
    cilix_wq_t* wq = cilix_wq_malloc(cs, base, len);

    uv_mutex_lock(&ca->mtx);
    QUEUE_INSERT_TAIL(&ca->wqueue, &wq->node);
    uv_mutex_unlock(&ca->mtx);

    uv_async_send(&ca->iasync);
}

void cilix_acceptor_write_system(cilix_socket_router_t* router, void* base, unsigned long len, char* protocol) {
    cilix_acceptor_t* ca = (cilix_acceptor_t*)router->owner;
    cilix_socket_t* cs = (cilix_socket_t*)router->sk;
    cilix_wq_t* wq = cilix_wq_malloc_system(cs, base, len, protocol);

    uv_mutex_lock(&ca->mtx);
    QUEUE_INSERT_TAIL(&ca->wqueue, &wq->node);
    uv_mutex_unlock(&ca->mtx);

    uv_async_send(&ca->iasync);
}

void cilix_acceptor_write_flag(cilix_socket_router_t* router, void* base, unsigned long len, int flag) {
    cilix_acceptor_t* ca = (cilix_acceptor_t*)router->owner;
    cilix_socket_t* cs = (cilix_socket_t*)router->sk;
    cilix_wq_t* wq = cilix_wq_malloc_flag(cs, base, len, flag);

    uv_mutex_lock(&ca->mtx);
    QUEUE_INSERT_TAIL(&ca->wqueue, &wq->node);
    uv_mutex_unlock(&ca->mtx);

    uv_async_send(&ca->iasync);
}