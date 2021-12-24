#include <stdio.h>
#include <stdlib.h>

#include "uv.h"

void signal_handler(uv_signal_t* handle, int signum)
{
    printf("cilix_test_signale received signum: %d\n", signum);
    if (signum == SIGINT) {
        uv_signal_stop(handle);
    }
}

void cilix_test_signal(void) {
    uv_loop_t loop;
    uv_loop_init(&loop);

    uv_signal_t sig_int; {
        uv_signal_init(&loop, &sig_int);
        uv_signal_start(&sig_int, signal_handler, SIGINT);
    }
    uv_signal_t sig_usr1; {
        uv_signal_init(&loop, &sig_usr1);
        uv_unref((uv_handle_t*)&sig_usr1);
        uv_signal_start(&sig_usr1, signal_handler, SIGUSR1);
    }
    uv_signal_t sig_usr2; {
        uv_signal_init(&loop, &sig_usr2);
        uv_unref((uv_handle_t*)&sig_usr2);
        uv_signal_start(&sig_usr2, signal_handler, SIGUSR2);
    }

    printf("cilix_test_signale start.\n");

    uv_run(&loop, UV_RUN_DEFAULT);
    uv_loop_close(&loop);

    printf("cilix_test_signale stop.\n");
}
