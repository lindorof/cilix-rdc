#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "cilix_rdc.h"
#include "cilix_rdc_task.h"
#include "cilix_rdc_seq.h"

void __rdc_loop_sig_cb(uv_signal_t* handle, int signum)
{
    if (signum == SIGINT) {
        uv_signal_stop(handle);
    }
}

void __rdc_loop_run() {
    uv_loop_t loop;
    uv_loop_init(&loop);

    uv_signal_t sig_exit; {
        uv_signal_init(&loop, &sig_exit);
        uv_signal_start(&sig_exit, __rdc_loop_sig_cb, SIGINT);
    }

    uv_run(&loop, UV_RUN_DEFAULT);
    uv_loop_close(&loop);
}

rdc_t* rdc_malloc_start(char* dlibphy, char* dlibname, char* ip, int port) {
    rdc_t* rdc = (rdc_t*)malloc(sizeof(rdc_t));

    rdc->dlib_h = 0;
    sp_cps_init(&rdc->dlib_cp);
    strcpy(rdc->dlib_phy, dlibphy);
    strcpy(rdc->dlib_name, dlibname);

    rdc->task = cilix_task_malloc(rdc, rdc_task_init_cb, rdc_task_fini_cb, rdc_task_q_get_cb, rdc_task_q_clean_cb);
    cilix_task_start(rdc->task);

    rdc->skcbs.s_n_cb = rdc_seq_socket_new_cb;
    rdc->skcbs.s_e_cb = 0;
    rdc->skcbs.s_r_cb = rdc_seq_socket_read_cb;
    rdc->skcbs.s_c_cb = rdc_seq_socket_close_cb;

    rdc->acceptor = cilix_acceptor_malloc(rdc, 0, &rdc->skcbs);
    cilix_acceptor_start(rdc->acceptor, ip, port, 0, 0);

    return rdc;
}

void rdc_stop_free(rdc_t* rdc) {
    if (rdc != 0) {
        cilix_acceptor_stop(rdc->acceptor);
        cilix_acceptor_wait(rdc->acceptor);
        cilix_acceptor_free(rdc->acceptor);

        cilix_task_stop(rdc->task);
        cilix_task_wait(rdc->task);
        cilix_task_free(rdc->task);
    } free(rdc);
}

#define _ARG_TEST   "-t"
#define _ARG_INIT   "-init"

#define IF_ARG_INVALID_EXIT \
    { \
        if (argc < 2) return 0; \
        if (CILIX_STRICMP(argv[1],_ARG_TEST) && CILIX_STRICMP(argv[1],_ARG_INIT)) return 0; \
    } \

#define IF_ARG_TEST_EXIT \
    { \
        if (CILIX_STRICMP(argv[1],_ARG_TEST)==0) return cilix_test_x(argc,argv); \
    } \

#define ARG_DLIB_PHY    (argc > 2 ? argv[2] : "")
#define ARG_DLIB_NAME   (argc > 3 ? argv[3] : "")
#define ARG_IP          (argc > 4 ? argv[4] : "")
#define ARG_PORT        (argc > 5 ? argv[5] : "")

int cilix_test_x(int argc, char **argv);

int main(int argc, char **argv)
{
    IF_ARG_INVALID_EXIT
    IF_ARG_TEST_EXIT

    rdc_t* rdc = rdc_malloc_start(ARG_DLIB_PHY, ARG_DLIB_NAME, ARG_IP, (int)strtol(ARG_PORT, 0, 10));
    __rdc_loop_run();
    rdc_stop_free(rdc);

    return 0;
}
