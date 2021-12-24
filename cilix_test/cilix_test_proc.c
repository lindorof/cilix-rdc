#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "Config.h"

#include "uv.h"

#include "cilix_system.h"
#include "cilix_file.h"

int g_exit = 0;
void on_proc_exit(uv_process_t* proc, int64_t exit_status, int term_signal) {
    fprintf(stderr, "cilix_test_proc Process exited with status %lld, signal %d\n", (long long)exit_status, term_signal);
    uv_close((uv_handle_t*)proc, NULL);
    g_exit = 1;
}

void thr_send_sig(void *arg)
{
    uv_process_t* proc = (uv_process_t*)arg;
    int signum = 0;

    /*
    SIGINT  2
    SIGUSR1 30
    SIGUSR2 31
    */
    for (int i=1;!g_exit; i++) {
    fprintf(stderr, "...[%d] cilix_test_proc input signum to send (%d[exit]/%d/%d) ...\n", i, SIGINT, SIGUSR1, SIGUSR2);
        scanf("%d",&signum);
        //uv_process_kill(proc, signum);
        uv_kill(uv_process_get_pid(proc), signum);
    }
}

void prepare_proc_file(char* procname, char* file) {
    char EXEPATH[1025] = "";
    char EXENAME[1025] = "";
    char EXENAMENSF[1025] = "";
    cilix_path_exe_path(0, EXEPATH);
    cilix_path_exe_name(0, EXENAME);
    cilix_path_exe_name_no_suffix(0, EXENAMENSF);

    char oldf[1025] = ""; {
        strcat(oldf, EXEPATH);
        strcat(oldf, EXENAME);
    }
    char newf[1025] = ""; {
        strcat(newf, EXEPATH);
        strcat(newf, EXENAMENSF);
        strcat(newf, "_");
        strcat(newf, procname);
        strcat(newf, _CILIX_OS_EXECUTABLE_SUFFIX);
    }

    if (0) {
        cilix_file_move(oldf, newf, 1, 0);
        strcpy(file, newf);
    }
    if (1) {
        strcpy(file, oldf);
    }
}

void cilix_test_proc(void) {
    uv_loop_t *loop = uv_default_loop();

    char _args[10][1025] = {0}; {
        prepare_proc_file("sig", _args[0]);
        strcpy(_args[1], "-t");
        strcpy(_args[2], "-sig");
    }
    char* args[10] = {0}; {
        args[0] = _args[0];
        args[1] = _args[1];
        args[2] = _args[2];
    }

    uv_stdio_container_t child_stdio[3] = {0};
    child_stdio[0].flags = UV_INHERIT_FD;
    child_stdio[0].data.fd = 0;
    child_stdio[1].flags = UV_INHERIT_FD;
    child_stdio[1].data.fd = 1;
    child_stdio[2].flags = UV_INHERIT_FD;
    child_stdio[2].data.fd = 2;

    uv_process_options_t ops = {0}; {
        ops.exit_cb = on_proc_exit;
        ops.file = _args[0];
        ops.args = args;

        ops.stdio_count = 3;
        ops.stdio = child_stdio;
    }

    uv_process_t proc = {0};
    int r;
    if ((r = uv_spawn(loop, &proc, &ops))) {
        fprintf(stderr, "cilix_test_proc uv_spawn return [%d][%s]\n", r, uv_strerror(r));
        return;
    } else {
        fprintf(stderr, "cilix_test_proc Launched process with ID %d\n", proc.pid);
    }

    uv_thread_t tid;
    uv_thread_create(&tid, thr_send_sig, &proc);

    uv_run(loop, UV_RUN_DEFAULT);

    uv_thread_join(&tid);

    fprintf(stderr, "cilix_test_proc stop\n");
}
