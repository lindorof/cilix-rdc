#include "cilix_timeout.h"

void* cilix_timeout_init(int timeout) {
    if (timeout <= 0) return 0;

    cilix_timeout_t *tmt = (cilix_timeout_t*)malloc(sizeof(cilix_timeout_t));
    {
        tmt->timeout = timeout;

        uv_loop_init(&tmt->loop);
        uv_async_init(&tmt->loop, &tmt->iasync, cilix_timeout_async_cb);
        tmt->iasync.data = tmt;
        uv_timer_init(&tmt->loop, &tmt->timer);
        tmt->timer.data = tmt;
        
        tmt->flag = 0;
        tmt->loop_t_stop = 0;
    }
    uv_thread_create(&tmt->t_id, cilix_timeout_thr, tmt);

    return tmt;
}

void cilix_timeout_destroy(void* pt) {
    if (pt == 0) return;

    cilix_timeout_t *tmt = (cilix_timeout_t*)pt;
    {
        tmt->loop_t_stop = 1;
        uv_async_send(&tmt->iasync);
        uv_thread_join(&tmt->t_id);
    }
    free(tmt);
}

int cilix_timeout_flag(void* pt) {
    if (pt == 0) return 0;

    return ((cilix_timeout_t*)pt)->flag;
}

void cilix_timeout_sleep(int timeout) {
    if (timeout <= 0) return;

    uv_sleep(timeout);
}

void cilix_timeout_async_cb(uv_async_t* handle) {
    cilix_timeout_t* tmt = (cilix_timeout_t*)handle->data;

    if (tmt->loop_t_stop) {
        uv_timer_stop(&tmt->timer);
        uv_close((uv_handle_t*)handle, NULL);
        return;
    }
}

void cilix_timeout_timer_cb(uv_timer_t* handle) {
    cilix_timeout_t* tmt = (cilix_timeout_t*)handle->data;

    tmt->flag = 1;
    tmt->loop_t_stop = 1;
    uv_async_send(&tmt->iasync);
}

void cilix_timeout_thr(void* arg) {
    cilix_timeout_t* tmt = (cilix_timeout_t*)arg;

    uv_timer_start(&tmt->timer, cilix_timeout_timer_cb, tmt->timeout, 0);

    uv_run(&tmt->loop, UV_RUN_DEFAULT);
    uv_loop_close(&tmt->loop);
}