#include <stdio.h>
#include <stdlib.h>

#include "uv.h"

#include "cilix_os_config.h"
#include CILIX_SOCKET_ADDR_H
#include CILIX_STRING_H

#define TTAB "    "

void cilix_test_sys(void) {
    int iret = 0;

    {
        char buf[2048+1] = {0};
        size_t isize = 2048;
        iret = uv_os_gethostname(buf, &isize);

        printf("uv_os_gethostname return %d\n", iret);
        printf(TTAB "[%s]\n", buf);
    }

    {
        uv_utsname_t utsname = {0};
        iret = uv_os_uname(&utsname);

        printf("uv_os_uname return %d\n", iret);
        printf(TTAB "sysname [%s]\n", utsname.sysname);
        printf(TTAB "release [%s]\n", utsname.release);
        printf(TTAB "version [%s]\n", utsname.version);
        printf(TTAB "machine [%s]\n", utsname.machine);
    }

    {
        char buf[2048+1] = {0};
        size_t isize = 2048;
        iret = uv_get_process_title(buf, isize);

        printf("uv_get_process_title return %d\n", iret);
        printf(TTAB "[%s]\n", buf);

        char buf1[2048+1] = {0};
        strcat(buf1, buf);
        strcat(buf1, "_set");
        iret = uv_set_process_title(buf1);
        printf("uv_set_process_title return %d\n", iret);
    }

    {
        uv_pid_t pid = uv_os_getpid();
        uv_pid_t ppid = uv_os_getppid();

        printf("pid [%d], ppid [%d]\n", pid, ppid);
    }

    {
        uv_cpu_info_t* ci = 0;
        int num = 0;
        iret = uv_cpu_info(&ci, &num);

        printf("uv_cpu_info return %d , num = %d\n", iret, num);
        for (int i=0; i<num; i++) {
            printf(TTAB "cpu [%d]\n", i);
            printf(TTAB TTAB "model [%s]\n", ci[i].model);
            printf(TTAB TTAB "speed [%d]\n", ci[i].speed);
            printf(TTAB TTAB "cpu_times.user [%llu]\n", (unsigned long long)ci[i].cpu_times.user);
            printf(TTAB TTAB "cpu_times.nice [%llu]\n", (unsigned long long)ci[i].cpu_times.nice);
            printf(TTAB TTAB "cpu_times.sys [%llu]\n", (unsigned long long)ci[i].cpu_times.sys);
            printf(TTAB TTAB "cpu_times.idle [%llu]\n", (unsigned long long)ci[i].cpu_times.idle);
            printf(TTAB TTAB "cpu_times.irq [%llu]\n", (unsigned long long)ci[i].cpu_times.irq);
        }

        uv_free_cpu_info(ci, num);
    }

    {
        uv_interface_address_t* ias = 0;
        int num = 0;
        iret = uv_interface_addresses(&ias, &num);

        printf("uv_interface_addresses return %d , num = %d\n", iret, num);
        for (int i=0; i<num; i++) {
            printf(TTAB "addrs [%d]\n", i);
            printf(TTAB TTAB "name [%s]\n", ias[i].name);
            printf(TTAB TTAB "is_internal [%d]\n", ias[i].is_internal);
            {
                char phys_addr[100] = "";
                for (int p=0; p<6; p++) {
                    sprintf(phys_addr+strlen(phys_addr), "%s%02X", (p==0?"":":"), (unsigned char)(ias[i].phys_addr[p]));
                }
                printf(TTAB TTAB "phys_addr [%s]\n", phys_addr);
            }
            {
                char addr[100] = "";
                iret = uv_ip4_name(&ias[i].address.address4, addr, 100);
                printf(TTAB TTAB "address4 [%d][%s]\n", iret, addr);
            }
            {
                char addr[100] = "";
                iret = uv_ip6_name(&ias[i].address.address6, addr, 100);
                printf(TTAB TTAB "address6 [%d][%s]\n", iret, addr);
            }
            {
                char mask[100] = "";
                iret = uv_ip4_name(&ias[i].netmask.netmask4, mask, 100);
                printf(TTAB TTAB "netmask4 [%d][%s]\n", iret, mask);
            }
            {
                char mask[100] = "";
                iret = uv_ip6_name(&ias[i].netmask.netmask6, mask, 100);
                printf(TTAB TTAB "netmask6 [%d][%s]\n", iret, mask);
            }
        }

        uv_free_interface_addresses(ias, num);
    }

    {
        char buf[2048+1] = {0};
        size_t isize = 2048;
        iret = uv_exepath(buf, &isize);

        printf("uv_exepath return %d\n", iret);
        printf(TTAB "[%s]\n", buf);
    }

    {
        char buf[2048+1] = {0};
        size_t isize = 2048;
        iret = uv_cwd(buf, &isize);

        printf("uv_cwd return %d\n", iret);
        printf(TTAB "[%s]\n", buf);
    }

    {
        //uv_chdir
    }

    {
        char buf[2048+1] = {0};
        size_t isize = 2048;
        iret = uv_os_homedir(buf, &isize);

        printf("uv_os_homedir return %d\n", iret);
        printf(TTAB "[%s]\n", buf);
    }

    {
        char buf[2048+1] = {0};
        size_t isize = 2048;
        iret = uv_os_tmpdir(buf, &isize);

        printf("uv_os_tmpdir return %d\n", iret);
        printf(TTAB "[%s]\n", buf);
    }

    {
        uv_passwd_t pw = {0};
        iret = uv_os_get_passwd(&pw);

        printf("uv_os_get_passwd return %d\n", iret);
        printf(TTAB "username [%s]\n", pw.username);
        printf(TTAB "uid [%ld]\n", pw.uid);
        printf(TTAB "gid [%ld]\n", pw.gid);
        printf(TTAB "shell [%s]\n", pw.shell);
        printf(TTAB "homedir [%s]\n", pw.homedir);

        uv_os_free_passwd(&pw);
    }

    {
        unsigned long long tm = uv_get_total_memory();
        unsigned long long fm = uv_get_free_memory();
        printf("uv_get_total_memory [%llu]\n", tm);
        printf("uv_get_free_memory [%llu]\n", fm);
    }

    {
        uv_env_item_t* envs = 0;
        int num = 0;
        iret = uv_os_environ(&envs, &num);

        printf("uv_os_environ return %d , num = %d\n", iret, num);
        for (int i=0; i<num; i++) {
            printf(TTAB "[%d] %s=%s\n", i, envs[i].name, envs[i].value);
        }

        uv_os_free_environ(envs, num);
    }

    {
        //uv_os_getenv
        //uv_os_setenv
        //uv_os_unsetenv
    }
}
